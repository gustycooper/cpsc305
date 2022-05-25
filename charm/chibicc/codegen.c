#include "chibicc.h"

/*
Charm Data Types - Approach for data types.
char - implement 
short - same as int
int - implement
float - implement
double - same as float
long double - same as float

Unitialized static data is allocated values in the .s output as if they are initialized.
Future - create a .bss section for unitialized data (maybe).

TODO - Update so that double literals and variables are treated as float
This involves reaching into the parser. For example

float f = 1.125;
.label f
  0x3f900000
double d = 1.125;
.label d
  0x0          <- I think these are backwards
  0x3ff20000

Currently chasm only implements 32-bits for static data
TODO - (Maybe) Add to chasm the ability for have a .byte where numbers are allocated one byte
The following allocates 3 bytes and a hole in front of the .data
.byte // subsequent numbers are allocated one byte
.label a
5
.label b
6
.label c
7
.data // subsequent numbers are allocated four bytes
.label i
23

NOTE: Currently for chibicc, when there are two static char.
char a = 'G';
char b = 34;
chibicc allocates 32-bits for each variable.
.label b
0x22000000
.label c
0x47000000
This may be just fine.
 */

/* TODO
I have to study how chibbicc allocates the offsets for variables.
Local variables have a negative offset, i.e., var->off is negative.
These are offsets from the frame pointer. See below for diagram of how it works.
On Intel esp (stack pointer) and ebp (frame (or base) pointer).
I need to correctly implement fp and sp. Need some more study
I use positive offsets from the frame pointer, so I use -var->off.
This works for now; however, I have had to fiddle with the stack_size and REG_SPACE
With more study, I can better implement the stack for functions. 
I am overallocating space for the stack.
Sample code and variable offsets

int add_me(int i, int j) {
    int x = 5;

The offsets for i and j are 20 and 24
The offset for x is 4

Somehow the following define is used in stack allocation
#define REG_SPACE 9*4

       +----------------+
sp->   |                |
       +----------------+
       |                |
       +----------------+
       |                | local var a has offset -8 from fp
       +----------------+
       |                |
       +----------------+
fp->   |                |
       +----------------+
       |                | param v passed on stack has offset 4 from fp
       +----------------+
       |                |
       +----------------+

 */

/*
Registers
r13 - stack pointer
r12 - frame pointer
r7 - original rax reg
r6 - original rdi reg
r8 - original rdx reg
r10 - used for function calls
    - addr of func put in r10, then blr [r10]
 */

// Verbose flag - 1 gets comments in the .s
// TODO - implement verbose
int V = 0;

// Define max args that are passed in regs
// More than these are passed on stack
// TODO - fix use of regs are aguments
#define GP_MAX 4
#define FP_MAX 8

static FILE *output_file;
static int depth;
/* Arrays to lookup regs used to pass arguments
   The arrays argreg8, argreg16, argreg32, and argreg64 are not used for Charm
   argreg64 is still in the code, but that code will be updated in the future
*/
static char *argreg64[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
static char *argregcharm[] = {"r0", "r1", "r2", "r3", "r5", "r6"}; // GUSTY
static Obj *current_fn;

static void gen_expr(Node *node);
static void gen_stmt(Node *node);

__attribute__((format(printf, 1, 2)))
static void println(char *fmt, ...) {
  //printf("in println\n");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(output_file, fmt, ap);
  //vprintf(fmt, ap);
  va_end(ap);
  fprintf(output_file, "\n");
  //printf("\n");
}

static int count(void) {
  static int i = 1;
  return i++;
}

static void push(void) {
  println("  str r7, [r13, #-4]! // push r7 on stack"); // GUSTY
  depth++;
}

static void pop(char *arg) {
  println("  ldr %s, [r13], #4   // pop top of stack into %s", arg, arg); // GUSTY
  depth--;
}

static void pushf(void) {
  println("  // push floating point - works for float, not double");
  println("  str r7, [r13, #-4]! // push r7 on stack"); // GUSTY
  depth++;
}

static void popf(int reg) {
  println("  // pop floating point - works for float, not double");
  println("  ldr r%d, [r13], #4   // pop top of stack into r%d", reg, reg); // GUSTY
  depth--;
}

// Round up `n` to the nearest multiple of `align`. For instance,
// align_to(5, 8) returns 8 and align_to(11, 8) returns 16.
int align_to(int n, int align) {
  return (n + align - 1) / align * align;
}

static char *reg_dx(int sz) {
  switch (sz) {
  case 1: return "%dl";
  case 2: return "%dx";
  case 4: return "%edx";
  case 8: return "%rdx";
  }
  unreachable();
}

static char *reg_ax(int sz) {
  switch (sz) {
  case 1: return "%al";
  case 2: return "%ax";
  case 4: return "%eax";
  case 8: return "%rax";
  }
  unreachable();
}

// Compute the absolute address of a given node.
// It's an error if a given node does not reside in memory.
static void gen_addr(Node *node) {
  switch (node->kind) {
  case ND_VAR:
    // Variable-length array, which is always local.
    if (node->var->ty->kind == TY_VLA) {
      println("  mov %d(%%rbp), %%rax", node->var->offset);
      return;
    }

    // Local variable
    if (node->var->is_local) {
      if (V) println("  // local variable %s, offset: %d", node->var->name, -node->var->offset); // GUSTY
      println("  add r7, r12, #%d     // addr of %s to r7", -node->var->offset, node->var->name); // GUSTY
      return;
    }

    if (opt_fpic) {
      // Thread-local variable
      if (node->var->is_tls) {
        println("  data16 lea %s@tlsgd(%%rip), %%rdi", node->var->name);
        println("  .value 0x6666");
        println("  rex64");
        println("  call __tls_get_addr@PLT");
        return;
      }

      // Function or global variable
      println("  mov %s@GOTPCREL(%%rip), %%rax", node->var->name);
      return;
    }

    // Thread-local variable
    if (node->var->is_tls) {
      println("  mov %%fs:0, %%rax");
      println("  add $%s@tpoff, %%rax", node->var->name);
      return;
    }

    // Here, we generate an absolute address of a function or a global
    // variable. Even though they exist at a certain address at runtime,
    // their addresses are not known at link-time for the following
    // two reasons.
    //
    //  - Address randomization: Executables are loaded to memory as a
    //    whole but it is not known what address they are loaded to.
    //    Therefore, at link-time, relative address in the same
    //    exectuable (i.e. the distance between two functions in the
    //    same executable) is known, but the absolute address is not
    //    known.
    //
    //  - Dynamic linking: Dynamic shared objects (DSOs) or .so files
    //    are loaded to memory alongside an executable at runtime and
    //    linked by the runtime loader in memory. We know nothing
    //    about addresses of global stuff that may be defined by DSOs
    //    until the runtime relocation is complete.
    //
    // In order to deal with the former case, we use RIP-relative
    // addressing, denoted by `(%rip)`. For the latter, we obtain an
    // address of a stuff that may be in a shared object file from the
    // Global Offset Table using `@GOTPCREL(%rip)` notation.

    // Function
    if (node->ty->kind == TY_FUNC) {
      if (node->var->is_definition)
        //println("  lea %s(%%rip), %%rax", node->var->name);
        println("  mva r7, %s   // addr of func %s", node->var->name, node->var->name);
      else {
        if (strcmp(node->var->name, "printf") == 0)
          println("  mva r7, 0x7000   // addr of func printf");
        else if (strcmp(node->var->name, "scanf") == 0)
          println("  mva r7, 0x7050   // addr of func scanf");
        else {
          println("  // external function %s\n", node->var->name);
          println("  mva r7, %s", node->var->name);
        }
      }
      return;
    }

    // Global variable
    println("  // global variable: %s", node->var->name); // GUSTY
    println("  mva r7, %s  // addr of %s to r7", node->var->name, node->var->name); // GUSTY
    return;
  case ND_DEREF:
    gen_expr(node->lhs);
    return;
  case ND_COMMA:
    gen_expr(node->lhs);
    gen_addr(node->rhs);
    return;
  case ND_MEMBER:
    gen_addr(node->lhs);
    println("  add r7, r7, #%d // add member offset", node->member->offset); // GUSTY
    return;
  case ND_FUNCALL:
    if (node->ret_buffer) {
      gen_expr(node);
      return;
    }
    break;
  case ND_ASSIGN:
  case ND_COND:
    if (node->ty->kind == TY_STRUCT || node->ty->kind == TY_UNION) {
      gen_expr(node);
      return;
    }
    break;
  case ND_VLA_PTR:
    println("  lea %d(%%rbp), %%rax", node->var->offset);
    return;
  }

  error_tok(node->tok, "not an lvalue");
}

// Load a value into r7 where r7 is pointing to. // GUSTY - only for size == 4
static void load(Type *ty) {
  switch (ty->kind) {
  case TY_ARRAY:
  case TY_STRUCT:
  case TY_UNION:
  case TY_FUNC:
  case TY_VLA:
    // If it is an array, do not attempt to load a value to the
    // register because in general we can't load an entire array to a
    // register. As a result, the result of an evaluation of an array
    // becomes not the array itself but the address of the array.
    // This is where "array is automatically converted to a pointer to
    // the first element of the array in C" occurs.
    return;
  case TY_FLOAT:
    println("  ldr r7, [r7] // float, good"); // GUSTY
    return;
  case TY_DOUBLE:
    println("  ldr r7, [r7] // double, like float"); // GUSTY
    return;
  case TY_LDOUBLE:
    println("  ldr r7, [r7] // long double, like float"); // GUSTY
    return;
  }

  // When we load a char or a short value to a register, we always
  // extend them to the size of int, so we can assume the lower half of
  // a register always contains a valid value. The upper half of a
  // register for char, short and int may contain garbage. When we load
  // a long value to a register, it simply occupies the entire register.
  if (ty->size == 1)
    println("  ldb r7, [r7] // char, size == 1"); // GUSTY
  else if (ty->size == 2)
    println("  ldr r7, [r7] // short, size == 2, load 4 bytes"); // ty->size == 2, this loads 4 bytes
  else if (ty->size == 4)
    println("  ldr r7, [r7] // int, size == 4"); // GUSTY
  else
    println("  ldr r7, [r7]  // long, size == 8 "); // GUSTY
}

// Store %rax to an address that the stack top is pointing to.
// Store r7 to an address that the stack top is pointing to.
// rax is now r7, rdi is now r6
// I put address of local variable in r7
// Use [r6] to store r7 to memory address of variable
static void store(Type *ty) {
  pop("r6"); // pop address from stack into r6

  switch (ty->kind) {
  case TY_STRUCT:
  case TY_UNION:
    for (int i = 0; i < ty->size; i++) {
      println("  stb r7, [r6], 1  // struct stores a byte at a time");
    }
    return;
  case TY_FLOAT:
    println("  str r7, [r6]");
    return;
  case TY_DOUBLE:
    println("  str r7, [r6]");
    return;
  case TY_LDOUBLE:
    println("  str r7, [r6]");
    return;
  }


  if (ty->size == 1)
    println("  stb r7, [r6]"); // GUSTY
  else if (ty->size == 2)
    println("  str r7, [r6]"); // GUSTY
  else if (ty->size == 4)
    println("  str r7, [r6]"); // GUSTY
  else
    println("  str r7, [r6]"); // GUSTY
}

static void cmp_zero(Type *ty) {
  switch (ty->kind) {
  case TY_FLOAT:
  case TY_DOUBLE:
  case TY_LDOUBLE:
    println("  mov r6, #0 // 0 is float 0.0");
    println("  cmf r7, r6");
    return;
  }

  if (is_integer(ty) && ty->size <= 4)
    println("  cmp r7, #0");
  else
    println("  cmp r7, #0");
}

// The following enum is ordered to match indices in cast_table
enum { I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, F80 };

// Return one of the enum values defined above
static int getTypeId(Type *ty) {
  switch (ty->kind) {
  case TY_CHAR:
    return ty->is_unsigned ? U8 : I8;
  case TY_SHORT:
    return ty->is_unsigned ? U16 : I16;
  case TY_INT:
    return ty->is_unsigned ? U32 : I32;
  case TY_LONG:
    return ty->is_unsigned ? U64 : I64;
  case TY_FLOAT:
    return F32;
  case TY_DOUBLE:
    return F64;
  case TY_LDOUBLE:
    return F80;
  }
  return U64;
}

/* 
The table for type casts - used to convert one primitive type to another. Samples
int to float - This is i32f32, which is a valid conversion in Charm
float to int - This is f32i32, which is a valid conversion in Charm

Charm supports the types as defined in the first comment of this file.

The value to be converted is in r7
 */
static char i32i8[] = "movsbl %al, %eax";
static char i32u8[] = "movzbl %al, %eax";
static char i32i16[] = "movswl %ax, %eax";
static char i32u16[] = "movzwl %ax, %eax";
static char i32f32[] = "itf r7, r7 // convert int to float";
static char i32i64[] = "// convert int to double";
static char i32f64[] = "itf r7, r7 // convert int to double (float for Charm)";
static char i32f80[] = "itf r7, r7 // convert int to long double (float for Charm)";

static char u32f32[] = "itf r7, r7 // convert uint to float - not correct";
static char u32i64[] = "// convert uint to double - not correct";
static char u32f64[] = "itf r7, r7 // convert uint to double (float for Charm) - not correct";
static char u32f80[] = "itf r7, r7 // convert uint to long double (float for Charm) - not correct";

static char i64f32[] = "itf r7, r7 // convert long to float";
static char i64f64[] = "itf r7, r7 // convert long to double";
static char i64f80[] = "itf r7, r7 // convert long to long double";

static char u64f32[] = "itf r7, r7 // convert ulong to float";
static char u64f64[] = "itf r7, r7 // convert ulong to double";
static char u64f80[] = "itf r7, r7 // convert ulong to long double";
static char f32i8[] = "fti r7, r7 // convert float to char"
                       "\n  mov r6, #0xff"
                       "\n  and r7, r7, r6";
static char f32u8[] = "fti r7, r7 // convert float to uchar"
                       "\n  mov r6, #0xff"
                       "\n  and r7, r7, r6";
static char f32i16[] = "fti r7, r7 // convert float to short";
static char f32u16[] = "fti r7, r7 // convert float to ushort";
static char f32i32[] = "fti r7, r7 // convert float to int";
static char f32u32[] = "fti r7, r7 // convert float to uint";
static char f32i64[] = "fti r7, r7 // convert float to long";
static char f32u64[] = "fti r7, r7 // convert float to ulong";
static char f32f64[] = "// convert float to double";
static char f32f80[] = "// convert float to long double";

static char f64i8[] = "fti r7, r7 // convert double to char"
                       "\n  mov r6, #0xff"
                       "\n  and r7, r7, r6";
static char f64u8[] = "fti r7, r7 // convert double to uchar"
                       "\n  mov r6, #0xff"
                       "\n  and r7, r7, r6";
static char f64i16[] = "fti r7, r7 // convert double to short";
static char f64u16[] = "fti r7, r7 // convert double to ushort";
static char f64i32[] = "fti r7, r7 // convert double to int";
static char f64u32[] = "fti r7, r7 // convert double to uint";
static char f64i64[] = "fti r7, r7 // convert double to long";
static char f64u64[] = "fti r7, r7 // convert double to ulong";
static char f64f32[] = "// convert double to float";
static char f64f80[] = "// convert double to long double";

static char f80i8[] = "// convert long double to char";
static char f80u8[] = "// convert long double to uchar";
static char f80i16[] = "// convert long double to short";
static char f80u16[] = "// convert long double to ushort";
static char f80i32[] = "// convert long double to int";
static char f80u32[] = "// convert long double to uint";
static char f80i64[] = "// convert long double to long";
static char f80u64[] = "// convert long double to ulong";
static char f80f32[] = "// convert long double to float";
static char f80f64[] = "// convert long double to double";

static char *cast_table[][11] = {
  // i8   i16     i32     i64     u8     u16     u32     u64     f32     f64     f80
  {NULL,  NULL,   NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i8
  {i32i8, NULL,   NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i16
  {i32i8, i32i16, NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i32
  {i32i8, i32i16, NULL,   NULL,   i32u8, i32u16, NULL,   NULL,   i64f32, i64f64, i64f80}, // i64

  {i32i8, NULL,   NULL,   i32i64, NULL,  NULL,   NULL,   i32i64, i32f32, i32f64, i32f80}, // u8
  {i32i8, i32i16, NULL,   i32i64, i32u8, NULL,   NULL,   i32i64, i32f32, i32f64, i32f80}, // u16
  {i32i8, i32i16, NULL,   u32i64, i32u8, i32u16, NULL,   u32i64, u32f32, u32f64, u32f80}, // u32
  {i32i8, i32i16, NULL,   NULL,   i32u8, i32u16, NULL,   NULL,   u64f32, u64f64, u64f80}, // u64

  {f32i8, f32i16, f32i32, f32i64, f32u8, f32u16, f32u32, f32u64, NULL,   f32f64, f32f80}, // f32
  {f64i8, f64i16, f64i32, f64i64, f64u8, f64u16, f64u32, f64u64, f64f32, NULL,   f64f80}, // f64
  {f80i8, f80i16, f80i32, f80i64, f80u8, f80u16, f80u32, f80u64, f80f32, f80f64, NULL},   // f80
};

static void cast(Type *from, Type *to) {
  if (to->kind == TY_VOID)
    return;

  if (to->kind == TY_BOOL) {
    cmp_zero(from);
    println("  setne %%al");
    println("  movzx %%al, %%eax");
    return;
  }

  int t1 = getTypeId(from);
  int t2 = getTypeId(to);
  //printf("GUSTY: cast(%d, %d)\n", t1, t2);
  if (cast_table[t1][t2])
    println("  %s", cast_table[t1][t2]);
}

// Structs or unions equal or smaller than 16 bytes are passed
// using up to two registers.
//
// If the first 8 bytes contains only floating-point type members,
// they are passed in an XMM register. Otherwise, they are passed
// in a general-purpose register.
//
// If a struct/union is larger than 8 bytes, the same rule is
// applied to the the next 8 byte chunk.
//
// This function returns true if `ty` has only floating-point
// members in its byte range [lo, hi).
static bool has_flonum(Type *ty, int lo, int hi, int offset) {
  if (ty->kind == TY_STRUCT || ty->kind == TY_UNION) {
    for (Member *mem = ty->members; mem; mem = mem->next)
      if (!has_flonum(mem->ty, lo, hi, offset + mem->offset))
        return false;
    return true;
  }

  if (ty->kind == TY_ARRAY) {
    for (int i = 0; i < ty->array_len; i++)
      if (!has_flonum(ty->base, lo, hi, offset + ty->base->size * i))
        return false;
    return true;
  }

  return offset < lo || hi <= offset || ty->kind == TY_FLOAT || ty->kind == TY_DOUBLE;
}

static bool has_flonum1(Type *ty) {
  return has_flonum(ty, 0, 8, 0);
}

static bool has_flonum2(Type *ty) {
  return has_flonum(ty, 8, 16, 0);
}

static void push_struct(Type *ty) {
  int sz = align_to(ty->size, 8);
  println("  sub $%d, %%rsp", sz);
  depth += sz / 8;

  for (int i = 0; i < ty->size; i++) {
    println("  mov %d(%%rax), %%r10b", i);
    println("  mov %%r10b, %d(%%rsp)", i);
  }
}

static void push_args2(Node *args, bool first_pass) {
  if (!args)
    return;
  push_args2(args->next, first_pass);

  if ((first_pass && !args->pass_by_stack) || (!first_pass && args->pass_by_stack))
    return;

  gen_expr(args);

  switch (args->ty->kind) {
  case TY_STRUCT:
  case TY_UNION:
    push_struct(args->ty);
    break;
  case TY_FLOAT:
  case TY_DOUBLE:
    pushf();
    break;
  case TY_LDOUBLE:
    println("  sub $16, %%rsp");
    println("  fstpt (%%rsp)");
    depth += 2;
    break;
  default:
    push();
  }
}

// Load function call arguments. Arguments are already evaluated and
// stored to the stack as local variables. What we need to do in this
// function is to load them to registers or push them to the stack as
// specified by the x86-64 psABI. Here is what the spec says:
//
// - Up to 6 arguments of integral type are passed using RDI, RSI,
//   RDX, RCX, R8 and R9.
//
// - Up to 8 arguments of floating-point type are passed using XMM0 to
//   XMM7.
//
// - If all registers of an appropriate type are already used, push an
//   argument to the stack in the right-to-left order.
//
// - Each argument passed on the stack takes 8 bytes, and the end of
//   the argument area must be aligned to a 16 byte boundary.
//
// - If a function is variadic, set the number of floating-point type
//   arguments to RAX.
static int push_args(Node *node) {
  int stack = 0, gp = 0, fp = 0;

  // If the return type is a large struct/union, the caller passes
  // a pointer to a buffer as if it were the first argument.
  if (node->ret_buffer && node->ty->size > 16)
    gp++;

  // Load as many arguments to the registers as possible.
  for (Node *arg = node->args; arg; arg = arg->next) {
    Type *ty = arg->ty;

    switch (ty->kind) {
    case TY_STRUCT:
    case TY_UNION:
      if (ty->size > 16) {
        arg->pass_by_stack = true;
        stack += align_to(ty->size, 8) / 8;
      } else {
        bool fp1 = has_flonum1(ty);
        bool fp2 = has_flonum2(ty);

        if (fp + fp1 + fp2 < FP_MAX && gp + !fp1 + !fp2 < GP_MAX) {
          fp = fp + fp1 + fp2;
          gp = gp + !fp1 + !fp2;
        } else {
          arg->pass_by_stack = true;
          stack += align_to(ty->size, 8) / 8;
        }
      }
      break;
    case TY_FLOAT:
    case TY_DOUBLE:
      if (fp++ >= FP_MAX) {
        arg->pass_by_stack = true;
        stack++;
      }
      break;
    case TY_LDOUBLE:
      arg->pass_by_stack = true;
      stack += 2;
      break;
    default:
      if (gp++ >= GP_MAX) {
        arg->pass_by_stack = true;
        stack++;
      }
    }
    //printf("GUSTY - push_args: %d\n", stack);
  }

  if ((depth + stack) % 2 == 1) {
    println("  // orig code adjusted stack"); // GUSTY
    //println("  sub $8, %%rsp");
    depth++;
    stack++;
  }

  push_args2(node->args, true);
  push_args2(node->args, false);

  // If the return type is a large struct/union, the caller passes
  // a pointer to a buffer as if it were the first argument.
  if (node->ret_buffer && node->ty->size > 16) {
    println("  lea %d(%%rbp), %%rax", node->ret_buffer->offset);
    push();
  }

  return stack;
}

static void copy_ret_buffer(Obj *var) {
  Type *ty = var->ty;
  int gp = 0, fp = 0;

  if (has_flonum1(ty)) {
    assert(ty->size == 4 || 8 <= ty->size);
    if (ty->size == 4)
      println("  movss %%xmm0, %d(%%rbp)", var->offset);
    else
      println("  movsd %%xmm0, %d(%%rbp)", var->offset);
    fp++;
  } else {
    for (int i = 0; i < MIN(8, ty->size); i++) {
      println("  mov %%al, %d(%%rbp)", var->offset + i);
      println("  shr $8, %%rax");
    }
    gp++;
  }

  if (ty->size > 8) {
    if (has_flonum2(ty)) {
      assert(ty->size == 12 || ty->size == 16);
      if (ty->size == 12)
        println("  movss %%xmm%d, %d(%%rbp)", fp, var->offset + 8);
      else
        println("  movsd %%xmm%d, %d(%%rbp)", fp, var->offset + 8);
    } else {
      char *reg1 = (gp == 0) ? "%al" : "%dl";
      char *reg2 = (gp == 0) ? "%rax" : "%rdx";
      for (int i = 8; i < MIN(16, ty->size); i++) {
        println("  mov %s, %d(%%rbp)", reg1, var->offset + i);
        println("  shr $8, %s", reg2);
      }
    }
  }
}

static void copy_struct_reg(void) {
  Type *ty = current_fn->ty->return_ty;
  int gp = 0, fp = 0;

  println("  mov %%rax, %%rdi");

  if (has_flonum(ty, 0, 8, 0)) {
    assert(ty->size == 4 || 8 <= ty->size);
    if (ty->size == 4)
      println("  movss (%%rdi), %%xmm0");
    else
      println("  movsd (%%rdi), %%xmm0");
    fp++;
  } else {
    println("  mov $0, %%rax");
    for (int i = MIN(8, ty->size) - 1; i >= 0; i--) {
      println("  shl $8, %%rax");
      println("  mov %d(%%rdi), %%al", i);
    }
    gp++;
  }

  if (ty->size > 8) {
    if (has_flonum(ty, 8, 16, 0)) {
      assert(ty->size == 12 || ty->size == 16);
      if (ty->size == 4)
        println("  movss 8(%%rdi), %%xmm%d", fp);
      else
        println("  movsd 8(%%rdi), %%xmm%d", fp);
    } else {
      char *reg1 = (gp == 0) ? "%al" : "%dl";
      char *reg2 = (gp == 0) ? "%rax" : "%rdx";
      println("  mov $0, %s", reg2);
      for (int i = MIN(16, ty->size) - 1; i >= 8; i--) {
        println("  shl $8, %s", reg2);
        println("  mov %d(%%rdi), %s", i, reg1);
      }
    }
  }
}

static void copy_struct_mem(void) {
  Type *ty = current_fn->ty->return_ty;
  Obj *var = current_fn->params;

  println("  mov %d(%%rbp), %%rdi", var->offset);

  for (int i = 0; i < ty->size; i++) {
    println("  mov %d(%%rax), %%dl", i);
    println("  mov %%dl, %d(%%rdi)", i);
  }
}

static void builtin_alloca(void) {
  // Align size to 16 bytes.
  println("  add $15, %%rdi");
  println("  and $0xfffffff0, %%edi");

  // Shift the temporary area by %rdi.
  println("  mov %d(%%rbp), %%rcx", current_fn->alloca_bottom->offset);
  println("  sub %%rsp, %%rcx");
  println("  mov %%rsp, %%rax");
  println("  sub %%rdi, %%rsp");
  println("  mov %%rsp, %%rdx");
  println("1:");
  println("  cmp $0, %%rcx");
  println("  je 2f");
  println("  mov (%%rax), %%r8b");
  println("  mov %%r8b, (%%rdx)");
  println("  inc %%rdx");
  println("  inc %%rax");
  println("  dec %%rcx");
  println("  jmp 1b");
  println("2:");

  // Move alloca_bottom pointer.
  println("  mov %d(%%rbp), %%rax", current_fn->alloca_bottom->offset);
  println("  sub %%rdi, %%rax");
  println("  mov %%rax, %d(%%rbp)", current_fn->alloca_bottom->offset);
}

// Generate code for a given node.
static void gen_expr(Node *node) {
  //println("  .loc %d %d", node->tok->file->file_no, node->tok->line_no);

  switch (node->kind) {
  case ND_NULL_EXPR:
    return;
  case ND_NUM: {
    switch (node->ty->kind) {
    case TY_FLOAT: 
    case TY_DOUBLE: 
    case TY_LDOUBLE: {
      if (V) {
        if (node->ty->kind == TY_DOUBLE) {
          println("  // Charm does not support double");
          println("  // double is treated as a float");
        }
        if (node->ty->kind == TY_LDOUBLE) {
          println("  // Charm does not support long double");
          println("  // long double is treated as a float");
        }
      }
      union { float f32; uint32_t u32; } u = { node->fval };
      int upper = u.u32 >> 16;
      int lower = u.u32 & 0xffff;
      println("  // 0x%x is float %Lf", u.u32, node->fval);
      println("  mov r7, #0x%x  // upper of float %Lf", upper, node->fval);
      println("  shf r7, #16");
      println("  mov r6, #0x%x  // lower of float %Lf", lower, node->fval);
      println("  orr r7, r7, r6  // put upper and lower together");
      return;
    }
    }

    if (V) println("  // number: %lld", node->val); // GUSTY
    println("  mov r7, #%lld", node->val); // GUSTY
    return;
  }
  case ND_NEG:
    gen_expr(node->lhs);

    switch (node->ty->kind) {
    case TY_FLOAT:
    case TY_DOUBLE:
    case TY_LDOUBLE:
      if (V) {
        if (node->ty->kind == TY_DOUBLE) {
          println("  // Charm does not support double");
          println("  // double is treated as a float");
        }
        if (node->ty->kind == TY_LDOUBLE) {
          println("  // Charm does not support long double");
          println("  // long double is treated as a float");
        }
      }
      println("  mov r6, #0");
      println("  suf r7, r6, r7 // 0 - r7 to negate r7");
      return;
    }

    println("  mov r6, #0");
    println("  sub r7, r6, r7 // 0 - r7 to negate r7");
    //println("  one r7, r7"); // Do I want a neg instruction
    //println("  add r7, r7, 1"); // Do I want a neg instruction
    return;
  case ND_VAR:
    gen_addr(node);
    load(node->ty);
    return;
  case ND_MEMBER: {
    gen_addr(node);
    load(node->ty);

    Member *mem = node->member;
    if (mem->is_bitfield) {
      println("  shl $%d, %%rax", 64 - mem->bit_width - mem->bit_offset);
      if (mem->ty->is_unsigned)
        println("  shr $%d, %%rax", 64 - mem->bit_width);
      else
        println("  sar $%d, %%rax", 64 - mem->bit_width);
    }
    return;
  }
  case ND_DEREF:
    gen_expr(node->lhs);
    load(node->ty);
    return;
  case ND_ADDR:
    gen_addr(node->lhs);
    return;
  case ND_ASSIGN:
    gen_addr(node->lhs);
    push();
    gen_expr(node->rhs);

    if (node->lhs->kind == ND_MEMBER && node->lhs->member->is_bitfield) {
      println("  mov %%rax, %%r8");

      // If the lhs is a bitfield, we need to read the current value
      // from memory and merge it with a new value.
      Member *mem = node->lhs->member;
      println("  mov %%rax, %%rdi");
      println("  and $%ld, %%rdi", (1L << mem->bit_width) - 1);
      println("  shl $%d, %%rdi", mem->bit_offset);

      println("  mov (%%rsp), %%rax");
      load(mem->ty);

      long mask = ((1L << mem->bit_width) - 1) << mem->bit_offset;
      println("  mov $%ld, %%r9", ~mask);
      println("  and %%r9, %%rax");
      println("  or %%rdi, %%rax");
      store(node->ty);
      println("  mov %%r8, %%rax");
      return;
    }

    store(node->ty);
    return;
  case ND_STMT_EXPR:
    for (Node *n = node->body; n; n = n->next)
      gen_stmt(n);
    return;
  case ND_COMMA:
    gen_expr(node->lhs);
    gen_expr(node->rhs);
    return;
  case ND_CAST:
    gen_expr(node->lhs);
    cast(node->lhs->ty, node->ty);
    return;
  case ND_MEMZERO:
    // 'rep stosb' is equivalent to 'memset(%rdi, %al, %rcx)'.
    // My code does not consider node->var->ty->size.
    // My code zeros 4 bytes of memory on stack
    //println("  mov $%d, %%rcx", node->var->ty->size);
    // TODO - consider node->var->ty->size
    println("  mov r6, #0");
    println("  str r6, [r13, #%d]   // %s", -node->var->offset, node->var->name);
    return;
  case ND_COND: {
    int c = count();
    gen_expr(node->cond);
    cmp_zero(node->cond->ty);
    println("  beq LL_else_%d", c);
    gen_expr(node->then);
    println("  bal LL_end_%d", c);
    println(".local LL_else_%d", c);
    gen_expr(node->els);
    println(".local LL_end_%d", c);
    return;
  }
  case ND_NOT: {
    int c = count();
    gen_expr(node->lhs);
    cmp_zero(node->lhs->ty);
    println("  beq LL_false_%d", c);
    println("  mov r7, #1");
    println("  bal LL_end_%d", c);
    println(".local LL_false_%d", c);
    println("  mov r7, #0");
    println(".local LL_end_%d", c);
    return;
  }
  case ND_BITNOT:
    gen_expr(node->lhs);
    println("  mov r6, #0");
    println("  sub r7, r6, r7 // 0 - r7 to negate r7");
    println("  sub r7, r7, 1  // create one's complement");
    //println("  one r7, r7"); // Do I want a one instruction for one's complement
    return;
  case ND_LOGAND: {
    int c = count();
    gen_expr(node->lhs);
    cmp_zero(node->lhs->ty);
    println("  beq LL_false_%d", c);
    gen_expr(node->rhs);
    cmp_zero(node->rhs->ty);
    println("  beq LL_false_%d", c);
    println("  mov r7, #1");
    println("  bal LL_end_%d", c);
    println(".local LL_false_%d", c);
    println("  mov r7, #0");
    println(".local LL_end_%d", c);
    return;
  }
  case ND_LOGOR: {
    int c = count();
    gen_expr(node->lhs);
    cmp_zero(node->lhs->ty);
    println("  bne LL_true_%d", c);
    gen_expr(node->rhs);
    cmp_zero(node->rhs->ty);
    println("  bne LL_true_%d", c);
    println("  mov r7, #0");
    println("  bal LL_end_%d", c);
    println(".local LL_true_%d", c);
    println("  mov r7, #1");
    println(".local LL_end_%d", c);
    return;
  }
/* TODO
Implement function calls with large return types - like return a structure
Must pass a pointer to a buffer to receive the large return type
   TODO
Fix the use of fp regs. There are no differences in Charm.
 */
  case ND_FUNCALL: {
    if (node->lhs->kind == ND_VAR && !strcmp(node->lhs->var->name, "alloca")) {
      gen_expr(node->args);
      println("  mov %%rax, %%rdi");
      builtin_alloca();
      return;
    }

    int stack_args = push_args(node);
    gen_expr(node->lhs);

    int gp = 0, fp = 0;

    // If the return type is a large struct/union, the caller passes
    // a pointer to a buffer as if it were the first argument.
    if (node->ret_buffer && node->ty->size > 16)
      pop(argreg64[gp++]);

    for (Node *arg = node->args; arg; arg = arg->next) {
      Type *ty = arg->ty;

      switch (ty->kind) {
      case TY_STRUCT:
      case TY_UNION:
        if (ty->size > 16)
          continue;

        bool fp1 = has_flonum1(ty);
        bool fp2 = has_flonum2(ty);

        if (fp + fp1 + fp2 < FP_MAX && gp + !fp1 + !fp2 < GP_MAX) {
          if (fp1)
            popf(fp++);
          else
            pop(argreg64[gp++]);

          if (ty->size > 8) {
            if (fp2)
              popf(fp++);
            else
              pop(argreg64[gp++]);
          }
        }
        break;
      case TY_FLOAT:
      case TY_DOUBLE:
        if (fp < FP_MAX)
          popf(gp++);  // GUSTY - This is a temporary fix
          //popf(fp++);
        break;
      case TY_LDOUBLE:
        break;
      default:
        if (gp < GP_MAX)
          pop(argregcharm[gp++]); // GUSTY
          //pop(argreg64[gp++]);
      }
    }

    // GUSTY
    println("  mov r10, r7");
    println("  mov r7, #%d", fp);
    println("  blr [r10]");
    // see original code - println("  add r13, r13, #%d // add space for args pushed onto stack", stack_args * 8);
    println("  mov r7, r0 // mov return value to r7 for assignment");

    depth -= stack_args;

    // For Intel, the most significant 48 or 56 bits in RAX may
    // contain garbage if a function return type is short or bool/char,
    // respectively. Original code cleared these bits. 
    // Removed for Charm

    // If the return type is a small struct, a value is returned
    // using up to two registers.
    if (node->ret_buffer && node->ty->size <= 16) {
      copy_ret_buffer(node->ret_buffer);
      println("  lea %d(%%rbp), %%rax", node->ret_buffer->offset);
    }

    return;
  }
  case ND_LABEL_VAL:
    println("  mva r7, %s // *p = &&label", node->unique_label);
    return;
/*
C11 defines atomic types and functions.
#include <stdatomic.h>

atomic_int ai;

++ai generates code that is thread safe <-- I think
TODO - Study C atomics (like semaphores) and update the following two cases
 */
  // CAS - Atomic Compare and Swap
  case ND_CAS: {
    gen_expr(node->cas_addr);
    push();
    gen_expr(node->cas_new);
    push();
    gen_expr(node->cas_old);
    println("  mov %%rax, %%r8");
    load(node->cas_old->ty->base);
    pop("%rdx"); // new
    pop("%rdi"); // addr

    int sz = node->cas_addr->ty->base->size;
    println("  lock cmpxchg %s, (%%rdi)", reg_dx(sz));
    println("  sete %%cl");
    println("  je 1f");
    println("  mov %s, (%%r8)", reg_ax(sz));
    println("1:");
    println("  movzbl %%cl, %%eax");
    return;
  }
  // EXCH - Atomic Exchange
  case ND_EXCH: {
    gen_expr(node->lhs);
    push();
    gen_expr(node->rhs);
    pop("%rdi");

    int sz = node->lhs->ty->base->size;
    println("  xchg %s, (%%rdi)", reg_ax(sz));
    return;
  }
  }

  switch (node->lhs->ty->kind) {
  case TY_FLOAT:
  case TY_DOUBLE: 
  case TY_LDOUBLE: {
    if (node->lhs->ty->kind == TY_LDOUBLE) {
      println("  // Charm does not support long double");
      println("  // Generate same code as float and double");
    }
    gen_expr(node->rhs);
    pushf();
    gen_expr(node->lhs);
    popf(6);
    switch (node->kind) {
    case ND_ADD:
      println("  adf r7, r7, r6");
      return;
    case ND_SUB:
      println("  suf r7, r7, r6");
      return;
    case ND_MUL:
      println("  muf r7, r7, r6");
      return;
    case ND_DIV:
      println("  dif r7, r7, r6");
      return;
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
      // GUSTY
      println("  cmf r7, r6");
      println("  mov r7, #1");
      int c = count();

      if (node->kind == ND_EQ) {
        println("  beq LL_cond_%d", c);
      } else if (node->kind == ND_NE) {
        println("  bne LL_cond_%d", c);
      } else if (node->kind == ND_LT) {
        println("  blt LL_cond_%d", c);
      } else if (node->kind == ND_LE) {
        println("  ble LL_cond_%d", c);
      }
      println("  mov r7, #0");
      println(".local LL_cond_%d", c);
      return;
    }

    error_tok(node->tok, "invalid expression");
  }

  }

  // This code is for integer expressions?

  gen_expr(node->rhs);
  push();
  gen_expr(node->lhs);
  pop("r6"); // GUSTY pop address of lhs into r6

/*
   The x86 regs mapped to Charm regs
   ax -> r7
   di -> r6
   dx -> r8
 */
  char *ax, *di, *dx;

  if (node->lhs->ty->kind == TY_LONG || node->lhs->ty->base) {
    ax = "r7";
    di = "r6";
    dx = "r8";
  } else {
    ax = "r7";
    di = "r6";
    dx = "r8";
  }

  switch (node->kind) {
  case ND_ADD:
    println("  add %s, %s, %s", ax, di, ax); // GUSTY
    return;
  case ND_SUB:
    println("  sub %s, %s, %s", ax, ax, di); // GUSTY
    return;
  case ND_MUL:
    println("  mul %s, %s, %s", ax, di, ax); // GUSTY
    return;
  case ND_DIV:
    println("  div %s, %s, %s", ax, ax, di); // GUSTY
    return;
  case ND_MOD:
    // does an unsigned value make any difference, if (node->ty->is_unsigned) {
    println("  mod %s, %s, %s", ax, ax, di); // GUSTY
    return;
  case ND_BITAND:
    println("  and %s, %s, %s", ax, di, ax); // GUSTY
    return;
  case ND_BITOR:
    println("  orr %s, %s, %s", ax, di, ax); // GUSTY
    return;
  case ND_BITXOR:
    println("  eor %s, %s, %s", ax, di, ax); // GUSTY
    return;
  case ND_EQ:
  case ND_NE:
  case ND_LT:
  case ND_LE:
    // GUSTY
    println("  cmp %s, %s", ax, di);
    println("  mov r7, #1");
    int c = count();

    if (node->kind == ND_EQ) {
      println("  beq LL_cond_%d", c);
    } else if (node->kind == ND_NE) {
      println("  bne LL_cond_%d", c);
    } else if (node->kind == ND_LT) {
      if (node->lhs->ty->is_unsigned)
        println("  blt LL_cond_%d", c);
      else
        println("  blt LL_cond_%d", c);
    } else if (node->kind == ND_LE) {
      if (node->lhs->ty->is_unsigned)
        println("  ble LL_cond_%d", c);
      else
        println("  ble LL_cond_%d", c);
    }
    println("  mov r7, #0");
    println(".local LL_cond_%d", c);

    return;
  case ND_SHL:
    println("  shf %s, %s", ax, di);
    return;
  case ND_SHR:
    println("  one %s, %s // shift right - negative shift value", di, di);
    println("  add %s, %s, 1", di, di);
    println("  shf %s, %s", ax, di);
    return;
  }

  error_tok(node->tok, "invalid expression");
}

static void gen_stmt(Node *node) {
  //println("  .loc %d %d", node->tok->file->file_no, node->tok->line_no);

  switch (node->kind) {
  case ND_IF: {
    int c = count();
    gen_expr(node->cond);
    cmp_zero(node->cond->ty);
    println("  beq LL_else_%d", c);
    gen_stmt(node->then);
    println("  bal LL_end_%d", c);
    println(".local LL_else_%d", c);
    if (node->els)
      gen_stmt(node->els);
    println(".local LL_end_%d", c);
    return;
  }
  case ND_FOR: {
    int c = count();
    if (node->init)
      gen_stmt(node->init);
    println(".local LL_begin_%d", c);
    if (node->cond) {
      gen_expr(node->cond);
      cmp_zero(node->cond->ty);
      println("  beq %s", node->brk_label);
    }
    gen_stmt(node->then);
    println(".local %s", node->cont_label);
    if (node->inc)
      gen_expr(node->inc);
    println("  bal LL_begin_%d", c);
    println(".local %s", node->brk_label);
    return;
  }
  case ND_DO: {
    int c = count();
    println(".local LL_begin_%d", c);
    gen_stmt(node->then);
    println(".local %s", node->cont_label);
    gen_expr(node->cond);
    cmp_zero(node->cond->ty);
    println("  jne LL_begin_%d", c);
    println(".local %s", node->brk_label);
    return;
  }
  case ND_SWITCH:
    gen_expr(node->cond);

    for (Node *n = node->case_next; n; n = n->case_next) {
      //char *ax = (node->cond->ty->size == 8) ? "%rax" : "%eax";
      //char *di = (node->cond->ty->size == 8) ? "%rdi" : "%edi";
      char *ax = "r7";
      char *di = "r6";

      if (n->begin == n->end) {
        println("  cmp %s, %ld", ax, n->begin);
        println("  beq %s", n->label);
        continue;
      }

      // [GNU] Case ranges HERE - SWITCH
      println("  mov %s, %s", ax, di);
      println("  sub $%ld, %s", n->begin, di);
      println("  cmp $%ld, %s", n->end - n->begin, di);
      println("  jbe %s", n->label); // HERE what is jbe
    }

    if (node->default_case)
      println("  bal %s", node->default_case->label);

    println("  bal %s", node->brk_label);
    gen_stmt(node->then);
    println(".local %s", node->brk_label);
    return;
  case ND_CASE:
    println(".local %s", node->label);
    gen_stmt(node->lhs);
    return;
  case ND_BLOCK:
    for (Node *n = node->body; n; n = n->next)
      gen_stmt(n);
    return;
  case ND_GOTO:
    println("  bal %s", node->unique_label);
    return;
  case ND_GOTO_EXPR:
    gen_expr(node->lhs);
    println("  bal *%%rax");
    return;
  case ND_LABEL:
    println(".label %s", node->unique_label);
    gen_stmt(node->lhs);
    return;
/* TODO
Implement the return of stuct / union.
This is the functions copy_struct_reg and copy_struct_mem - see above
 */
  case ND_RETURN:
    if (node->lhs) {
      gen_expr(node->lhs);
      Type *ty = node->lhs->ty;

      switch (ty->kind) {
      case TY_STRUCT:
      case TY_UNION:
        if (ty->size <= 16)
          copy_struct_reg();
        else
          copy_struct_mem();
        break;
      }
    }

    println("  mov r0, r7          // return value in r0"); // GUSTY
    println("  bal LL_return_%s", current_fn->name);
    return;
  case ND_EXPR_STMT:
    gen_expr(node->lhs);
    return;
  case ND_ASM:
    println("  %s", node->asm_str);
    return;
  }

  error_tok(node->tok, "invalid statement");
}

// Assign offsets to local variables.
static void assign_lvar_offsets(Obj *prog) {
  for (Obj *fn = prog; fn; fn = fn->next) {
    if (!fn->is_function)
      continue;

    // If a function has many parameters, some parameters are
    // inevitably passed by stack rather than by register.
    // The first passed-by-stack parameter resides at RBP+16.
    int top = 16;
    int bottom = 0;

    int gp = 0, fp = 0;

    // Assign offsets to pass-by-stack parameters.
    for (Obj *var = fn->params; var; var = var->next) {
      Type *ty = var->ty;

      switch (ty->kind) {
      case TY_STRUCT:
      case TY_UNION:
        if (ty->size <= 16) {
          bool fp1 = has_flonum(ty, 0, 8, 0);
          bool fp2 = has_flonum(ty, 8, 16, 8);
          if (fp + fp1 + fp2 < FP_MAX && gp + !fp1 + !fp2 < GP_MAX) {
            fp = fp + fp1 + fp2;
            gp = gp + !fp1 + !fp2;
            continue;
          }
        }
        break;
      case TY_FLOAT:
      case TY_DOUBLE:
        if (fp++ < FP_MAX)
          continue;
        break;
      case TY_LDOUBLE:
        break;
      default:
        if (gp++ < GP_MAX)
          continue;
      }

      top = align_to(top, 8);
      var->offset = top;
      top += var->ty->size;
    }

    // Assign offsets to pass-by-register parameters and local variables.
    for (Obj *var = fn->locals; var; var = var->next) {
      if (var->offset)
        continue;

      // AMD64 System V ABI has a special alignment rule for an array of
      // length at least 16 bytes. We need to align such array to at least
      // 16-byte boundaries. See p.14 of
      // https://github.com/hjl-tools/x86-psABI/wiki/x86-64-psABI-draft.pdf.
      int align = (var->ty->kind == TY_ARRAY && var->ty->size >= 16)
        ? MAX(16, var->align) : var->align;

      if (strcmp(var->name, "__va_area__") == 0) // GUSTY
        continue;
      bottom += var->ty->size;
      //printf("GUSTY var->name: %s, var->ty->size: %d\n", var->name, var->ty->size);
      bottom = align_to(bottom, align);
      //printf("GUSTY assign Bottom: %d\n", bottom);
      var->offset = -bottom;
    }

    //printf("GUSTY assign bottom: %d\n", bottom);
    fn->stack_size = align_to(bottom, 16);
    //printf("GUSTY assign local variables : fn->stack_size: %d\n", fn->stack_size);
  }
}

static void emit_data(Obj *prog) {
  if (V) println("// r13 (sp) value");
  println(".stack 0x5000");
  if (V) println("// data section");
  println(".data 0x100");
  for (Obj *var = prog; var; var = var->next) {
    //println("Gusty: var->name: %s, var->is_definition: %d, var->is_function: %d\n", var->name, var->is_definition, var->is_function);
    // Original code was the following two lines
    //if (var->is_function || !var->is_definition)
    //  continue;

    // I want to emit .extern statements for external functions and variables
    if (var->is_function && var->is_definition) // function is defined in this .c file
      continue;

    if (!var->is_definition) {
      if (strcmp(var->name, "printf") == 0 || strcmp(var->name, "scanf") == 0)
        continue;
      println(".extern %s", var->name);
      continue;
    }

    if (V) {
      if (var->is_static)
        println("// .local %s", var->name);
      else
        println("// .globl %s", var->name);
    }

    int align = (var->ty->kind == TY_ARRAY && var->ty->size >= 16)
      ? MAX(16, var->align) : var->align;

    // Common symbol
    if (opt_fcommon && var->is_tentative) {
      if (V) println("// .comm %s, %d, %d", var->name, var->ty->size, align);
      //continue;
    }

    // .data or .tdata
    if (var->init_data) {
      if (V) {
        if (var->is_tls)
          println("// .section .tdata,\"awT\",@progbits");
        else
          println("// .data");

        println("// .type %s, @object", var->name);
        println("// .size %s, %d", var->name, var->ty->size);
        println("// .kind %s, %d", var->name, var->ty->kind);
        if (var->tok)
          println("// .tokkind %s, %d", var->name, var->tok->kind);
        println("// .align %d", align);
        println("// .varalign %d", var->align);
      }
      if (var->name[0] == '.')
        println("// .label %s", var->name);
    /*
      Global functions get a compiler generated label followed by their ASCII name. Two of these actually.
      Compiler labels are LL__. I want to omit these labels and data, but there are other compiler generated
      labels I want to keep.
      else if (var->name[0] == 'L' && var->name[1] == 'L' && var->name[2] == '_' && var->name[3] == '_' &&
               var->ty->kind == TY_FUNC) { <<-- This is not TY_FUNC
        println("// .label %s", var->name);
        continue;
      }
     */
      else {
        if (var->name[0] == 'L' && var->name[1] == 'L')
            println(".local %s", var->name);
        else
            println(".label %s", var->name);
      }

/* TODO
The following code generates 32-bit values for static data
- chemu is big endian for int
- chemu uses calls linux printf for its internal printf
- chemu converts strings from big to little endian
- I use the var->align == 1 to determine a string
- Strings are output in little endian

Currently, we do not generate a .bss section, which collects uninitialized static variables.
Instead, we generate zeros for unitialized static variables.
 */

      Relocation *rel = var->rel;
      int pos = 0;
      int value = 0;
      while (pos < var->ty->size) {
        if (rel && rel->offset == pos) {
          if (V) println("// .quad %s%+ld", *rel->label, rel->addend);
          println("  %s + %ld", *rel->label, rel->addend);
          rel = rel->next;
          pos += 8;
        } else {
/*
The next two println calls are helpful to see the byte values
 */
          //println("  .byte %d", var->init_data[pos]);
          //println("  .byte %x", var->init_data[pos] & 0xff);
          int shft = (pos % 4) * 8;
          int dataval = var->init_data[pos++] & 0xff;
          if (var->align == 1)
            value = (value << 8) | dataval;
          else
            value = value | (dataval << shft);
	  if ((pos % 4) == 0) {       // output 32-bits (4 bytes) at a time
            println("  0x%x", value);
	    value = 0;
	  }
          //println("  .byte %d", var->init_data[pos++]);
        }
      }
      if (var->ty->size % 4)  {// size is not a multiple of 4 (like string lit)
        // pad to be a multiple of 4
        for (int i = 0; i < (4 - var->ty->size % 4); i++)
          value = (value << 8) | 0;
        println("  0x%x", value);
      }
      if (var->name[0] == '.')
        println("// %d", value);
      //else
      //  println("  %d", value);
      continue;
    }

    // .bss or .tbss
    int w = align_to(var->ty->size, 4) / 4;
    println(".label %s", var->name);
    for (int i = 0; i < w; i++)
      println("  0x0");
    if (V) {
      if (var->is_tls)
        println("// .section .tbss,\"awT\",@nobits");
      else
        println("// .bss");

      println("// .align %d", align);
      println("// .zero %d", var->ty->size);
      println("// .words %d", w);
    }
  }
}

// Called to save fp parameter values passed in regs
static void store_fp(int r, int offset, int sz) {
  switch (sz) {
  case 4:
    println("  str %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  case 8:
    println("  str %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  }
  unreachable();
}

// Called to save gp parameter values passed in regs
static void store_gp(int r, int offset, int sz) {
  switch (sz) {
  case 1:
    println("  stb %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  case 2:
    println("  str %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  case 4:
    println("  str %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  case 8:
    println("  str %s, [r13, #%d] // sz: %d", argregcharm[r], -offset, sz);
    return;
  default:
    println("  str %s, [r13, #%d] // sz: %d, default", argregcharm[r], -offset, sz);
    //for (int i = 0; i < sz; i++) {
    //  println("  mov %s, %d(%%rbp)", argreg8[r], offset + i);
    //  println("  shr $8, %s", argreg64[r]);
    //}
    return;
  }
}

static int dot_text = 1;

static void emit_text(Obj *prog) {
  for (Obj *fn = prog; fn; fn = fn->next) {
    if (!fn->is_function || !fn->is_definition)
      continue;

    // No code is emitted for "static inline" functions
    // if no one is referencing them.
    if (!fn->is_live)
      continue;

    if (dot_text) {
      println(".text 0x700");
      dot_text = 0;
    }
    if (fn->is_static)
      println("// .local %s", fn->name);
    else
      println("// .globl %s", fn->name);

    println("// .type %s, @function", fn->name);
    println(".label %s", fn->name);
    current_fn = fn;

    // GUSTY - TODO - ignore variatic functions for now. Maybe forever in Charm
    int off = 0;
    //if (fn->va_area)
    //    off = fn->va_area->offset;
    //else
    //    off = 0;
#define REG_SPACE 9*4
    int stack_size = fn->stack_size;
    if (fn->va_area)
        stack_size = stack_size + fn->va_area->offset;
    //printf("GUSTY: fn->stack_size: %d\n", fn->stack_size);
    println("  sub r13, r13, #%d   // carve stack", stack_size + REG_SPACE);
    // __reg_save_area__ // GUSTY - save regs used by function
    println("  str r14, [r13, #%d] // save lr on stack", off + stack_size  + REG_SPACE - 16);
    println("  str r12, [r13, #%d] // save fp on stack", off + stack_size  + REG_SPACE - 20);
    println("  mov r12, r13        // Establish fp");
    println("  str r6, [r13, #%d]  // save r6 on stack", off + stack_size + REG_SPACE - 24);
    println("  str r7, [r13, #%d]  // save r7 on stack", off + stack_size + REG_SPACE - 28);
    println("  str r10, [r13, #%d] // save r10 on stack",off + stack_size + REG_SPACE - 32);
    // Save arg registers if function is variadic
    // GUSTY - ignore variatic functions for now. Maybe forever in Charm
    // NOTE: main is variatic
    if (fn->va_area) {
      //printf("GUSTY: fn->va_area->offset: %d\n", fn->va_area->offset);
      int gp = 0, fp = 0;
      for (Obj *var = fn->params; var; var = var->next) {
        if (is_flonum(var->ty))
          fp++;
        else
          gp++;
      }

      off = fn->va_area->offset;

      // va_elem
      //println("  movl $%d, %d(%%rbp)", gp * 8, off);          // gp_offset
      //println("  movl $%d, %d(%%rbp)", fp * 8 + 48, off + 4); // fp_offset
      //println("  movq %%rbp, %d(%%rbp)", off + 8);            // overflow_arg_area
      //println("  addq $16, %d(%%rbp)", off + 8);
      //println("  movq %%rbp, %d(%%rbp)", off + 16);           // reg_save_area
      //println("  addq $%d, %d(%%rbp)", off + 24, off + 16);

    }

// The num of regs with args is determined by GP_MAX and FP_MAX
// TODO - I have to merge store_fp and store_gp into one
    // Save passed-by-register arguments to the stack
    int gp = 0, fp = 0;
    for (Obj *var = fn->params; var; var = var->next) {
      if (var->offset > 0)
        continue;

      Type *ty = var->ty;

      switch (ty->kind) {
      case TY_STRUCT:
      case TY_UNION:
        assert(ty->size <= 16);
        if (has_flonum(ty, 0, 8, 0))
          store_fp(fp++, var->offset, MIN(8, ty->size));
        else
          store_gp(gp++, var->offset, MIN(8, ty->size));

        if (ty->size > 8) {
          if (has_flonum(ty, 8, 16, 0))
            store_fp(fp++, var->offset + 8, ty->size - 8);
          else
            store_gp(gp++, var->offset + 8, ty->size - 8);
        }
        break;
      case TY_FLOAT:
      case TY_DOUBLE:
        store_fp(fp++, var->offset, ty->size);
        break;
      default:
        store_gp(gp++, var->offset, ty->size);
      }
    }

    // Emit code
    gen_stmt(fn->body);
    assert(depth == 0);

    // [https://www.sigbus.info/n1570#5.1.2.2.3p1] The C spec defines
    // a special rule for the main function. Reaching the end of the
    // main function is equivalent to returning 0, even though the
    // behavior is undefined for the other functions.
    if (strcmp(fn->name, "main") == 0)
      println("  mov r0, #0       // end of main without return returns a 0"); // GUSTY

    // Epilogue // GUSTY
    println(".local LL_return_%s", fn->name); 
    if (strcmp(fn->name, "main") == 0) {
      println("  bal LL_return_%s // loop on self for main", fn->name);
      println("                   // Future - main can return to OS");
    }
    // __reg_save_area__ // GUSTY - restore regs used by function
    println("  ldr lr, [sp, #%d]   // restore lr from stack", off + fn->stack_size  + REG_SPACE - 16);
    println("  ldr r12, [sp, #%d]  // restore fp from stack", off + stack_size  + REG_SPACE - 20);
    println("  ldr r6, [sp, #%d]   // restore r6 from stack", off + fn->stack_size + REG_SPACE - 24);
    println("  ldr r7, [sp, #%d]   // restore r7 from stack", off + fn->stack_size + REG_SPACE - 28);
    println("  ldr r10, [sp, #%d]  // restore r10 from stack",off + fn->stack_size + REG_SPACE - 32);
    println("  add sp, sp, #%d     // restore stack", fn->stack_size + REG_SPACE);
    println("  mov pc, lr          // return");
  }
}

void codegen(Obj *prog, FILE *out) {
  output_file = out;

  File **files = get_input_files();
  for (int i = 0; files[i]; i++)
    println("// .file %d \"%s\"", files[i]->file_no, files[i]->name);

  assign_lvar_offsets(prog);
  emit_data(prog);
  emit_text(prog);
}

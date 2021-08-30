// .file 1 "tests/array_test2.c"
// r13 (sp) value
.stack 0x5000
// data section
.data 0x100
// .local LL__6
// .data
// .type LL__6, @object
// .size LL__6, 4
// .align 1
.label LL__6
  111
// .local LL__1
// .data
// .type LL__1, @object
// .size LL__1, 2
// .align 1
.label LL__1
// .local LL__0
// .data
// .type LL__0, @object
// .size LL__0, 2
// .align 1
.label LL__0
// .globl this
// .data
// .type this, @object
// .size this, 4
// .align 4
.label this
  100
// .globl a1
// .data
// .type a1, @object
// .size a1, 4
// .align 4
.label a1
  5
// .globl a
// .data
// .type a, @object
// .size a, 12
// .align 4
.label a
  1
  2
  3
.text 0x200
// .globl f
// .type f, @function
.label f
  sbi r13, r13, #116   // carve stack
  str r14, [r13, #100] // save lr on stack
  str r12, [r13, #96] // save fp on stack
  mov r12, r13        // Establish fp
  str r6, [r13, #92]  // save r6 on stack
  str r7, [r13, #88]  // save r7 on stack
  str r10, [r13, #84] // save r10 on stack
  // zero b
  mov r6, #0
  str r6, [r13, #60]
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 0
  mov r7, #0
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable b, offset: 60
  adi r7, r12, #60    // addr of b to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 1
  mov r7, #1
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable b, offset: 60
  adi r7, r12, #60    // addr of b to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 5
  mov r7, #5
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 2
  mov r7, #2
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable b, offset: 60
  adi r7, r12, #60    // addr of b to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 6
  mov r7, #6
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // zero p
  mov r6, #0
  str r6, [r13, #48]
  // local variable p, offset: 48
  adi r7, r12, #48    // addr of p to r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_begin_1
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 3
  mov r7, #3
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 48
  adi r7, r12, #48    // addr of p to r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_2
  mov r7, #0
.label LL_cond_2
  cmp r7, #0
  beq LL__2
  // local variable , offset: 32
  adi r7, r12, #32    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: this
  mva r7, this // addr of this to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 32
  adi r7, r12, #32    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 48
  adi r7, r12, #48    // addr of p to r7
  ldr r7, [r7]
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 32
  adi r7, r12, #32    // addr of  to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL__3
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: -1
  mov r7, #-1
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 40
  adi r7, r12, #40    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 48
  adi r7, r12, #48    // addr of p to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 40
  adi r7, r12, #40    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 1
  mov r7, #1
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 40
  adi r7, r12, #40    // addr of  to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  bal LL_begin_1
.label LL__2
  // zero i
  mov r6, #0
  str r6, [r13, #20]
  // local variable i, offset: 20
  adi r7, r12, #20    // addr of i to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 0
  mov r7, #0
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_begin_3
  // number: 3
  mov r7, #3
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 20
  adi r7, r12, #20    // addr of i to r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_4
  mov r7, #0
.label LL_cond_4
  cmp r7, #0
  beq LL__4
  // local variable , offset: 8
  adi r7, r12, #8    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a1
  mva r7, a1 // addr of a1 to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 8
  adi r7, r12, #8    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 20
  adi r7, r12, #20    // addr of i to r7
  ldr r7, [r7]
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable b, offset: 60
  adi r7, r12, #60    // addr of b to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 8
  adi r7, r12, #8    // addr of  to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL__5
  // number: -1
  mov r7, #-1
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 16
  adi r7, r12, #16    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 20
  adi r7, r12, #20    // addr of i to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 16
  adi r7, r12, #16    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // number: 1
  mov r7, #1
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 16
  adi r7, r12, #16    // addr of  to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  bal LL_begin_3
.label LL__4
  // global variable: a1
  mva r7, a1 // addr of a1 to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__6
  mva r7, LL__6 // addr of LL__6 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mov printf@GOTPCREL(%rip), %rax
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
.label LL_return_f
  ldr lr, [sp, #100]   // restore lr from stack
  ldr r12, [sp, #96]  // restore fp from stack
  ldr r6, [sp, #92]   // restore r6 from stack
  ldr r7, [sp, #88]   // restore r7 from stack
  ldr r10, [sp, #84]  // restore r10 from stack
  adi sp, sp, #116     // restore stack
  mov pc, lr          // return

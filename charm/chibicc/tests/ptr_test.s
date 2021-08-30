// .file 1 "tests/ptr_test.c"
// r13 (sp) value
.stack 0x5000
// data section
.data 0x100
// .local LL__1
// .data
// .type LL__1, @object
// .size LL__1, 2
// .align 1
.label LL__1
  102
// .local LL__0
// .data
// .type LL__0, @object
// .size LL__0, 2
// .align 1
.label LL__0
  102
// .globl z
// .data
// .type z, @object
// .size z, 4
// .align 4
.label z
  0
// .globl gp
// .data
// .type gp, @object
// .size gp, 8
// .align 8
.label gp
  0
.text 0x200
// .globl f
// .type f, @function
.label f
  sbi r13, r13, #68   // carve stack
  str r14, [r13, #52] // save lr on stack
  str r12, [r13, #48] // save fp on stack
  mov r12, r13        // Establish fp
  str r6, [r13, #44]  // save r6 on stack
  str r7, [r13, #40]  // save r7 on stack
  str r10, [r13, #36] // save r10 on stack
  // zero x
  mov r6, #0
  str r6, [r13, #20]
  // local variable x, offset: 20
  adi r7, r12, #20    // addr of x to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 5
  mov r7, #5
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // zero lp
  mov r6, #0
  str r6, [r13, #16]
  // local variable lp, offset: 16
  adi r7, r12, #16    // addr of lp to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable x, offset: 20
  adi r7, r12, #20    // addr of x to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // zero y
  mov r6, #0
  str r6, [r13, #4]
  // local variable y, offset: 4
  adi r7, r12, #4    // addr of y to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 5
  mov r7, #5
  str r7, [r13, #-4]! // push r7 on stack
  // local variable lp, offset: 16
  adi r7, r12, #16    // addr of lp to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // global variable: gp
  mva r7, gp // addr of gp to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable y, offset: 4
  adi r7, r12, #4    // addr of y to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // global variable: z
  mva r7, z // addr of z to r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: gp
  mva r7, gp // addr of gp to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_return_f
  ldr lr, [sp, #52]   // restore lr from stack
  ldr r12, [sp, #48]  // restore fp from stack
  ldr r6, [sp, #44]   // restore r6 from stack
  ldr r7, [sp, #40]   // restore r7 from stack
  ldr r10, [sp, #36]  // restore r10 from stack
  adi sp, sp, #68     // restore stack
  mov pc, lr          // return

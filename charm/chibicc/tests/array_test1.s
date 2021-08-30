// .file 1 "tests/array_test1.c"
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
  sbi r13, r13, #52   // carve stack
  str r14, [r13, #36] // save lr on stack
  str r12, [r13, #32] // save fp on stack
  mov r12, r13        // Establish fp
  str r6, [r13, #28]  // save r6 on stack
  str r7, [r13, #24]  // save r7 on stack
  str r10, [r13, #20] // save r10 on stack
  // global variable: a1
  mva r7, a1 // addr of a1 to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 2
  mov r7, #2
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 0
  mov r7, #0
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: this
  mva r7, this // addr of this to r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_return_f
  ldr lr, [sp, #36]   // restore lr from stack
  ldr r12, [sp, #32]  // restore fp from stack
  ldr r6, [sp, #28]   // restore r6 from stack
  ldr r7, [sp, #24]   // restore r7 from stack
  ldr r10, [sp, #20]  // restore r10 from stack
  adi sp, sp, #52     // restore stack
  mov pc, lr          // return

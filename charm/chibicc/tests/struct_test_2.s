// .file 1 "tests/struct_test.c"
// .file 2 "tests/struct_test.h"
// r13 (sp) value
.stack 0x5000
// data section
.data 0x100
// .local LL__3
// .data
// .type LL__3, @object
// .size LL__3, 20
// .kind LL__3, 12
// .align 16
// .varalign 1
.label LL__3
  0x4e616d65
  0x3a204775
  0x73747920
  0x6167653a
  0x20256400
// .local LL__2
// .data
// .type LL__2, @object
// .size LL__2, 12
// .kind LL__2, 12
// .align 1
// .varalign 1
.label LL__2
  0x73747275
  0x63742074
  0x65737400
// .local LL__1
// .data
// .type LL__1, @object
// .size LL__1, 12
// .kind LL__1, 12
// .align 1
// .varalign 1
.label LL__1
  0x73747275
  0x63745f74
  0x65737400
// .local LL__0
// .data
// .type LL__0, @object
// .size LL__0, 12
// .kind LL__0, 12
// .align 1
// .varalign 1
.label LL__0
  0x73747275
  0x63745f74
  0x65737400
// .globl p
// .data
// .type p, @object
// .size p, 12
// .kind p, 14
// .align 4
// .varalign 4
.label p
  0x74737547
  0x79
  0x16
.text 0x200
// .globl struct_test
// .type struct_test, @function
.label struct_test
  sbi r13, r13, #52   // carve stack
  str r14, [r13, #36] // save lr on stack
  str r12, [r13, #32] // save fp on stack
  mov r12, r13        // Establish fp
  str r6, [r13, #28]  // save r6 on stack
  str r7, [r13, #24]  // save r7 on stack
  str r10, [r13, #20] // save r10 on stack
  // global variable: LL__2
  mva r7, LL__2  // addr of LL__2 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // global variable: p
  mva r7, p  // addr of p to r7
  adi r7, r7, #8 // add member offset
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__3
  mva r7, LL__3  // addr of LL__3 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
.label LL_return_struct_test
  ldr lr, [sp, #36]   // restore lr from stack
  ldr r12, [sp, #32]  // restore fp from stack
  ldr r6, [sp, #28]   // restore r6 from stack
  ldr r7, [sp, #24]   // restore r7 from stack
  ldr r10, [sp, #20]  // restore r10 from stack
  adi sp, sp, #52     // restore stack
  mov pc, lr          // return

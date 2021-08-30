// .file 1 "tests/printf_test.c"
// r13 (sp) value
.stack 0x5000
// data section
.data 0x100
// .local LL__3
// .data
// .type LL__3, @object
// .size LL__3, 6
// .kind LL__3, 12
// .align 1
// .varalign 1
.label LL__3
  0x623a2025
  0x78000000
// .local LL__2
// .data
// .type LL__2, @object
// .size LL__2, 23
// .kind LL__2, 12
// .align 16
// .varalign 1
.label LL__2
  0x47757374
  0x7920436f
  0x6f706572
  0x20697320
  0x74797069
  0x6e670000
// .local LL__1
// .data
// .type LL__1, @object
// .size LL__1, 2
// .kind LL__1, 12
// .align 1
// .varalign 1
.label LL__1
  0x66000000
// .local LL__0
// .data
// .type LL__0, @object
// .size LL__0, 2
// .kind LL__0, 12
// .align 1
// .varalign 1
.label LL__0
  0x66000000
// .globl s
// .data
// .type s, @object
// .size s, 13
// .kind s, 12
// .align 1
// .varalign 1
.label s
  0x47757374
  0x7920436f
  0x6f706572
  0x0
// .globl a
// .data
// .type a, @object
// .size a, 12
// .kind a, 12
// .align 4
// .varalign 4
.label a
  0x1
  0x2
  0x1234
// .globl b
// .data
// .type b, @object
// .size b, 4
// .kind b, 4
// .align 4
// .varalign 4
.label b
  0xaabbccdd
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
  // global variable: LL__2
  mva r7, LL__2  // addr of LL__2 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // global variable: b
  mva r7, b  // addr of b to r7
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
.label LL_return_f
  ldr lr, [sp, #36]   // restore lr from stack
  ldr r12, [sp, #32]  // restore fp from stack
  ldr r6, [sp, #28]   // restore r6 from stack
  ldr r7, [sp, #24]   // restore r7 from stack
  ldr r10, [sp, #20]  // restore r10 from stack
  adi sp, sp, #52     // restore stack
  mov pc, lr          // return

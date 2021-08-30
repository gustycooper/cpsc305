// .file 1 "tests/loop_sum_test.c"
// r13 (sp) value
.stack 0x5000
// data section
.data 0x100
// .local LL__11
// .data
// .type LL__11, @object
// .size LL__11, 8
// .kind LL__11, 12
// .align 1
// .varalign 1
.label LL__11
  0x73756d3a
  0x20256400
// .local LL__10
// .data
// .type LL__10, @object
// .size LL__10, 13
// .kind LL__10, 12
// .align 1
// .varalign 1
.label LL__10
  0x6e656761
  0x74697665
  0x3a202564
  0x0
// .local LL__7
// .data
// .type LL__7, @object
// .size LL__7, 17
// .kind LL__7, 12
// .align 16
// .varalign 1
.label LL__7
  0x666f7220
  0x706f696e
  0x74657220
  0x6c6f6f70
  0x0
// .local LL__6
// .data
// .type LL__6, @object
// .size LL__6, 8
// .kind LL__6, 12
// .align 1
// .varalign 1
.label LL__6
  0x73756d3a
  0x20256400
// .local LL__5
// .data
// .type LL__5, @object
// .size LL__5, 13
// .kind LL__5, 12
// .align 1
// .varalign 1
.label LL__5
  0x6e656761
  0x74697665
  0x3a202564
  0x0
// .local LL__2
// .data
// .type LL__2, @object
// .size LL__2, 18
// .kind LL__2, 12
// .align 16
// .varalign 1
.label LL__2
  0x666f7220
  0x636f756e
  0x74696e67
  0x206c6f6f
  0x70000000
// .local LL__1
// .data
// .type LL__1, @object
// .size LL__1, 9
// .kind LL__1, 12
// .align 1
// .varalign 1
.label LL__1
  0x6c6f6f70
  0x5f73756d
  0x0
// .local LL__0
// .data
// .type LL__0, @object
// .size LL__0, 9
// .kind LL__0, 12
// .align 1
// .varalign 1
.label LL__0
  0x6c6f6f70
  0x5f73756d
  0x0
// .globl a
// .data
// .type a, @object
// .size a, 20
// .kind a, 12
// .align 16
// .varalign 4
.label a
  0x1
  0x2
  0xfffffffd
  0x4
  0x5
.text 0x200
// .globl loop_sum
// .type loop_sum, @function
.label loop_sum
  sbi r13, r13, #100   // carve stack
  str r14, [r13, #84] // save lr on stack
  str r12, [r13, #80] // save fp on stack
  mov r12, r13        // Establish fp
  str r6, [r13, #76]  // save r6 on stack
  str r7, [r13, #72]  // save r7 on stack
  str r10, [r13, #68] // save r10 on stack
  // zero sum
  mov r6, #0
  str r6, [r13, #48]
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 0
  mov r7, #0
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // global variable: LL__2
  mva r7, LL__2  // addr of LL__2 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // zero i
  mov r6, #0
  str r6, [r13, #44]
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  str r7, [r13, #-4]! // push r7 on stack
  // number: 0
  mov r7, #0
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_begin_1
  // number: 5
  mov r7, #5
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_2
  mov r7, #0
.label LL_cond_2
  cmp r7, #0
  beq LL__3
  // number: 0
  mov r7, #0
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  ldr r7, [r7]
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a  // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_4
  mov r7, #0
.label LL_cond_4
  cmp r7, #0
  beq LL_else_3
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  ldr r7, [r7]
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a  // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__5
  mva r7, LL__5  // addr of LL__5 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  bal LL_end_3
.label LL_else_3
.label LL_end_3
  // local variable , offset: 32
  adi r7, r12, #32    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 32
  adi r7, r12, #32    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  ldr r7, [r7]
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a  // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
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
.label LL__4
  // number: -1
  mov r7, #-1
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 40
  adi r7, r12, #40    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable i, offset: 44
  adi r7, r12, #44    // addr of i to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 40
  adi r7, r12, #40    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // number: 1
  mov r7, #1
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
.label LL__3
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__6
  mva r7, LL__6  // addr of LL__6 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // global variable: LL__7
  mva r7, LL__7  // addr of LL__7 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // zero p
  mov r6, #0
  str r6, [r13, #24]
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a  // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
.label LL_begin_5
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: 5
  mov r7, #5
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: a
  mva r7, a  // addr of a to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  add r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_6
  mov r7, #0
.label LL_cond_6
  cmp r7, #0
  beq LL__8
  // number: 0
  mov r7, #0
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  ldr r7, [r7]
  ldr r7, [r7]
  ldr r6, [r13], #4   // pop top of stack into r6
  cmp r7, r6
  mov r7, #1
  blt LL_cond_8
  mov r7, #0
.label LL_cond_8
  cmp r7, #0
  beq LL_else_7
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  ldr r7, [r7]
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__10
  mva r7, LL__10  // addr of LL__10 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  bal LL_end_7
.label LL_else_7
.label LL_end_7
  // local variable , offset: 8
  adi r7, r12, #8    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 8
  adi r7, r12, #8    // addr of  to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  ldr r7, [r7]
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
//.label gusty_cooper
.label LL__9
//.label L9
  // number: 4
  mov r7, #4
  str r7, [r13, #-4]! // push r7 on stack
  // number: -1
  mov r7, #-1
  // cast i32 to i64
  ldr r6, [r13], #4   // pop top of stack into r6
//.label gusty_cooper
  mul r7, r6, r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable , offset: 16
  adi r7, r12, #16    // addr of  to r7
  str r7, [r13, #-4]! // push r7 on stack
  // local variable p, offset: 24
  adi r7, r12, #24    // addr of p to r7
  ldr r6, [r13], #4   // pop top of stack into r6
  str r7, [r6]
  // local variable , offset: 16
  adi r7, r12, #16    // addr of  to r7
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
  bal LL_begin_5
.label LL__8
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  ldr r7, [r7]
  str r7, [r13, #-4]! // push r7 on stack
  // global variable: LL__11
  mva r7, LL__11  // addr of LL__11 to r7
  str r7, [r13, #-4]! // push r7 on stack
  mva r7, 0x7000   // addr of func printf
  ldr r0, [r13], #4   // pop top of stack into r0
  ldr r1, [r13], #4   // pop top of stack into r1
  mov r10, r7
  mov r7, #0
  blr [r10]
  mov r7, r0 // mov return value to r7 for assignment
  // local variable sum, offset: 48
  adi r7, r12, #48    // addr of sum to r7
  ldr r7, [r7]
  mov r0, r7          // return value in r0
  bal LL_return_loop_sum
.label LL_return_loop_sum
  ldr lr, [sp, #84]   // restore lr from stack
  ldr r12, [sp, #80]  // restore fp from stack
  ldr r6, [sp, #76]   // restore r6 from stack
  ldr r7, [sp, #72]   // restore r7 from stack
  ldr r10, [sp, #68]  // restore r10 from stack
  adi sp, sp, #100     // restore stack
  mov pc, lr          // return

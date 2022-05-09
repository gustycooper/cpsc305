// data at 0x100
.data 0x100
// int a, b = 20, c = 30;
.label a
0xabadbead  // int a = 0xabadbead;
.label b
0xaabbccdd // int b = 0xaabbccdd;
// emu $ m 400 aabbccdd
.label c
0xfeedabee // int c = 0xfeedabee;
// emu $ m 404 feedabee
.label d
0xbeededad // int c = 0xbeededad;
.label e
0
.label f
0
.label gusty
0
// code at 0x200
.text 0x200
// various load and store instructions
.label code
mva r4, b           // r4 points to b
ldr r8, [r4]        // load b into r8
ldr r9, [r4, #4]    // load c into r9
ldr r9, [r4, #8]    // load d into r9
.label bp1          // use this for a breakpoint
ldr r10, [r4, #-4]! // r4 gets r4-4, load a into r10
ldr r11, [r4], #4   // load a into r11, r4 gets r4+4
ldr r12, [r4, #-4]  // load a into r12, r4 unchanged
mva r6, 0x10c       // r6 points to d
mov r3, #0xff       // put 255 in r3
str r3, [r6]        // store r3 in d, d = 255
str r3, [r6, #4]    // store r3 in e, e = 255
str r3, [r6, #8]    // store r3 in f, f = 255
str r3, [r6, #-4]!  // r6 gets r6-4: r6 has 0x108, store r3 in c
mov r2, #1          // put 1 in r2
add r3, r3, r2      // add 255 and 1. Result to r3
str r3, [r6], #4    // store r3 (256) in c, r6 gets r6+4: 0x10c
str r3, [r6], #4    // store r3 (256) in gusty, r6 gets r6+4
str r3, [r6, #-4]   // store r3 (256) in gusty
// e = e + 1
ldr r5, e           // put e in r5
add r5, r5, 1       // add 1 to r5
str r5, e           // store r5 in e
// gusty = f - e
ldr r7, f           // put f in r7
ldr r8, e           // put e in r8
sub r8, r7, r8      // r8 = r7 - r8
str r8, gusty       // gusty = f - e
// e = e + 1 using a base register
mva r13, e          // put address of e in r13
ldr r5, [r13, 0]    // put e in r5
add r5, r5, 1       // add 1 to r5
str r5, [r13, 0]    // store r5 in e
// gusty = f - e using a base register
ldr r7, [r13, 4]    // put f in r7
ldr r8, [r13, 0]    // put e in r8
sub r8, r7, r8      // r8 = r7 - r8
str r8, [r13, 8]    // gusty = f - e
.label end
bal end             // branch to self

// emu $ r 4 400
// emu $ r 6 500
// Figure ISA-4
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
// code at 0x200
.text 0x200
// various load and store instructions
.label code
mva r4, b           // r4 points to b
ldr r8, [r4]
ldr r9, [r4, #4]
ldr r9, [r4, #8]
ldr r10, [r4, #-4]!
ldr r11, [r4], #4
ldr r12, [r4, #-4]
mva r6, 0x11c
mov r3, #0xff
str r3, [r6]
str r3, [r6, #4]
str r3, [r6, #8]
str r3, [r6, #-4]!
mov r2, #1
add r3, r3, r2
str r3, [r6], #4
str r3, [r6], #4
str r3, [r6, #-4]
.label end
bal end

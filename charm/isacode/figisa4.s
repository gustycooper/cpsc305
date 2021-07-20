// Figure ISA-4
// data at 0x100
.data 0x100
// int a, b = 20, c = 30;
.label a
0  // int a = 0;
.label b
20 // int b = 20;
.label c
30 // int c = 30;
// code at 0x200
.text 0x200
// a = b + c;
.label code
ldr r0, b      // put b in r0
ldr r1, c      // put c in r1
add r2, r0, r1 // r0 + r1 in r2
str r2, a      // put r2 in a
.label end
bal end

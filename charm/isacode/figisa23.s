// Figure ISA-23
// data at 0x100
.data 0x100
.label a
16 // int a = 16;
.label b
32 // int b = 32;
.label c
48 // int c = 48;
// code at 0x200
.label d1
0  // int d1 = 0;
.label d2
0  // int d2 = 0;
.text 0x200
.label code
ldr r0, a           // r0 is a
ldr r1, b           // r0 is b
ldr r2, c           // r0 is c
blr add3
str r0, d1          // updated d1, sum in r0
mva r13, a          // r13 points to a
ldr r0, [r13,#0]    // r0 is a     int a = 16, b = 32, c = 48;
ldr r1, [r13,#4]    // r1 is b     int d = add3(a, b, c);
ldr r2, [r13,#8]    // r2 is c
blr add3
str r0, [r13,#0x10] // update d2, sum in r0
.label end
bal end

.label add3         // int add3(int x, int y, int z) {
add r0, r0, r1      //    return x + y + z;
add r0, r0, r2      // } // sum is in r0
mov r15, r14        // return from add3

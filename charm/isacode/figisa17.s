// Figure ISA-4
// data at 0x100
.data 0x100
.label a
40 // int a = 40;
.label b
25 // int b = 25;
.label c
0  // int c = 0;
// code at 0x200
.text 0x200
// int a = 40;		int a = 40;
// int b = 25;		int b = 25;
// if (a <= b)		if (a > b)
// 			   Goto else_part
//    a -= b;		   a -= b;
// 			   goto end_if;
// else			else_part:
//    b += a;		   b += a;
// 			end_if:
// int c = a + b	int c = a + b;
.label code
// access a, b, c via static addresses
ldr r0, a             // r0 is a
ldr r1, b             // r1 is b
cmp r0, r1            // compare a and b
bgt else1             // branch > to else
sub r0, r0, r1        // r0 gets a-b
str r0, a             // update a
bal endif1              // branch around else code
.label else1
add r0, r0, r1        // r0 gets a+b
str r0, b             // update b 
.label endif1
ldr r0, a             // r0 is a
ldr r1, b             // r1 is b
add r0, r0, r1        // r0 gets a+b
str r0, c             // c = a + b
mva r13, a            // r13 points to a
// access a, b, c via r13
ldr r0, [r13,#0]      // r0 is a
ldr r1, [r13,#4]      // r1 is b
cmp r0, r1            // compare a and b
bgt else2             // branch > to else
sub r0, r0, r1        // r0 gets a-b
str r0, [r13,#0]      // update a
bal endif2              // branch around else code
.label else2
add r0, r0, r1        // r0 gets a+b
str r0, [r13,#4]      // update b 
.label endif2
ldr r0, [r13,#0]      // r0 is a
ldr r1, [r13,#4]      // r1 is b
add r0, r0, r1        // r0 gets a+b
str r0, [r13,#8]      // c = a + b
.label end
bal end

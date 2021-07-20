// Figure ISA-19
// int a = 40;
// int b = 25;
// while (a != b) {
//   if (a > b) 
//     a -= b;
//   else
//     b -= a;
// }
// int finalanswer = a; // a == b at this point

// data at 0x100
.data 0x100
.label a
40 // int a = 40;
.label b
25 // int b = 25;
.label finalanswer
0
// code at 0x200
.text 0x200
.label code
ldr r0, a             // r0 is a
ldr r1, b             // r1 is b
.label gcd1
cmp r0, r1            // compare a to b
beq endloop1
blt isless1
sub r0, r0, r1        // a -= b
bal gcd1
.label isless1
sub r1, r1, r0        // b -= a
bal gcd1
.label endloop1
str r0, finalanswer   // update finalanswer
mva r13, a            // r13 points to a
ldr r0, [r13,#0]      // r0 is a
ldr r1, [r13,#4]      // r1 is b
.label gcd2
cmp r0, r1            // compare a to b
beq endloop2
blt isless2
sub r0, r0, r1        // a -= b
bal gcd2
.label isless2
sub r1, r1, r0        // b -= a
bal gcd2
.label endloop2
str r0, [r13,#8]      // update finalanswer
.label end
bal end               // endless loop

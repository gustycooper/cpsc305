// Figure ISA-18
// int sum = 0;
// int i = 1;
// while (i < 11) {
//   sum += i;
//   i++;
// }
// int finalanswer = sum;

// data at 0x100
.data 0x100
.label sum
0 // int sum = 0;
.label i
1 // int i = 1;
.label finalanswer
0
.label bounds
0  // int bounds = 11;
// code at 0x200
.text 0x200
.label code
ldr r0, i
ldr r1, bounds
.label loop1
cmp r0, r1            // compare i to 11
bge endloop1          // exit loop on >=
ldr r2, sum           // put sum in r2
add r2, r0, r2        // r2 gets sum+i
str r2, sum           // sum += i
mov r3, 1
add r0, r0, r3        // i++
str r0, i             // update i
bal loop1             // branch to loop start
.label endloop1
str r2, finalanswer   // update finalanswer
mva r13, sum          // r13 points to sum
mov r0, #0
str r0, [r13,#0]      // sum = 0
mov r0, #1
str r0, [r13,#4]      // i = 1
mov r1, #11           // loop bounds
str r1, bounds        // bounds = 11
.label loop2
cmp r0, r1            // compare i to 11
bge endloop2          // exit loop on >=
ldr r2, [r13,#0]      // put sum in r2
add r2, r0, r2        // r2 gets sum+i
str r2, [r13,#0]      // sum += i
mov r3, 1
add r0, r0, r3        // i++
str r0, [r13,#4]      // update i
bal loop2             // branch to loop start
.label endloop2
str r2, [r13,#8]      // update finalanswer
.label end
bal end

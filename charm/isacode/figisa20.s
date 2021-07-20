// Figure ISA-20
// int ia[] = {1,2,3,4,5,6,7,8,9,10};
// int size = 10; // num elements in ia
// int sum=0;
// for (int i=0; i<size; i++) {
//   sum += ia[i];
// }
// int finalanswer = sum;

// data at 0x100
.data 0x100
.label ia
// int ia[] = {1,2,3,4,5,6,7,8,9,10};
1  //  0 - offset from the start of data
2  //  4
3  //  8
4  //  c
5  // 10
6  // 14
7  // 18
8  // 1c
9  // 20
10 // 24
.label size
10 // 28, int size = 10;
.label sum
0  // 2c, int sum = 0;
.label i
0  // 30, int i = 0;
.label finalanswer1
0  // 34, int finalanswer1 = 0;
.label finalanswer2
0  // 38, int finalanswer2 = 0;
// code at 0x200
.text 0x200
.label code
ldr r0, size        // r0 is size        
mva r4, ia          // r4 has address of ia
.label addloop1
ldr r3, i           // r3 has i
cmp r3, r0          // cmp i to size
bge endloop1
shf r3, 2           // multiply i by 4 - 0 to 0, 1 to 4, 2 to 8
ldr r5, [r4, r3]    // put ia[i] in r5
ldr r2, sum         // r2 has sum
add r2, r2, r5      // add ia[i] to the accumulating sum
str r2, sum         // update sum
ldr r3, i           // r3 has i
mov r5, 1           // r5 has 1
add r3, r3, r5      // add 1 to i
str r3, i           // update i
bal addloop1        // continue looping until i > size
.label endloop1
ldr r2, sum         // r2 has sum
str r2, finalanswer1 // finalanswer1 = sum
// Second Loop - use r13 and offsets to access variables
mva r13, ia
mov r0, 0
str r0, [r13,#0x2c] // sum = 0; // prev loop modified
str r0, [r13,#0x30] // i = 0;   // prev loop modified
ldr r0, [r13,#0x28] // r0 is size        
ldr r2, [r13,#0x2c] // r2 is sum
ldr r3, [r13,#0x30] // r3 is i, counts from 1 to size
mov r6, 1           // r6 is 1 to increment i
mov r4, r13         // r4 has address of ia
.label addloop2
cmp r3, r0          // cmp i to size
bge endloop2
ldr r5, [r4],#4     // put ia[i] in r5, post increment r4
add r2, r2, r5      // add ia[i] to the accumulating sum
str r2, [r13,#0x2c] // update sum
add r3, r3, r6      // add 1 to i
str r3, [r13,#0x28] // update i
bal addloop2        // continue looping until i >= size
.label endloop2
str r2, [r13,#0x38] // finalanswer2 = sum;
.label end
bal end             // endless loop

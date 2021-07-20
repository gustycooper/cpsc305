// Figure ISA-24
// int l
// int ia = {1,2,3,4,5,6,7,8,9,10};
// int main() {
//   l = largest(ia, 10);
// }
// int largest(int *a, int size) {
//   int l = a[0];
//   for (int i = 0; i < size; i++)
//     if (a[i] > l)
//       l = a[i];
//   return;
// }
// data at 0x100
.data 0x100
.label l1
0  // int l1 = 0;
.label l2
0  // int l2 = 0;
.label size
10 // 28, int size = 10;
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
// code at 0x200
.text 0x200
.label code
.label main
mva r0, ia          // r0 has address of ia
ldr r1, size        // r1 has size of ia
blr largest
str r0, l1          // l1 = largest(ia,10);
mva r13, ia
mov r0, r13
ldr r1, size        // r1 has size of ia
blr largest
str r0, [r13, #-8]  // l1 = largest(ia,10);
.label end
bal end             // endless loop

.label largest
ldr r2, [r0]        // r2 is largest
mov r4, r0          // put address of a in r4
mov r3, #0          // i = 0
.label loop
cmp r3, r1          // compare i to size
bge endloop         // continue looping until i >= size
ldr r5, [r4],#4     // put ia[i] in r5, post increment r4
cmp r5, r2          // cmp ia[i] to largest
blt skip
mov r2, r5          // update largest
.label skip
mov r5, #1          // r5 has 1
add r3, r3, r5      // add 1 to i
bal loop            // continue looping until i >= size
.label endloop
mov r0, r2          // place largest in r0
mov r15, r14        // return to caller

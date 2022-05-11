// void do_something() {           .label do_something
//     printf("Hello World\n");    mov r15, r14
// }
// int add(int a, int b) {         .label add
//     int sum = a+b;
//     do_something();	            blr do_something
//     if (sum == 4)
//        a = sum;
//     else
//        a = -sum; 
//     return a+b;                  mov r15, r14
// }
// int main() {	                   .label main
//     int a = 1, b = 2, sum = 0;
//     sum = add(a, b);             blr add
//     return 1;
// } 
// stack at 0x5000
.stack 0x5000
// data at 0x100
.data 0x100
.label statica
10  // int statica = 10;
.label staticb
20  // int staticb = 20;
.label staticsum
0  // int staticsum = 0;
.label strfmt
.string //Hello World\n
// code at 0x200
.text 0x200
.label code
.label do_something
sub r13,r13,#4
str r14,[r13]
// function entry sequence is previous two instructions
mva r1, strfmt   // addr of strfmt to r1
ker 0x11         // call printf
// function exit sequence is the next three instructions
ldr r14,[r13]
add r13,r13,#4
mov r15, r14     // return

.label add2
sub r13,r13,#16  // stack space
str r0,[r13,#0]  // Put a on stk
str r1,[r13,#4]  // Put b on stk
// sum is at [r13,#8]
str lr,[r13,#12] // put lr on stk
// function entry sequence is previous four instructions
add r2,r0,r1    // a+b into r2
str r2,[r13,#8] // sum = a+b
blr do_something // bl do_something
ldr r0,[r13,#8] // put sum in r0
cmp r0,#4       // cmp sum to 4
bne else
str r0,[r13]    // a = sum
bal endif
.label else
mov r1,0
sub r0,r1,r0    // rev: 0-r0
str r0,[r13]    // a = -sum
.label endif
ldr r1,[r13,#4] // put b into r1
add r0,r0,r1    // a+b to r0
// function exit sequence is the next three instructions
ldr lr,[r13,#12] // restore lr
add r13,r13,#16 // restore r13
mov r15, r14    // return
.text 0x300
.label main
sub r13,r13,#16 // stack space, a, b, sum, and lr on stack
mov r0,1
str r0,[r13]    // int a = 1;
mov r0,2
str r0,[r13,#4] // int b = 2;
mov r0,#0
str r0,[r13,#8] // int sum = 0;
str r14,[r13,12] // save lr
// function entry sequence is previous eight instructions
ldr r0,[r13]    // put a into r0
ldr r1,[r13,#4] // put b into r1
blr add2        // call add2
str r0,[r13,#8] // sum=returned val
ldr r0,statica  // put statica into r0
ldr r1,staticb  // put staticb into r0
blr add2        // call add2
str r0,staticsum // staticsum=return val
// main does not deallocate stack so we can examine sum
// d the value that is in r13
.label end
bal end

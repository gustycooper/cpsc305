// r13 set to 0x5000
.stack 0x5000
// Base address of interrupt vector table
.data 0x7ff0
0x6000        // address of OS for ker instruction
// OS base address
.text 0x6000
.label os_start
cmp r0, #0x10
beq scanf
cmp r0, #0x11
beq printf
bal done
.label scanf
ioi 0x10  // scanf
bal done
.label printf
ioi 0x11  // printf
.label done
mov r1, r0
srg #0x3b
mov r15, r14
//
// Address of printf is 0x7000, when called
//  r0 has addr of fmt string
//  r1 has first % variable, if any
//  r2 has second % variable, if any
.text 0x7000
str r14, [r13, #-4]! // push lr on stack
mov r3, r2           // set regs expected by ker 0x11
mov r2, r1
mov r1, r0
ker 0x11             // 0x11 is placed into r0
                     // user to kernel rupt is generated
ldr r14, [r13], #4   // pop lr from stack
mov r15, r14         // return
//
// Address of scanf is 0x7050, when called
//  r0 has addr of fmt string
//  r1 has first % variable, if any
//  r2 has second % variable, if any
.text 0x7050
str r14, [r13, #-4]! // push lr on stack
mov r3, r2           // set regs expected by ker 0x11
mov r2, r1
mov r1, r0
ker 0x10             // 0x10 is placed into r0
                     // user to kernel rupt is generated
ldr r14, [r13], #4   // pop lr from stack
mov r15, r14         // return

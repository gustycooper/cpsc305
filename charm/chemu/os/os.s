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
//.text 0x200
//ker 5   // test kernel instruction
//mov r0, -1

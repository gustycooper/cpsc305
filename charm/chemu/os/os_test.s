// os test
.data 0x100
.label s0
.string //Test Print
.label s1
.string //Test Print %d
.label s2
.string //Test Print %d and %d
.text 0x200
.label os_test_start
mva r1, s0
ker 0x11   // test kernel instruction for printf
mva r1, s1
mov r2, 10
ker 0x11   // test kernel instruction for printf
mva r1, s2
mov r2, 10
mov r3, 0x55
ker 0x11   // test kernel instruction for printf
mov r0, -1
ker 0x10   // test kernel instruction for scanf
mov r0, 0x10
mov r1, 0x11
.label end
bal end

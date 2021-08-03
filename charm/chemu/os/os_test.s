// os test
.data 0x100
.label s0
.string //Test Print
.label s1
.string //Test Print %d
.label s2
.string //Test Print %d and %d
.label s3
.string //%d
.label i
0
.label s4
.string //%s
.label s5
.string //1234567890123456
.label s6
.string //%d %s
.label j
0
.label s7
.string //%s
.label s8
.string //1234567890123456
.label prompt1
.string //Enter a number: 
.label prompt2
.string //Enter a string: 
.label prompt3
.string //Enter a number string:
.text 0x200
.label os_test_start
mva r1, s0 // fmt str "Test Print" - does not have %
ker 0x11   // test kernel instruction for printf
mva r1, s1 // fmt str "Test Print %d"
mov r2, 10 // The number 10 to be printed
ker 0x11   // test kernel instruction for printf
mva r1, s2 // fmt str "Test Print %d and %d
mov r2, 10 // The number 10 to be printed
mov r3, 0x55 // The number 0x55 to be printed
ker 0x11   // test kernel instruction for printf
mva r1, prompt1
ker 0x11
mov r0, -1 // put -1 into r0 - just an instruction in streem
mva r1, s3 // fmt str "%d"
mva r2, i  // &i - addr of int for %d
ker 0x10   // test kernel instruction for scanf %d
mva r1, prompt2
ker 0x11
ldr r0, i  // put i in r0
mva r1, s4 // fmt str "%s"
mva r2, s5 // s5 - address of string for %s
ker 0x10   // test kernel instruction for scanf %s
mva r1, prompt3
ker 0x11
mva r1, s6 // fmt str "%d %s"
mva r2, j  // &j - addr of int for %d
mva r3, s8 // s8 - addr of string for %s
ker 0x10   // test kernel instruction for scanf %d %s
mov r1, 0x11
.label end
bal end    // branch to self

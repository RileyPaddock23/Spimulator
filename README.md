This is my spimulator project and here is a little bit about how to use it and how I designed it

USAGE:

NOTE: FOR MEMORY ADDRESSES USE LINE NUMBER
    (Example: If you have a command on line 8, then to go to that command you would use "j 8")
    (Example: if you have something stored in memory.txt at line 10 you would use ($t1 = 10) "lw $t0 $t1 0)

The implemented commands are:
R type: add, addu, and, nor, or, slt, sltu, sll, sra, srl, sub, subu
I type: addi, addiu, andi, lui, ori, slti, sltiu, beq, bne, lw, sw, lhu, sh, lbu, sh
J type: j, jal, jr

There are 2 ways to use the spimulator the first by just writing a hex file the second by writing in "psuedo-MIPS"

Hex file:
If you go to the file "text.txt" you can put the hex codes corresponding to mips commands on each line.
Then make sure you finish the file with an "exit" command.

psuedo-MIPS:
The second way to compose a file is to write one in almost mips in "test_read.txt"
The conventions of psuedo-MIPS are as follows:
    Always start with the command keyword
    Registers always go in order ($rs, $rt, $rd)
    Immediates and shift amounts go on the end given as base 10
    Everything is seperated by spaces

(See test_read.txt for example)


Design:

This design is built on storing information as strings of zero's and one's
Within this framework I make heavy usage of the helper functions in "globals.cpp". These help me convert between hex, binary, and decimal. Additionally, it acts like an ALU in some ways in that it has a default add function which returns a carry bit, a flip function which flips the bits and a fill function which can fill space if I have a binary number less than 32 bits.

Additionally, globals is where I also first create the registers and the program counter

Then using the tools in global I have the Rtype and IJtype functions which when given the details of their appropriate function
they do the associated operations. None of these have anything too special in particular, just doing the required bit operations.

Then in spimulator.cpp I put the tools together with the do_command file which decodes the hex command and then gives it to either Rtype or IJtype with the information from the hex extracted into attributes for the function.

There is some more interesting stuff in asm_parser.cpp which takes a "psued-MIPS" program and creates the file of hex commands in "text.txt". 
The first thing you will notice if you look at "asm_parser.cpp" is the very long functions which translate the command names to the correct op-code or func-code and 
the ones which translate the names for the 32 registers to their binary. But once you have these it isn't too bad to translate an instruction given the simple format described above. 
I made sure to test every function on ~50 test cases but it is very probable that there are edge cases I am missing. I hope it works for you!

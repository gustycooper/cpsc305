## CPSC 305 In Class Code

This directory contains the source code that is created and used for in class activities

* computer.c - a teeny computer simulation program that has int memory[1024]
  * gcc -o computer computer.c
* computer1.c - a teeny computer simulation program that has char memory[1024]. This forces the CPU to collect bytes into an Endianess
  * gcc -o computer1 computer1.c
* pokey.c - a program that demonstrates ncurses as a Pokemon game. The game is not complete. You must have ncurses installed.
  * gcc -o pokey pokey.c -lncurses
* pokeydir - a directory that has a multi-file C program. The program continues the demonstration of the ncurses Pokemon game. You must have ncurses installed.
  * gcc -o pokey pokey_main.c pokmon.c -lncurses
  * The .txt files have pokmons and fighters


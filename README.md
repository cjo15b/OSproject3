# OSproject3

-Jeremy Dean-
cd, padding names, open, close, creat, mkdir read, write.
-Jean Bougeois-
initial structure, ls, open, close, creat, mkdir, read, write.
-Cody O'Connor - 
initial structure,  ls, cd, creat, padding names, miscellaneous helper functions.



Tar file contents
   -Makefile - make command to quickly compile and test the program
   -README - File contatining general information about the project and group members
   -filesystem.c - source file containing the functions used for FAT32 manipulation
   -filesystem.h - header file for filesystem.c
   -system_main.c - main source file for the project. Includes the command prompt for the FAT32 manipulation

How to compile using the Makefile

	$make
	$./a.out <fat image>
	$clean

Known Bugs and unfinished portions of the project
	-read and write segfault sometimes.

Special considerations
	-
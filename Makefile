#Makefile
CC=gcc
system_main: system_main.c
	gcc -std=c99 -g system_main.c filesystem.c
	./a.out fat32.img
clean:
	rm a.out

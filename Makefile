#Makefile
CC=gcc
system_main: system_main.c
	gcc -std=c99 system_main.c filesystem.c
	./a.out
clean:
	rm a.out

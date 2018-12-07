//filesystem.h
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void setBootBlock(char * FILENAME);
void initClusterNum();
void OurExit(char *ptr);
void info();
char* ls(char* FAT32, char* DIRNAME);
void cd(char* FAT32, char* DIRNAME);
void size(char* FAT32, char* DIRNAME);
char* creat(char* FILENAME);
void mkdir(char * FAT32, char* DIRNAME);



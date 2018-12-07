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
char* cd(char* DIRNAME);
char* size(char* FILENAME);
char* creat(char* FILENAME);
char* mkdir(char* DIRNAME);



//filesystem.h
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    unsigned int clust_num;
    unsigned short mode;
} Status;
Status *opened;

void setBootBlock(char * FILENAME);
void initClusterNum();
void OurExit(char *ptr);
void info();
char* ls(char* FAT32, char* DIRNAME);
void cd(char* FAT32, char* DIRNAME);
void size(char* FAT32, char* DIRNAME);
void mkdir(char * FAT32, char* DIRNAME);
void creat(char* FAT32, char* FILENAME);
void open(char* FAT32,  char* FILENAME, char* mode);



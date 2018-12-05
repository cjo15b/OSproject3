#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "filesystem.h"

typedef struct {
	unsigned char BS_jmpBoot[3];
	unsigned char BS_OEMName[8];
	unsigned short BPB_BytsPerSec;
	unsigned char BPB_SecPerClus;
	unsigned short BPB_RsvdSecCnt;
	unsigned char BPB_NumFATs;
	unsigned short BPB_RootEntCnt;
	unsigned short BPB_TotSec16;
	unsigned char BPB_Media;
	unsigned short BPB_FATSz16;
	unsigned short BPB_SecPerTrk;
	unsigned short BPB_NumHeads;
	unsigned int BPB_HiddSec;
	unsigned int BPB_TotSec32;
	unsigned int BPB_FATSz32;
	unsigned short BPB_ExtFlags;
	unsigned short BPB_FSVer;
	unsigned int BPB_RootClus;
	unsigned short  BPB_FSInfo;
	unsigned short BPB_BkBootSec;
	unsigned char BPB_Reserved[12];
	unsigned char BS_DrvNum;
	unsigned char BS_Reserved1;
	unsigned char BS_BootSig;
	unsigned int BS_VolID;
	unsigned char BS_VolLab[11];
	unsigned char BS_FilSysType[8];
} BootBlock;

void info(char * fileName){
	FILE * fat32 = fopen(fileName, "rb+");
	BootBlock * x;
    fread(&x.BS_jmpBoot, 1, 3, fat32);
    fread(&x.BS_OEMName, 1, 8, fat32);
    fread(x.BPB_BytsPerSec, sizeof(x.BPB_BytsPerSec), 1, fat32);
    fread(x.BPB_SecPerClus, sizeof(x.BPB_SecPerClus), 1, fat32);
	printf("jmpBoot 1: %x, jmpBoot 2: %x, jmpBoot 3: %x\n", x.BS_jmpBoot[0], x.BS_jmpBoot[1], x.BS_jmpBoot[2]);
}


void OurExit(char *ptr){
	printf("Exiting...\n");
	free(ptr);
	exit(0);
}


char* ls(char* DIRNAME){


	return DIRNAME;
}

char* cd(char* DIRNAME){

	return DIRNAME;
}

char* size(char* FILENAME){	

	return FILENAME;
}

char* creat(char* FILENAME){

	return FILENAME;
}

char* mkdir(char* DIRNAME){

	return DIRNAME;
}




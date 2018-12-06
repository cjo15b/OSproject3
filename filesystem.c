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

//Declaring globals
BootBlock x;

void info(char * fileName){
	FILE * fat32 = fopen(fileName, "rb+");
	int i;
	// Reading in all data from BootSector
    fread(&x.BS_jmpBoot, 1, 3, fat32);
    fread(&x.BS_OEMName, 1, 8, fat32);
    fread(&x.BPB_BytsPerSec, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_SecPerClus, sizeof(unsigned char), 1, fat32);
    fread(&x.BPB_RsvdSecCnt, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_NumFATs, sizeof(unsigned char), 1, fat32);
    fread(&x.BPB_RootEntCnt, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_TotSec16, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_Media, sizeof(unsigned char), 1, fat32);
    fread(&x.BPB_FATSz16, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_SecPerTrk, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_NumHeads, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_HiddSec, sizeof(unsigned int), 1, fat32);
    fread(&x.BPB_TotSec32, sizeof(unsigned int), 1, fat32);
    fread(&x.BPB_FATSz32, sizeof(unsigned int), 1, fat32);
    fread(&x.BPB_ExtFlags, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_FSVer, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_RootClus, sizeof(unsigned int), 1, fat32);
    fread(&x.BPB_FSInfo, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_BkBootSec, sizeof(unsigned short), 1, fat32);
    fread(&x.BPB_Reserved, 1, 12, fat32);
    fread(&x.BS_DrvNum, sizeof(unsigned char), 1, fat32);
    fread(&x.BS_Reserved1, sizeof(unsigned char), 1, fat32);
    fread(&x.BS_BootSig, sizeof(unsigned char), 1, fat32);
    fread(&x.BS_VolID, sizeof(unsigned int), 1, fat32);
    fread(x.BS_VolLab, 1, 11, fat32);
    fread(x.BS_FilSysType, 1, 8, fat32);
	printf("jmpBoot 1: 0x%x, jmpBoot 2: 0x%x, jmpBoot 3: 0x%x\n", x.BS_jmpBoot[0], x.BS_jmpBoot[1], x.BS_jmpBoot[2]);
	printf("OEM Name: %s", x.BS_OEMName);
	printf("\nBytes per sector: %d\n", x.BPB_BytsPerSec);
	printf("Sectors per cluster: %d\n", x.BPB_SecPerClus);
	printf("Number of reserved sectors: %d\n", x.BPB_RsvdSecCnt);
	printf("Number of FAT data strucures: %d\n", x.BPB_NumFATs);
	printf("Must be zero for FAT32(BPB_RootEntCnt): %d\n", x.BPB_RootEntCnt);
	printf("Must be zero for FAT32(BPB_TotSec16): %d\n", x.BPB_TotSec16);
	printf("Hex value for fixed media: 0x%x\n", x.BPB_Media);
	printf("Must be zero for FAT 32(BPB_FATSz16): %d\n", x.BPB_FATSz16);
	printf("Sectors per track: %d\n", x.BPB_SecPerTrk);
	printf("Number of heads: %d\n", x.BPB_NumHeads);
	printf("Number of hidden sectors: %d\n", x.BPB_HiddSec);
	printf("Total sectors on volume: %d\n", x.BPB_TotSec32);
	printf("Sectors occupied by one FAT: %d\n", x.BPB_FATSz32);
	printf("BPB_ExtFlags: 0x%x\n", x.BPB_ExtFlags);
	printf("Version number of volume: %d\n", x.BPB_FSVer);
	printf("Cluster number of root cluster: %d\n", x.BPB_RootClus);
	printf("FSInfo sector number: %d\n", x.BPB_FSInfo);
	printf("Sector number of boot record copy volume: %d\n", x.BPB_BkBootSec);
	//Not important: printf("BPB_Reserved: %s", x.BPB_Reserved);
	printf("Drive number: %x\n", x.BS_DrvNum);
	//Not important: printf("BS_Reserved1 %d\n", x.BS_Reserved1);
	printf("Boot signature: 0x%x\n", x.BS_BootSig);
	printf("Volume serial number: %u\n", x.BS_VolID);
	printf("Volume Label: ");
	for (i = 0; i < 11; i++)
	{
		printf("%c", x.BS_VolLab[i]);
	}
	printf("\nFile System Type: %s\n", x.BS_FilSysType);
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




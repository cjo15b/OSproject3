#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filesystem.h"

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
	unsigned int BPB_RootClus;		//Important: Root Cluser Number
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

typedef struct
{
    char DIR_Name[11];
    unsigned char DIR_Attr;
    unsigned char DIR_NTRes;
    unsigned char DIR_CrtTimeTenth;
    unsigned short DIR_CrtTime;
    unsigned short DIR_CrtDate;
    unsigned short DIR_LstAccDate;
    unsigned short DIR_FstClusHI;
    unsigned short DIR_WrtTime;
    unsigned short DIR_WrtDate;
    unsigned short DIR_FstClusLO;
    unsigned int DIR_FileSize;

} Directory;

//Declaring globals
BootBlock x;
unsigned int cluster_number;

void setBootBlock(char * FILENAME)
{
	FILE * fat32 = fopen(FILENAME, "rb+");
	// Reading in all data from Boot Sector
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
    fclose(fat32);
}
void initClusterNum (){
	cluster_number = x.BPB_RootClus;
}

void info(){
	int i;
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
	printf("Root cluster number %d\n", x.BPB_RootClus);
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

char* padDir(char* DIRNAME){
   int len = 11;
   char* token = strtok(DIRNAME, ".");
   char* name;
   char* extension;
   name = token;
   token = strtok(NULL, ".");
   if(token != NULL){
      extension = token;
      len = 8;
   }
   int i;
   for(i = strlen(name); i < len; i++){
      strcat(name, " ");
   }
   if(len == 8){
      for(i = strlen(extension); i < 3; i++){
         strcat(extension, " ");
      }
      strcat(name, extension);
   }
   return name;
}

unsigned int findCluster(char *FAT32, char *DIRNAME)
{
	FILE * fat32 = fopen(FAT32, "rb+");
	Directory y;
	unsigned int cluster = cluster_number;
	unsigned int current = 0;
	int nm, i = 1;
	//Always 0 for FAT32
	//unsigned int RootDirSectors = ((x.BPB_RootEntCnt * 32) + (x.BPB_BytsPerSec - 1)) / x.BPB_BytsPerSec;
	unsigned int FirstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	//Ends up being same as FirstDataSector
	unsigned int FirstSectorofCluster = ((x.BPB_RootClus - 2) * x.BPB_SecPerClus) + FirstDataSector * x.BPB_BytsPerSec;

	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
    {
        current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + FirstSectorofCluster;
        i = 1;
        while((i * 32) < x.BPB_BytsPerSec) {
        	fseek(fat32, current + (i * 32), SEEK_SET);
            fread(&y, 32, 1, fat32);
            for (nm = 0; nm < 11; nm++)
            {
               printf("%c == %c\n", y.DIR_Name[nm], DIRNAME[nm]);
            	if (y.DIR_Name[nm] != DIRNAME[nm])
            	{
            		break;
            	}
            	else if (y.DIR_Name[nm] == DIRNAME[nm] && nm == 10)
            	{
            		return (y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO);
            	}
            }
            i += 2;
        }
        fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
        fread(&cluster, sizeof(unsigned int), 1, fat32);
    }
    fclose(fat32);
	return cluster_number;
}

char* ls(char * FAT32, char* DIRNAME){
	Directory y;
   unsigned int cluster = x.BPB_RootClus;
   if(strcmp(DIRNAME, "/") == 0){
      cluster = x.BPB_RootClus;
   }else{
      cluster = findCluster(FAT32, padDir(DIRNAME));
   }
	unsigned int current = 0;
	FILE * fat32 = fopen(FAT32, "rb+");
	int i = 1;
	//Always 0 for FAT32
	//unsigned int RootDirSectors = ((x.BPB_RootEntCnt * 32) + (x.BPB_BytsPerSec - 1)) / x.BPB_BytsPerSec;
	unsigned int FirstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	//Ends up being same as FirstDataSector
	unsigned int FirstSectorofCluster = ((x.BPB_RootClus - 2) * x.BPB_SecPerClus) + FirstDataSector * x.BPB_BytsPerSec;
   
	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
    {
        current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + FirstSectorofCluster;
        i = 1;
        while((i * 32) < x.BPB_BytsPerSec) {
        	fseek(fat32, current + (i * 32), SEEK_SET);
            fread(&y, 32, 1, fat32);
            if(y.DIR_Name[0] == (char) 0xE5)
            {
            	i += 2;
            	continue;
            }
            //No entry after this one
            else if(y.DIR_Name[0] == 0x00)
                break;    
            else
            {	//If pos 8 isn't a space that means it has an extention
        		//A dot is implied
            	if(y.DIR_Name[8] != ' ')
            		y.DIR_Name[7] = '.';
                printf("%.*s\n", 11, y.DIR_Name);
            }
            i += 2;
        }
        fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
        fread(&cluster, sizeof(unsigned int), 1, fat32);
    }
    fclose(fat32);
	/*fseek(fat32, FirstSectorofCluster, SEEK_SET);
	fread(&y, 1, 32, fat32);
	printf("[0]: 0x%x\n", y.DIR_Name[0]);
	printf("Attr: %x\n", y.DIR_Attr);
	printf("DIRNAME: %s\n", y.DIR_Name);*/
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




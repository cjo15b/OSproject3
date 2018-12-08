#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "filesystem.h"

int statSize = 0;

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
	unsigned int BPB_RootClus;		//Important: Root Cluster Number
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
char* parentString = "..         ";


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
   free(opened);
	exit(0);
}

char* padDir(char* DIRNAME){
   int len = 11;
   char* token = strtok(DIRNAME, ".");
   char* name = malloc(11 * sizeof(char*));
   char* extension = malloc(3 * sizeof(char*));
   strcpy(name, token);
   token = strtok(NULL, ".");
   if(token != NULL){
	  strcpy(extension, token);
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
   free(extension);
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
				if (y.DIR_Name[nm] != DIRNAME[nm])
				{
					break;
				}
				else if (y.DIR_Name[nm] == DIRNAME[nm] && nm == 10)
				{
					fclose(fat32);
					if (y.DIR_Attr == 0x10)
				  {
						return (y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO);
					}
					else{
						printf("Error: File is not a Directory.\n");
						return cluster_number;
					}
				}
			}
			i += 2;
		}
		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}
	fclose(fat32);
	printf("Error: File doesn't exist.\n");
	return cluster_number;
}

void cd(char* FAT32, char* DIRNAME){
   Directory y;
   unsigned int cluster = cluster_number;
   if(strcmp(DIRNAME, "/") == 0){
	  cluster = x.BPB_RootClus;
   }else if(strcmp(DIRNAME, ".") == 0){
	  cluster = cluster_number;
   }else if(strcmp(DIRNAME, "..") == 0){
	  cluster = findCluster(FAT32, parentString);
	   if(cluster == 0){
		 cluster = x.BPB_RootClus;
		 }
   }else{
	  char* tempDir = padDir(DIRNAME);
	  cluster = findCluster(FAT32, tempDir);
	  free(tempDir);
   }
   cluster_number = cluster;
}

char* ls(char * FAT32, char* DIRNAME){
	Directory y;
   unsigned int current = 0;
   unsigned int cluster = cluster_number;
   if(strcmp(DIRNAME, "/") == 0){
	  cluster = x.BPB_RootClus;
   }else if(strcmp(DIRNAME, ".") == 0){
	  cluster = cluster_number;
   }else if(strcmp(DIRNAME, "..") == 0){
	  cluster = findCluster(FAT32, parentString);
	  if(cluster == 0){
		 cluster = x.BPB_RootClus;
	  }
	  if(cluster = cluster_number)
	  {
		return "Error";
	  }
   }else {
		char* tempDir = padDir(DIRNAME);
		unsigned int temp = findCluster(FAT32, tempDir);
		free(tempDir);
		if (temp == cluster_number)
		{

			return "Error";
		}
		else{
			cluster = temp;
		}
	  }           
	FILE * fat32 = fopen(FAT32, "rb+");
  printf(".\n");
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
			if(y.DIR_Name[0] == (char)0xE5)
			{
				printf("EQUALS 0xEF");
				i += 2;
				continue;
			}
			//No entry after this one empty y
			else if(y.DIR_Name[0] == 0x00){
				break;  
				}  
			else
			{	//If pos 8 isn't a space that means it has an extention
				//A dot is implied
				if(y.DIR_Name[8] != ' '){
					/*y.DIR_Name[7] = '.';*/
					for (int i = 0; i < 8; i++)
					{
						if(y.DIR_Name[i] != ' ')
							printf("%c", y.DIR_Name[i]);
					}
					printf(".");
					for (int i = 8; i < 11; i++)
					{
						if(y.DIR_Name[i] != ' ')
							printf("%c", y.DIR_Name[i]);
					}
					printf("\n");
				}
				else
					printf("%.*s\n", 11, y.DIR_Name);
			}
			i += 2;
		}
		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}
	fclose(fat32);
	return DIRNAME;
}

void size(char * FAT32, char* DIRNAME){	
	FILE * fat32 = fopen(FAT32, "rb+");
	Directory y;
	unsigned int cluster = cluster_number;
	unsigned int current = 0;
	int nm, i = 1;
	char* tempDir = padDir(DIRNAME);
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
				if (y.DIR_Name[nm] != tempDir[nm])
				{
					break;
				}
				else if (y.DIR_Name[nm] == tempDir[nm] && nm == 10)
				{
					fclose(fat32);
					if (y.DIR_Attr != 0x10)
					{
						printf("File Size: %d\n", y.DIR_FileSize);
						return;
					}
					else{
						printf("Error: Not a file.\n");
						return;
					}
				}
			}
			i += 2;
		}
		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}
	fclose(fat32);
	printf("Error: File doesn't exist.\n");
}

int nameExists(char * DIRNAME)
{
	return 0;
}

unsigned int findAvailableCluster(char* FAT32)
{
	FILE *fat32 = fopen(FAT32, "rb+");
	int i = 0;
	unsigned int current;
	unsigned int dataSec = x.BPB_TotSec32 + (x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32));
	unsigned int clusterCount = dataSec / (x.BPB_SecPerClus * x.BPB_BytsPerSec);


	while(i < clusterCount)
	{
		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (i * sizeof(int)), SEEK_SET);
		fread(&current, sizeof(unsigned int), 1, fat32);
		if(current == 0x00000000){
			fclose(fat32);
			return i;
		}
		++i;
	}
	fclose(fat32);
	return -1;

}

void mkdir(char* FAT32, char *DIRNAME)
{
   unsigned int cluster = cluster_number;
	int i = 1, j= 0;
	unsigned int current = 0, newCluster = 0;
	unsigned int parentClust = cluster;
	unsigned int lastClust = cluster;

	unsigned short datePlace, date;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	Directory y, ourDir, ourCurDir, ourParDir;


	unsigned int nextWrite = findAvailableCluster(FAT32), tempEmpty;
	unsigned int setUsed = 0x0FFFFFF8;
	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);

	FILE *fat32 = fopen(FAT32, "rb+");

	firstDataSector *= x.BPB_BytsPerSec;


	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec)
		{
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);
			if(y.DIR_Name[0] == 0x00 || y.DIR_Name[0] == (char) 0xE5)
			{
				fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextWrite * sizeof(int)), SEEK_SET);
				fwrite(&setUsed, sizeof(int), 1, fat32);

				memset(&ourDir, 0, 32);
				for(int j = 0; j < 11; ++j)
				{
					if(j >= strlen(DIRNAME))
					{
						ourDir.DIR_Name[j] = 0x20;
					} else {
						ourDir.DIR_Name[j] = DIRNAME[j];
					}
				}
				ourDir.DIR_Attr = 0x10;
				ourDir.DIR_FstClusHI = nextWrite/0x100;
				ourDir.DIR_FstClusLO = nextWrite%0x100;
				ourDir.DIR_WrtDate = tm.tm_mday;
				datePlace = tm.tm_mon << 5;

				ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;
				datePlace = 0;
				datePlace = (tm.tm_year - 80)<< 9;
				ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;

				datePlace = 0;
				ourDir.DIR_WrtTime = tm.tm_sec/2;
				datePlace = tm.tm_min << 5;
				ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;
				datePlace = 0;
				datePlace = tm.tm_hour << 11;
				ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;


				fseek(fat32, current + (i * 32), SEEK_SET);
				fwrite(&ourDir, 32, 1, fat32);


				current = ((nextWrite - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));
				memset(&ourCurDir, 0, 32);
				for(j = 0; j < 11; ++j)
				{
					if(j==0){
						ourCurDir.DIR_Name[0] = '.';
					} else {
						ourCurDir.DIR_Name[j] = ' ';
					}
				}
				ourCurDir.DIR_Attr = 0x10;
				ourCurDir.DIR_FstClusLO = ourDir.DIR_FstClusLO;
				ourCurDir.DIR_FstClusHI = ourDir.DIR_FstClusHI;

				fseek(fat32, current, SEEK_SET);
				fwrite(&ourCurDir, 32, 1, fat32);

				memset(&ourParDir, 0, 32);

				for(j = 0; j < 11; ++j)
				{
					if(j < 2){
						ourParDir.DIR_Name[j] = '.';
					} else {
						ourParDir.DIR_Name[j] = ' ';
					}
				}
				ourParDir.DIR_Attr = 0x10;
				ourParDir.DIR_FstClusHI = parentClust / 0x100;
				ourParDir.DIR_FstClusLO = parentClust % 0x100;

				fseek(fat32, current + 32, SEEK_SET);
				fwrite(&ourParDir, 32, 1, fat32);


				fclose(fat32);
				return; //fuck
			}
			for(j = 0; j < 11; ++j)
			{
				if(y.DIR_Name[j] == 0x20)
				{
					y.DIR_Name[j] = '\0';
					break;
				}
			}
			if(strcmp(y.DIR_Name, DIRNAME) == 0)
			{
				printf("Error, the directory you entered already exists.\n");
				return;
			}
			i += 2;
		} 


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);

		if(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
			lastClust = cluster;
	}

	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (lastClust * sizeof(int)), SEEK_SET);
	fwrite(&nextWrite, sizeof(unsigned int), 1, fat32);

	//for(i = 0; i < 2; ++i){
	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + ((nextWrite) * sizeof(int)), SEEK_SET);
	fwrite(&setUsed, sizeof(unsigned int), 1, fat32);

	//fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + ((nextWrite+1) * sizeof(int)), SEEK_SET);
	//fwrite(&setUsed, sizeof(unsigned int), 1, fat32);
	//}
	fclose(fat32);
	tempEmpty = findAvailableCluster(FAT32);
	fat32 = fopen(FAT32, "rb+");

	current = ((nextWrite - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));

	memset(&ourDir, 0, 32);
	for(int j = 0; j < 11; ++j)
	{
		if(j >= strlen(DIRNAME))
		{
			ourDir.DIR_Name[j] = 0x20;
		} else {
			ourDir.DIR_Name[j] = DIRNAME[j];
		}
	}


	ourDir.DIR_Attr = 0x10;
	ourDir.DIR_FstClusHI = tempEmpty/0x100;
	ourDir.DIR_FstClusLO = tempEmpty%0x100;
	ourDir.DIR_WrtDate = tm.tm_mday;
	datePlace = tm.tm_mon << 5;

	ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;
	datePlace = (tm.tm_year - 80)<< 9;
	ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;

	datePlace = 0;
	ourDir.DIR_WrtTime = tm.tm_sec/2;
	datePlace = tm.tm_min << 5;
	ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;
	datePlace = 0;
	datePlace = tm.tm_hour << 11;
	ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;


	fseek(fat32, current + 32, SEEK_SET);
	fwrite(&ourDir, 32, 1, fat32);

	nextWrite = tempEmpty;

	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextWrite * sizeof(int)), SEEK_SET);
	fwrite(&setUsed, sizeof(int), 1,fat32);

	current = ((nextWrite - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));
	memset(&ourCurDir, 0, 32);
	for(j = 0; j < 11; ++j)
	{
		if(j==0){
			ourCurDir.DIR_Name[0] = '.';
		} else {
			ourCurDir.DIR_Name[j] = ' ';
		}
	}
	ourCurDir.DIR_Attr = 0x10;
	ourCurDir.DIR_FstClusLO = nextWrite / 0x100;
	ourCurDir.DIR_FstClusHI = nextWrite % 0x100;

	fseek(fat32, current, SEEK_SET);
	fwrite(&ourCurDir, 32, 1, fat32);

	memset(&ourParDir, 0, 32);

	for(j = 0; j < 11; ++j)
	{
		if(j < 2){
			ourParDir.DIR_Name[j] = '.';
		} else {
			ourParDir.DIR_Name[j] = ' ';
		}
	}
	ourParDir.DIR_Attr = 0x10;
	ourParDir.DIR_FstClusHI = parentClust / 0x100;
	ourParDir.DIR_FstClusLO = parentClust % 0x100;

	fseek(fat32, current + 32, SEEK_SET);
	fwrite(&ourParDir, 32, 1, fat32);

	fclose(fat32);
}

void creat(char* FAT32, char* FILENAME)
{
	unsigned int cluster = cluster_number;
	int i = 1, j= 0;
	unsigned int current = 0, newCluster = 0;
	unsigned int parentClust = cluster;
	unsigned int lastClust = cluster;

	unsigned short datePlace, date;

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	Directory y, ourDir, ourCurDir, ourParDir;


	unsigned int nextWrite = findAvailableCluster(FAT32), tempEmpty;
	unsigned int setUsed = 0x0FFFFFF8;
	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);

	FILE *fat32 = fopen(FAT32, "rb+");

	firstDataSector *= x.BPB_BytsPerSec;


	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec){
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);
			if(y.DIR_Name[0] == 0x00 || y.DIR_Name[0] == (char) 0xE5)
			{
				fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextWrite * sizeof(int)), SEEK_SET);
				fwrite(&setUsed, sizeof(int), 1, fat32);

				memset(&ourDir, 0, 32);
				char* padded = padDir(FILENAME);
				strcpy(ourDir.DIR_Name, padded);
				free(padded);
				ourDir.DIR_FstClusHI = nextWrite/0x100;
				ourDir.DIR_FstClusLO = nextWrite%0x100;
				ourDir.DIR_WrtDate = tm.tm_mday;
				datePlace = tm.tm_mon << 5;

				ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;
				datePlace = 0;
				datePlace = (tm.tm_year - 80)<< 9;
				ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;

				datePlace = 0;
				ourDir.DIR_WrtTime = tm.tm_sec/2;
				datePlace = tm.tm_min << 5;
				ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;
				datePlace = 0;
				datePlace = tm.tm_hour << 11;
				ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;


				fseek(fat32, current + (i * 32), SEEK_SET);
				fwrite(&ourDir, 32, 1, fat32);

				//printf("%x\n", nextWrite);


				fclose(fat32);
				return; //fuck
			}
			for(j = 0; j < 11; ++j)
			{
				if(y.DIR_Name[j] == 0x20)
				{
					y.DIR_Name[j] = '\0';
					break;
				}
			}
			if(strcmp(y.DIR_Name, FILENAME) == 0)
			{
				printf("Error, the directory you entered already exists.\n");
				return;
			}
			i += 2;
		}


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);

		if(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
			lastClust = cluster;
	}

	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (lastClust * sizeof(int)), SEEK_SET);
	fwrite(&nextWrite, sizeof(unsigned int), 1, fat32);

	//for(i = 0; i < 2; ++i){
	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + ((nextWrite) * sizeof(int)), SEEK_SET);
	fwrite(&setUsed, sizeof(unsigned int), 1, fat32);

	fclose(fat32);
	tempEmpty = findAvailableCluster(FAT32);
	fat32 = fopen(FAT32, "rb+");

	current = ((nextWrite - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;

	memset(&ourDir, 0, 32);
	char* padded = padDir(FILENAME);
	strcpy(ourDir.DIR_Name, padded);
	free(padded);

	ourDir.DIR_FstClusHI = tempEmpty/0x100;
	ourDir.DIR_FstClusLO = tempEmpty%0x100;
	ourDir.DIR_WrtDate = tm.tm_mday;
	datePlace = tm.tm_mon << 5;

	ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;
	datePlace = (tm.tm_year - 80)<< 9;
	ourDir.DIR_WrtDate = ourDir.DIR_WrtDate | (unsigned short) datePlace;

	datePlace = 0;
	ourDir.DIR_WrtTime = tm.tm_sec/2;
	datePlace = tm.tm_min << 5;
	ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;
	datePlace = 0;
	datePlace = tm.tm_hour << 11;
	ourDir.DIR_WrtTime = ourDir.DIR_WrtTime | datePlace;


	fseek(fat32, current + 32, SEEK_SET);
	fwrite(&ourDir, 32, 1, fat32);

	nextWrite = tempEmpty;

	fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextWrite * sizeof(int)), SEEK_SET);
	fwrite(&setUsed, sizeof(int), 1,fat32);

	fclose(fat32);
}

void openMyFile(char* FAT32,  char* FILENAME, char* mode)
{
	int i = 1, j;
	unsigned int current = 0;
	unsigned int cluster = cluster_number;
	Directory y;
	Status fs;
	int * openSize = &statSize;
	FILE *fat32 = fopen(FAT32, "rb+");

	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	firstDataSector *= x.BPB_BytsPerSec;

	// check that the mode is valid

	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec);
		{
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);
			char* tempName = (char*) malloc(12);
			tempName[11] = '\0';
			if(y.DIR_Name[8] != ' ')
			   y.DIR_Name[7] = '.';
			int j = 0;
			for(j; j < 11; ++j) {
			   if( y.DIR_Name[j] == 0x20) {
				  tempName[j] = '\0';
				  break;
			   }
			   else
				 tempName[j] = y.DIR_Name[j];
			}
			// if we found the name and it isn't a directory
			if(strncmp(tempName, FILENAME, 11) == 0 && y.DIR_Attr != 0x10)
			{
				fs.cluster = y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO;
				// look through open file list to see if it is already open
				for (int l = 0; l < (*openSize); ++l) {
					// if we find the file in the list
					if (opened[l].cluster == fs.cluster) {
						printf("ERROR: This file is already open.\n");
						return;
					}
				}
				// if read only and chosen mode is read
				if((y.DIR_Attr & 0x01) == 0x01 && (strcmp(mode, "r") == 0 || strcmp(mode, "R") == 0)) // read only
				{
					fs.mode = 0;
				}
					// else if not read only
				else if((y.DIR_Attr & 0x01) != 0x01)
				{
					if(strcmp(mode, "r") == 0 || strcmp(mode, "R") == 0)
					{
						fs.mode = 0;
					} else if(strcmp(mode, "w") == 0 || strcmp(mode, "W") == 0){
						fs.mode = 1;
					} else if(strcmp(mode, "rw") == 0 || strcmp(mode, "wr") == 0
							  || strcmp(mode, "RW") == 0 || strcmp(mode, "WR") == 0)
					{
						fs.mode = 2;
					} else {
						printf("ERROR: Invalid mode.\n");
						return;
					}
				} else {
					// if we get here, then the user tried to open a read only file in a write mode
					printf("ERROR: Cannot open a read-only file in write mode.\n");
					return;
				}
				opened[(*openSize)++] = fs;
				return;
			}
			i += 2;
		} 


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}

	fclose(fat32);
}

void closeMyFile(char* FAT32, char* FILENAME)
{
	int i = 1, j;
	unsigned int current = 0;
	unsigned int cluster = cluster_number;
	Directory y;
	Status fs;
	int * openSize = &statSize;
	FILE *fat32 = fopen(FAT32, "rb+");

	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	firstDataSector *= x.BPB_BytsPerSec;

	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec){
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);
			char* tempName = (char*) malloc(12);
			tempName[11] = '\0';
			if(y.DIR_Name[8] != ' ')
			   y.DIR_Name[7] = '.';
			int j = 0;
			for(j; j < 11; ++j) {
			   if( y.DIR_Name[j] == 0x20) {
				  tempName[j] = '\0';
				  break;
			   }
			   else
				 tempName[j] = y.DIR_Name[j];
			}
			// if we found the name and it isn't a directory
			if(strncmp(tempName, FILENAME, 11) == 0 && y.DIR_Attr != 0x10)
			{
				fs.cluster = y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO;
				// look through open file list to see if it is already open
				for (int l = 0; l < (*openSize); ++l) {
					// if we find the file in the list
					if (opened[l].cluster == fs.cluster) {
						// move each element of the array after back one to remove from opened
						for (int m = l; m < (*openSize)-1; ++m) {
							opened[m] = opened[m+1];
						}
						// decrement the size
						--(*openSize);
						return;
					}
				}
			}
			i += 2;
		} 


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}

	fclose(fat32);
}

void readMyFile(char *FAT32, char *FILENAME, char* OFFSET, char* SIZE)
{
	int i = 1, j, k;
	int off = atoi(OFFSET), size = atoi(SIZE);
	unsigned int current = 0;
	unsigned int cluster = cluster_number;
	Directory y;
	Status fs;
	FILE *fat32 = fopen(FAT32, "rb+");
	char *output;
	unsigned int firstClus = off / x.BPB_BytsPerSec;
	unsigned int startPos = off % x.BPB_BytsPerSec;
	unsigned int currSize = 0;
	char* tempName = (char*) malloc(12);;

	// go to first reserved sector
	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	firstDataSector *= x.BPB_BytsPerSec;

	output = (char *) malloc(size);

	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec);
		{
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);

			tempName[11] = '\0';
			if(y.DIR_Name[8] != ' ')
			   y.DIR_Name[7] = '.';
			int j = 0;
			for(j; j < 11; ++j) {
			   if( y.DIR_Name[j] == 0x20) {
				  tempName[j] = '\0';
				  break;
			   }
			   else
				 tempName[j] = y.DIR_Name[j];
			}

			// if we found the name and it isn't a directory
			if(strcmp(tempName, FILENAME) == 0 && y.DIR_Attr != 0x10 && y.DIR_FileSize != 0)
			{
				cluster = y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO;
				fs.cluster = cluster;
				// look through open file list to see if it is already open
				for (int l = 0; l < statSize; ++l) {
					// if we find the file in the open file array, and if the mode allows reading
					if (opened[l].cluster == fs.cluster && (opened[l].mode == 0 || opened[l].mode == 2)) {

						if(off > y.DIR_FileSize)
						{
							printf("ERROR: The offset %d exceeds the file size %d\n",off,y.DIR_FileSize);
							return;
						}
						j = 0;
						while(currSize < size)
						{
							current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
							if(j >= firstClus)
							{
								output += currSize;
								k = x.BPB_BytsPerSec - startPos;
								if(k > size-currSize)
									k = size-currSize;

								fseek(fat32, current + startPos, SEEK_SET);
								fread(output, sizeof(char), k, fat32);

								output -= currSize;
								currSize += k;
								startPos = 0;
							}
							fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
							fread(&cluster, sizeof(unsigned int), 1, fat32);
							++j;
						}
						printf("%.*s\n", size, output);
						free(output);
						free(tempName);
						fclose(fat32);
						return;
					}
						// else if we do find the file but it does not allow reading
					else if (opened[l].cluster == fs.cluster && (opened[l].mode != 0 || opened[l].mode != 2)) {
						printf("ERROR: The file is not open in read or read/write mode.\n");
						free(output);
						free(tempName);
						fclose(fat32);
						return;
					}
				}
				// if we got here, then the file is not open
				printf("ERROR: The file is not open.\n");
				free(output);
				free(tempName);
				fclose(fat32);
				return;
			}
			free(tempName);
			i += 2;
		} 


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}

	free(output);
	fclose(fat32);
}

void writeMyFile(char *FAT32, char *FILENAME, char* OFFSET, char* SIZE, char* STRING)
{
	int i = 1, j, k;
	int off = atoi(OFFSET), size = atoi(SIZE);
	unsigned int current = 0;
	unsigned int cluster = cluster_number;
	unsigned int nClust = 0;
	unsigned int tempClust = 0;
	unsigned int zeroo = 0;
	unsigned int ending = 0x0FFFFFF8;
	unsigned int dirClust = 0;
	Directory y;
	Status fs;
	FILE *fat32 = fopen(FAT32, "rb+");
	char *input, *zeroes;
	unsigned int firstClus = off / x.BPB_BytsPerSec;
	unsigned int startPos = off % x.BPB_BytsPerSec;
	unsigned int currSize = 0;
	char* tempName = (char*) malloc(12);;
	int inputStrLen = (int)strlen(STRING);

	// go to first reserved sector
	unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
	firstDataSector *= x.BPB_BytsPerSec;

	input = (char *) malloc(size);
	// do this instead of strcopy to not have it nullterminated.
	memset(input, 0, sizeof(char) * size);
	for(j = 0; j < strlen(STRING); ++j)
	{
		input[j] = STRING[j];
	}

	zeroes = (char*) malloc(512);
	memset(zeroes, 0, 512);


	while(cluster != 0x0FFFFFF8 && cluster != 0x0FFFFFFF)
	{
		current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
		i = 1;
		while((i * 32) < x.BPB_BytsPerSec){
			fseek(fat32, current + (i * 32), SEEK_SET);
			fread(&y, 32, 1, fat32);

			tempName[11] = '\0';
			if(y.DIR_Name[8] != ' ')
			   y.DIR_Name[7] = '.';
			int j = 0;
			for(j; j < 11; ++j) {
			   if( y.DIR_Name[j] == 0x20) {
				  tempName[j] = '\0';
				  break;
			   }
			   else
				 tempName[j] = y.DIR_Name[j];
			}

			// if we found the name and it isn't a directory
			if(strcmp(tempName, FILENAME) == 0 && y.DIR_Attr != 0x10)
			{
				dirClust = current + (i*32);
				cluster = ((unsigned int)y.DIR_FstClusHI) * 0x100 + ((unsigned int)y.DIR_FstClusLO);
				fs.cluster = cluster;
				// look through open file list to see if it is already open
				for (int i = 0; i < statSize; ++i) {
					// if we find the file in the open file array, and if the mode allows writing
					if (opened[i].cluster == fs.cluster && (opened[i].mode == 1 || opened[i].mode == 2)) {
						// check that the offset doesn't exceed the file size
						/*
						if(off > y.DIR_FileSize)
						{
							printf("ERROR: The offset %d exceeds the file size %d\n",off,y.DIR_FileSize);
							return;
						}*/


						j = 0;

						// while the current byte does not exceed the given size to write
						while(currSize < size)
						{
							if(cluster == 0x0FFFFFFF || cluster == 0x0FFFFFF8)
							{
								fclose(fat32);
								nClust = findAvailableCluster(FAT32);
								fat32 = fopen(FAT32, "rb+");
								fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (tempClust * sizeof(int)), SEEK_SET);
								fwrite(&nClust, sizeof(int), 1, fat32);
								
								fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nClust * sizeof(int)), SEEK_SET);
								fwrite(&ending, sizeof(int), 1, fat32);
								cluster = nClust;
							}

							current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;

							if(j >= firstClus)
							{
								input += currSize;
								k = x.BPB_BytsPerSec - startPos;
								if(k > size-currSize)
								{
									k = size-currSize;
									fseek(fat32, current + startPos, SEEK_SET);
									fwrite(input, sizeof(char), k, fat32);

									fseek(fat32, current + startPos + k, SEEK_SET);
									fwrite(zeroes, sizeof(char), 512-k-startPos, fat32);
								} else {

									fseek(fat32, current + startPos, SEEK_SET);
									fwrite(input, sizeof(char), k, fat32);
								}
								input -= currSize;
								currSize += k;
								startPos = 0;
							}

							tempClust = cluster;
							fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
							fread(&cluster, sizeof(unsigned int), 1, fat32);
							++j;
						}

						// if given size to write is less than the total file's size
						if (off + size < y.DIR_FileSize) {
							if(cluster == 0x0FFFFFFF || cluster == 0x0FFFFFF8)
							{
								k = (currSize % 512);
								fseek(fat32, current + k, SEEK_SET);
								fwrite(zeroes, sizeof(char), 512 - k, fat32);
							} else if(cluster != 0x0FFFFFFF && cluster != 0x0FFFFFF8)
							{
								fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (tempClust * sizeof(int)), SEEK_SET);
								fwrite(&ending, sizeof(int), 1, fat32);
							}
							while(cluster != 0x0FFFFFFF && cluster != 0x0FFFFFF8)
							{
								current = ((cluster - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
								k = (currSize % 512);
								fseek(fat32, current + k, SEEK_SET);
								fwrite(zeroes, sizeof(int), 512-k, fat32);
								currSize = 0;
								tempClust = cluster;
								fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
								fread(&cluster, sizeof(int), 1, fat32);

								fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (tempClust * sizeof(int)), SEEK_SET);
								fwrite(&zeroo, sizeof(int), 1, fat32);

								if(cluster == 0x0FFFFFFF || cluster == 0x0FFFFFF8)
								{
									fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
									fwrite(&zeroo, sizeof(int), 1, fat32);
								}
							}
							y.DIR_FileSize = (firstClus * x.BPB_BytsPerSec) + (off%x.BPB_BytsPerSec) + size;
						} else {
							y.DIR_FileSize = off + size;
						}


						fseek(fat32, dirClust, SEEK_SET);
						fwrite(&y, 32, 1, fat32);

						free(input);
						free(tempName);
						fclose(fat32);
						return;
					}
					else if (opened[i].cluster == fs.cluster && (opened[i].mode != 1 && opened[i].mode != 2)) {
						printf("ERROR: The file is not open in read or read/write mode.\n");
						free(input);
						free(tempName);
						fclose(fat32);
						return;
					}
				}
				printf("ERROR: The file is not open.\n");
			}
			free(tempName);
			i += 2;
		} 


		fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
		fread(&cluster, sizeof(unsigned int), 1, fat32);
	}

	free(input);
	fclose(fat32);
}

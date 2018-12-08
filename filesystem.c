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

void creat(char* FAT32, char* FILENAME){
   FILE * fat32 = fopen(FAT32, "rb+");
	Directory y;
	unsigned int cluster = cluster_number;
	unsigned int current = 0;
	int nm, i = 1;
   int exists = 0;
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
            	if (y.DIR_Name[nm] != FILENAME[nm])
            	{
            		break;
            	}
            	else if (y.DIR_Name[nm] == FILENAME[nm] && nm == 10)
            	{
                  exists = 1;
            	}
            }
            i += 2;
        }
        fseek(fat32, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (cluster * sizeof(int)), SEEK_SET);
        fread(&cluster, sizeof(unsigned int), 1, fat32);
    }
    if(exists == 0){
       
    }
    fclose(fat32);
}

void OurExit(char *ptr){
	printf("Exiting...\n");
	free(ptr);
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
            		printf("File HI: 0x%x\nFile Lo: 0x%x\n", y.DIR_FstClusHI, y.DIR_FstClusLO);
            		printf("File first bit: 0x%x\n", y.DIR_Name[0]);
            		printf("File attribute: 0x%x\n", y.DIR_Attr);
            		printf("Sent Cluster %x\n", (y.DIR_FstClusHI * 0x100 + y.DIR_FstClusLO));
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
            if(y.DIR_Name[0] == 0xE5)
            {
            	printf("EQUALS 0xEF");
            	i += 2;
            	continue;
            }
            //No entry after this one
            else if(y.DIR_Name[0] == 0x00){
            	printf("Empty Directory\n");
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

unsigned int findEmptyCluster(char* FAT32)
{
    FILE *fp = fopen(FAT32, "rb+");
    int i = 0;
    unsigned int currClust;
    unsigned int dataSec = x.BPB_TotSec32 + (x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32));
    unsigned int clusterCount = dataSec / (x.BPB_SecPerClus * x.BPB_BytsPerSec);


    while(i < clusterCount)
    {
        fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (i * sizeof(int)), SEEK_SET);
        fread(&currClust, sizeof(unsigned int), 1, fp);
        if(currClust == 0x00000000){
            fclose(fp);
            return i;
        }
        ++i;
    }
    fclose(fp);
    return -1;

}

void mkdir(char* FAT32, char *dirname)
{
   unsigned int clust_num = cluster_number;
    int i = 1, j= 0;
    unsigned int currClust = 0, newCluster = 0;
    unsigned int parentClust = clust_num;
    unsigned int lastClust = clust_num;

    unsigned short holder, date;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Directory dir, newDir, dotDir, dotdotDir;


    unsigned int nextEmpty = findEmptyCluster(FAT32), tempEmpty;
    unsigned int usedClust = 0x0FFFFFF8;
    unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);

    FILE *fp = fopen(FAT32, "rb+");

    firstDataSector *= x.BPB_BytsPerSec;


    while(clust_num != 0x0FFFFFF8 && clust_num != 0x0FFFFFFF)
    {
        currClust = ((clust_num - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
        i = 1;
        do{
            fseek(fp, currClust + (i * sizeof(Directory)), SEEK_SET);
            fread(&dir, sizeof(Directory), 1, fp);
            if(dir.DIR_Name[0] == 0x00 || dir.DIR_Name[0] == (char) 0xE5)
            {
                fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextEmpty * sizeof(int)), SEEK_SET);
                fwrite(&usedClust, sizeof(int), 1, fp);

                memset(&newDir, 0, sizeof(Directory));
                for(int j = 0; j < 11; ++j)
                {
                    if(j >= strlen(dirname))
                    {
                        newDir.DIR_Name[j] = 0x20;
                    } else {
                        newDir.DIR_Name[j] = dirname[j];
                    }
                }
                newDir.DIR_Attr = 0x10;
                newDir.DIR_FstClusHI = nextEmpty/0x100;
                newDir.DIR_FstClusLO = nextEmpty%0x100;
                newDir.DIR_WrtDate = tm.tm_mday;
                holder = tm.tm_mon << 5;

                newDir.DIR_WrtDate = newDir.DIR_WrtDate | (unsigned short) holder;
                holder = 0;
                holder = (tm.tm_year - 80)<< 9;
                newDir.DIR_WrtDate = newDir.DIR_WrtDate | (unsigned short) holder;

                holder = 0;
                newDir.DIR_WrtTime = tm.tm_sec/2;
                holder = tm.tm_min << 5;
                newDir.DIR_WrtTime = newDir.DIR_WrtTime | holder;
                holder = 0;
                holder = tm.tm_hour << 11;
                newDir.DIR_WrtTime = newDir.DIR_WrtTime | holder;


                fseek(fp, currClust + (i * sizeof(Directory)), SEEK_SET);
                fwrite(&newDir, sizeof(Directory), 1, fp);


                currClust = ((nextEmpty - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));
                memset(&dotDir, 0, sizeof(Directory));
                for(j = 0; j < 11; ++j)
                {
                    if(j==0){
                        dotDir.DIR_Name[0] = '.';
                    } else {
                        dotDir.DIR_Name[j] = ' ';
                    }
                }
                dotDir.DIR_Attr = 0x10;
                dotDir.DIR_FstClusLO = newDir.DIR_FstClusLO;
                dotDir.DIR_FstClusHI = newDir.DIR_FstClusHI;

                fseek(fp, currClust, SEEK_SET);
                fwrite(&dotDir, sizeof(Directory), 1, fp);

                memset(&dotdotDir, 0, sizeof(Directory));

                for(j = 0; j < 11; ++j)
                {
                    if(j < 2){
                        dotdotDir.DIR_Name[j] = '.';
                    } else {
                        dotdotDir.DIR_Name[j] = ' ';
                    }
                }
                dotdotDir.DIR_Attr = 0x10;
                dotdotDir.DIR_FstClusHI = parentClust / 0x100;
                dotdotDir.DIR_FstClusLO = parentClust % 0x100;

                fseek(fp, currClust + sizeof(Directory), SEEK_SET);
                fwrite(&dotdotDir, sizeof(Directory), 1, fp);


                fclose(fp);
                return; //fuck
            }
            for(j = 0; j < 11; ++j)
            {
                if(dir.DIR_Name[j] == 0x20)
                {
                    dir.DIR_Name[j] = '\0';
                    break;
                }
            }
            if(strcmp(dir.DIR_Name, dirname) == 0)
            {
                printf("Error, the directory you entered already exists.\n");
                return;
            }
            i += 2;
        } while((i * sizeof(Directory)) < x.BPB_BytsPerSec);


        fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (clust_num * sizeof(int)), SEEK_SET);
        fread(&clust_num, sizeof(unsigned int), 1, fp);

        if(clust_num != 0x0FFFFFF8 && clust_num != 0x0FFFFFFF)
            lastClust = clust_num;
    }

    fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (lastClust * sizeof(int)), SEEK_SET);
    fwrite(&nextEmpty, sizeof(unsigned int), 1, fp);

    //for(i = 0; i < 2; ++i){
    fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + ((nextEmpty) * sizeof(int)), SEEK_SET);
    fwrite(&usedClust, sizeof(unsigned int), 1, fp);

    //fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + ((nextEmpty+1) * sizeof(int)), SEEK_SET);
    //fwrite(&usedClust, sizeof(unsigned int), 1, fp);
    //}
    fclose(fp);
    tempEmpty = findEmptyCluster(FAT32);
    fp = fopen(FAT32, "rb+");

    currClust = ((nextEmpty - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));

    memset(&newDir, 0, sizeof(Directory));
    for(int j = 0; j < 11; ++j)
    {
        if(j >= strlen(dirname))
        {
            newDir.DIR_Name[j] = 0x20;
        } else {
            newDir.DIR_Name[j] = dirname[j];
        }
    }


    newDir.DIR_Attr = 0x10;
    newDir.DIR_FstClusHI = tempEmpty/0x100;
    newDir.DIR_FstClusLO = tempEmpty%0x100;
    newDir.DIR_WrtDate = tm.tm_mday;
    holder = tm.tm_mon << 5;

    newDir.DIR_WrtDate = newDir.DIR_WrtDate | (unsigned short) holder;
    holder = (tm.tm_year - 80)<< 9;
    newDir.DIR_WrtDate = newDir.DIR_WrtDate | (unsigned short) holder;

    holder = 0;
    newDir.DIR_WrtTime = tm.tm_sec/2;
    holder = tm.tm_min << 5;
    newDir.DIR_WrtTime = newDir.DIR_WrtTime | holder;
    holder = 0;
    holder = tm.tm_hour << 11;
    newDir.DIR_WrtTime = newDir.DIR_WrtTime | holder;


    fseek(fp, currClust + sizeof(Directory), SEEK_SET);
    fwrite(&newDir, sizeof(Directory), 1, fp);

    nextEmpty = tempEmpty;

    fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (nextEmpty * sizeof(int)), SEEK_SET);
    fwrite(&usedClust, sizeof(int), 1,fp);

    currClust = ((nextEmpty - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector; //- (sizeof(FAT32Directory));
    memset(&dotDir, 0, sizeof(Directory));
    for(j = 0; j < 11; ++j)
    {
        if(j==0){
            dotDir.DIR_Name[0] = '.';
        } else {
            dotDir.DIR_Name[j] = ' ';
        }
    }
    dotDir.DIR_Attr = 0x10;
    dotDir.DIR_FstClusLO = nextEmpty / 0x100;
    dotDir.DIR_FstClusHI = nextEmpty % 0x100;

    fseek(fp, currClust, SEEK_SET);
    fwrite(&dotDir, sizeof(Directory), 1, fp);

    memset(&dotdotDir, 0, sizeof(Directory));

    for(j = 0; j < 11; ++j)
    {
        if(j < 2){
            dotdotDir.DIR_Name[j] = '.';
        } else {
            dotdotDir.DIR_Name[j] = ' ';
        }
    }
    dotdotDir.DIR_Attr = 0x10;
    dotdotDir.DIR_FstClusHI = parentClust / 0x100;
    dotdotDir.DIR_FstClusLO = parentClust % 0x100;

    fseek(fp, currClust + sizeof(Directory), SEEK_SET);
    fwrite(&dotdotDir, sizeof(Directory), 1, fp);

    fclose(fp);
}


void open(char* FAT32,  char* FILENAME, char* mode)
{
    int i = 1, j;
    unsigned int currClust = 0;
    unsigned int trackClust = cluster_number;
    Directory dir;
    Status fs;
    int * openSize = &statSize;
    FILE *fp = fopen(FAT32, "rb+");

    unsigned int firstDataSector = x.BPB_RsvdSecCnt + (x.BPB_NumFATs * x.BPB_FATSz32);
    firstDataSector *= x.BPB_BytsPerSec;

    // check that the mode is valid

    while(trackClust != 0x0FFFFFF8 && trackClust != 0x0FFFFFFF)
    {
        currClust = ((trackClust - 2) * (x.BPB_SecPerClus * x.BPB_BytsPerSec)) + firstDataSector;
        i = 1;
        do{
            fseek(fp, currClust + (i * sizeof(Directory)), SEEK_SET);
            fread(&dir, sizeof(Directory), 1, fp);
            char* tempName = (char*) malloc(12);
            tempName[11] = '\0';
            if(dir.DIR_Name[8] != ' ')
               dir.DIR_Name[7] = '.';
            int j = 0;
            for(j; j < 11; ++j) {
               if( dir.DIR_Name[j] == 0x20) {
                  tempName[j] = '\0';
                  break;
               }
               else
                 tempName[j] = dir.DIR_Name[j];
            }
            // if we found the name and it isn't a directory
            if(strncmp(tempName, FILENAME, 11) == 0 && dir.DIR_Attr != 0x10)
            {
                fs.clust_num = dir.DIR_FstClusHI * 0x100 + dir.DIR_FstClusLO;
                // look through open file list to see if it is already open
                for (int l = 0; l < (*openSize); ++l) {
                    // if we find the file in the list
                    if (opened[l].clust_num == fs.clust_num) {
                        printf("ERROR: This file is already open.\n");
                        return;
                    }
                }
                // if read only and chosen mode is read
                if((dir.DIR_Attr & 0x01) == 0x01 && (strcmp(mode, "r") == 0 || strcmp(mode, "R") == 0)) // read only
                {
                    fs.mode = 0;
                }
                    // else if not read only
                else if((dir.DIR_Attr & 0x01) != 0x01)
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
        } while((i * sizeof(Directory)) < x.BPB_BytsPerSec);


        fseek(fp, (x.BPB_RsvdSecCnt * x.BPB_BytsPerSec) + (trackClust * sizeof(int)), SEEK_SET);
        fread(&trackClust, sizeof(unsigned int), 1, fp);
    }

    fclose(fp);
}



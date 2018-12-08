//system_main.c

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filesystem.h"

int main (int argc, const char* argv[]){		//this will be our main filesystem program.
	FILE * fat32;
	char *ptr = NULL;
   char input[256];
	char command[256];
	char cwd[256];
	char hostname[1024];
   char arg[256];
	int ls_flag = 0;
   
   
   opened = (Status *) malloc(255 * sizeof(Status));

	//IMPLEMENT AFTER FINISHING FOR CONVENIENCE
	/*if (argc == 2){
		if(fopen(argv[1]) != NULL)
		{
			fclose(argv[1]);
		}
		else
		{
			printf("Error: Invalid File Name");
		}
	}*/
	
	// command prompt with hostname
	/*
	getcwd(cwd, 250);
	hostname[1023] = '\0';		
	gethostname(hostname, 1023); 
	printf("%s", getenv("USER")); 
	printf("@");				  
	printf("%s", hostname);		  
	printf(" :: ");				  
	printf("%s", cwd);			  
	printf("-> ");	
	*/
	//To do: remove set fat32.img to argv[1]
	//Setting the boot block and current dir to root
	setBootBlock("fat32.img");
	initClusterNum();
	while(1){


		printf("Command-> ");	

	
		do{
			scanf("%256[^\n]", input);
         char* token =  strtok(input, " \n");
         strcpy(command, token);
         token = strtok(NULL, " \n");
         if(token != NULL){
            strcpy(arg, token);
         }else{
            strcpy(arg, ".");
         }

			if(strcmp(command, "exit") == 0)				//exit
				OurExit(ptr);
			
			if(strcmp(command, "info") == 0)
				info();
			if(strcmp(command, "ls") == 0){
	      		ls("fat32.img", arg); 
			}
			if(strcmp(command, "size") == 0){
	      		size("fat32.img", arg); 
			}
         if(strcmp(command, "cd") == 0){
	      	cd("fat32.img", arg); 
			}
			if(strcmp(command, "mkdir") == 0){
	      	mkdir("fat32.img", arg); 
			}
         if(strcmp(command, "creat") == 0){
	      	creat("fat32.img", arg); 
			}
         if(strcmp(command, "open") == 0){
            token = strtok(NULL, " \n");
            if(token != NULL){
               openMyFile("fat32.img", arg, token);
            }else{
               printf("Need to specify a mode\n");
            }
         }
         if(strcmp(command, "close") == 0){
            closeMyFile("fat32.img", arg);
         }
         if(strcmp(command, "read") == 0){
            char offset[256];
            char size[256];
            token = strtok(NULL, " \n");
            if(token != NULL){
               strcpy(offset, token);
               token = strtok(NULL, " \n");
               if(token != NULL){
                  strcpy(size, token);
                  readMyFile("fat32.img", arg, offset, size);
               }else{
                  printf("Not enough arguments\n");
               }
            }else{
               printf("Not enough arguments\n");
            }
         }
         if(strcmp(command, "write") == 0){
            char offset[256];
            char size[256];
            char string[256];
            token = strtok(NULL, " \n");
            if(token != NULL){
               strcpy(offset, token);
               token = strtok(NULL, " \n");
               if(token != NULL){
                  strcpy(offset, token);
                  token = strtok(NULL, "\n");
                  if(token != NULL){
                     strcpy(string, token);
                     writeMyFile("fat32.img", arg, offset, size, string);
                  }else{
                     printf("Not enough arguments\n");
                  }
               }else{
                  printf("Not enough arguments\n");
               }
            }else{
               printf("Not enough arguments\n");
            }
         }
         

		}while(getchar() != '\n');
	}

   free(opened);
	return 0;
}


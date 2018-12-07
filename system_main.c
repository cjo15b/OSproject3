//system_main.c

#include "filesystem.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, const char* argv[]){		//this will be our main filesystem program.
	FILE * fat32;
	char *ptr = NULL;
   char input[256];
	char command[256];
	char cwd[256];
	char hostname[1024];
   char arg[256];
	int ls_flag = 0;

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

		}while(getchar() != '\n');
	}

	return 0;
}


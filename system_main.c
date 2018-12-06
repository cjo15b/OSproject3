//system_main.c

#include <stdio.h>
#include "filesystem.h"
#include <unistd.h>
#include <stdlib.h>

int main (int argc, const char* argv[]){		//this will be our main filesystem program.
	FILE * fat32;
	char *ptr = NULL;
	char command[256];
	char cwd[256];
	char hostname[1024];
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
	

	while(1){


		printf("Command-> ");	

	
	do{
		scanf("%s", command);

		if(strcmp(command, "exit") == 0)				//exit
			OurExit(ptr);
		//To do: remove set fat32.img to argv[1]
		if(strcmp(command, "info") == 0)				//info
			info("fat32.img");
		if(strcmp(command, "ls") == 0 || ls_flag == 1){			//ls - passes second parameter as DIRNAME to ls function
		   if(ls_flag == 1){
		      ls(command); 
		      ls_flag = 0;
		   }
		   else
		      ls_flag = 1;
		}	


	}while(getchar() != '\n');
	}

return 0;
}


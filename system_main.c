//system_main.c

#include <stdio.h>
#include "filesystem.h"
#include <unistd.h>
#include <stdlib.h>

int main (int argc, const char* argv[]){		//this will be our main filesystem program.
	
	char *ptr = NULL;
	char command[256];
	char cwd[256];
	char hostname[1024];
	
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

		if(strcmp(command, "exit") == 0)
			OurExit(ptr);
		if(strcmp(command, "info") == 0)
			info("fat32.img");
			

	}while(getchar() != '\n');
	}




return 0;
}


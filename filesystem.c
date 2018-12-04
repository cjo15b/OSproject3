//filesystem.c
//
//

#include <stdlib.h>
#include "filesystem.h"

void OurExit(char *ptr){
	printf("Hello world!\n");
	free(ptr);
	exit(0);
}

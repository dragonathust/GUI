#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include "GifData.h"

int main(int argc, char *argv[])
{

	FILE *file_out;	

    int i;
    int file_length;

    unsigned char ch;	
	
	if( argc !=2 )
    	{
    		printf("Usage: write filename.\n");
    		return -1;
	}
	
	file_out=fopen(argv[1],"w+");
	if( file_out == NULL )
	{
    		printf("Can't open file %s.\n",argv[1]);
    		return -1;
	}

	fwrite(GifData,sizeof(GifData),1,file_out);

	fclose(file_out);

	return 0;
}



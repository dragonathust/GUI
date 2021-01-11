#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	int fd_in;
	FILE *file_out;	
	struct stat s;
    int i;
    int file_length;
    unsigned char *buffer_in;

	
	if( argc !=3 )
    	{
    		printf("Usage: read filename write filename.\n");
    		return -1;
	}

    fd_in=open(argv[1],O_RDONLY);
	if(fd_in <=0)
	{
    		printf("Can't open file %s.\n",argv[1]);
    		return -1;
	}
	
	file_out=fopen(argv[2],"w+");
	if( file_out == NULL )
	{
    		printf("Can't open file %s.\n",argv[2]);
			close(fd_in);
    		return -1;
	}

		printf("open file [%s] ok!\n",argv[1]);
		fstat(fd_in,&s);

		printf("size=%d\n",s.st_size);	
		
		file_length=s.st_size;

	buffer_in=malloc(file_length);

	if( !buffer_in )
	{
		printf("Out of memory.\n");
		goto fail2;
	}
	
	read(fd_in,buffer_in,file_length);
	
	fprintf(file_out,"const unsigned char GifData[]=\n");	
	fprintf(file_out,"{\n");	
	for(i=0;i<file_length;i++)
	{
	if( (i!=0)&&(i%32==0) )
	fprintf(file_out,"\n");	
	
	if(i!=file_length-1)
	fprintf(file_out,"0x%02x,",buffer_in[i]);
	else
	fprintf(file_out,"0x%02x",buffer_in[i]);
		
	}
	fprintf(file_out,"\n};\n\n");		
	free(buffer_in);
	
fail2:			
	close(fd_in);
	fclose(file_out);

	return 0;
}



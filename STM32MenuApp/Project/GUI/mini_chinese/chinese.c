#include <stdlib.h>
#include <stdio.h>
#include "Hzk16s.h"
#include "english_16_8.h"

#define USER_BASE 0xa1
#define USER_SPACE 94

#define INCLUDE_BEGIN "#ifndef __CHINESE_16S_H__\n#define __CHINESE_16S_H__\n\n"
#define INCLUDE_END "#endif /*__CHINESE_16S_H__*/\n\n"

#define FONT_STRING "static const unsigned char acFontHZ16s"

#define GUI_FONT_STRING "\nstatic const GUI_CHARINFO GUI_FontHZ16s_CharInfo"

int main(
	int			argc,
	char			**argv)
{
	char *res_name="res.txt";
    FILE *fpIn;
    FILE* fpOut;

    int size;
    int i,j;
    unsigned char word0;
    unsigned char word1;
    int q,w;

    int word_cnt;
    unsigned char *buffer;
    unsigned char *buffer_used;
    unsigned char buffer_line[256];
	int used_cnt;
	int res_cnt;
	int group_cnt;
	
    if (argc <= 1)
    {
    	puts("Create Chinese characters lib from txt file.\n");    	
        puts("Usage:\tchinese <file>");
  	fpIn = fopen(res_name, "rb");
	}
	else
	{
  	fpIn = fopen(argv[1], "rb");
	}
	if(fpIn == NULL )
	 {
		 printf("Can't open file %s\n",argv[1]);
		 exit(1);
	 }

    fseek(fpIn, 0, SEEK_END);
    size = ftell(fpIn);
    rewind(fpIn);

    printf("txt file [%s] size=%d\n",argv[1],size);
    	 		 
    buffer=malloc(size);
	buffer_used=malloc(size);
	memset(buffer_used,0,size);
	
    fread(buffer,size,1,fpIn);

	
	/*chinese_16s.h*/
    fpOut=fopen("chinese_16s.h", "wb+");
    
	fprintf(fpOut,INCLUDE_BEGIN);
	
	word0=' ';
	for(i=0;i<95;i++)
	{
	    	fprintf(fpOut,"/*");
    		fwrite(&word0,1,1,fpOut);
    		fprintf(fpOut,"*/\n");

    		fprintf(fpOut,FONT_STRING"_%02x%02x[]=\n",0,word0);
    		
    		fprintf(fpOut,"{");

    		for(j=0;j<16;j++)
    		{
    		fprintf(fpOut,"0x%02x,",acFont8x16[word0-32][j]);
    		}
    		
    		fprintf(fpOut,"};\n");
			word0++;
	}

    		fprintf(fpOut,"\n");
			
    i=0;
    word_cnt=0;
	used_cnt=0;
    while(1)
    {
NEXT:	
	if( i==size ) break;
	    	
    	word0=buffer[i];
    	i++;
    	
    	if( word0>=0xa1 )
    	{
    	 word1=buffer[i];
    	 i++;
	}
	else
	{
	 continue;
	}

    	if( (word0>=0xa1)&&(word1>=0xa1) )
    	{
			for(j=0;j<used_cnt;j++)
			{
				if( (buffer_used[2*j]==word0)&&(buffer_used[2*j+1]==word1))
				{
					goto NEXT;
				}
			}
			buffer_used[2*word_cnt]=word0;
    		buffer_used[2*word_cnt+1]=word1;
			used_cnt++;
			
    		q = word0-0xa1;
    		w = word1-0xa1;
    		
    		fprintf(fpOut,"/*");
    		fwrite(&word0,1,1,fpOut);
    		fwrite(&word1,1,1,fpOut);
    		fprintf(fpOut,"*/\n");

    		fprintf(fpOut,FONT_STRING"_%02x%02x[]=\n",word1,word0);
    		
    		fprintf(fpOut,"{");

    		
    		for(j=0;j<32;j++)
    		{
    		fprintf(fpOut,"0x%02x,",*(hzk16s+(q*94+w)*32+j));
    		}
    		
    		fprintf(fpOut,"};\n");
		
		word_cnt++;
	}

    }	

	 
     fprintf(fpOut,"\n#define WORD_NUMBER %d\n",word_cnt);

	 fprintf(fpOut,"/*\n");
     fprintf(fpOut,"const unsigned short chinese_16s_index[%d]={\n",word_cnt);
     
			for(j=0;j<used_cnt;j++)
			{
			word0=buffer_used[2*j];
			word1=buffer_used[2*j+1];
			fprintf(fpOut,"0x%04x,",word0|(word1<<8));
    		if( (j!=0)&&(j%16==0) )
    		{
    		 fprintf(fpOut,"\n");
    	    }						

			}

	fprintf(fpOut,"\n};\n");
	fprintf(fpOut,"*/\n");
	 
	word0=' ';	
	fprintf(fpOut,GUI_FONT_STRING"En[%d] = {\n",95);

	for(i=0;i<95;i++)
	{
	fprintf(fpOut,"{8, 8, 1, (void*)&acFontHZ16s_%02x%02x },\n",
	0,word0++);
	}	
	fprintf(fpOut,"};\n");
	

	fprintf(fpOut,GUI_FONT_STRING"[%d] = {\n",word_cnt);

	for(i=0;i<word_cnt;i++)
	{
	fprintf(fpOut,"{16, 16, 2, (void*)&acFontHZ16s_%02x%02x },\n",
	buffer_used[2*i+1],buffer_used[2*i]);
	}	
	fprintf(fpOut,"};\n");

	if(word_cnt%94)
	{
	q=0xa1+word_cnt/94;
	fprintf(fpOut,"\nstatic const GUI_FONT_PROP GUI_FontHZ16S_Prop_%02x = {\n",q);	
	fprintf(fpOut," 0x%02x%02x /* first character */\n",q,0xa1);	
	fprintf(fpOut,",0x%02x%02x /* last character  */\n",q,0xa1+(word_cnt-1)%94);	
	fprintf(fpOut,",&GUI_FontHZ16s_CharInfo[%d] /* address of first character */\n",(word_cnt/94)*94);	
	fprintf(fpOut,",(void*)0 /* pointer to next GUI_FONT_PROP */\n");		
	fprintf(fpOut,"};\n");		
	}
	
	for(i=word_cnt/94-1;i>=0;i--)
	{
	q=0xa1+i;
	fprintf(fpOut,"\nstatic const GUI_FONT_PROP GUI_FontHZ16S_Prop_%02x = {\n",q);	
	fprintf(fpOut," 0x%02x%02x /* first character */\n",q,0xa1);	
	fprintf(fpOut,",0x%02x%02x /* last character  */\n",q,0xfe);	
	fprintf(fpOut,",&GUI_FontHZ16s_CharInfo[%d] /* address of first character */\n",i*94);	
	if( (word_cnt%94==0)&&(i==0) )
	fprintf(fpOut,",(void*)0 /* pointer to next GUI_FONT_PROP */\n");	
	else
	fprintf(fpOut,",(void *)&GUI_FontHZ16S_Prop_%02x /* pointer to next GUI_FONT_PROP */\n",q+1);	
	
	fprintf(fpOut,"};\n");		
	}
	

	
	fprintf(fpOut,"\nstatic const GUI_FONT_PROP GUI_FontHZ16S_PropASCII = {\n");	
	fprintf(fpOut," 0x%02x%02x /* first character */\n",0,' ');	
	fprintf(fpOut,",0x%02x%02x /* last character  */\n",0,'~');	
	fprintf(fpOut,",&GUI_FontHZ16s_CharInfoEn[0] /* address of first character */\n");	
	//fprintf(fpOut,",(void*)0 /* pointer to next GUI_FONT_PROP */\n");	
	fprintf(fpOut,",(void *)&GUI_FontHZ16S_Prop_a1 /* pointer to next GUI_FONT_PROP */\n");
	fprintf(fpOut,"};\n");	
	
	fprintf(fpOut,"\nstatic const GUI_FONT GUI_FontHZ16S = {\n");
	fprintf(fpOut,"GUI_FONTTYPE_PROP_SJIS/*GUI_FONTTYPE_PROP_GB2312*/ /* type of font    */\n");
	fprintf(fpOut,",16 /* height of font  */\n");
	fprintf(fpOut,",16 /* space of font y */\n");
	fprintf(fpOut,",1 /* magnification x */\n");
	fprintf(fpOut,",1 /* magnification y */\n");
	fprintf(fpOut,",(void *)&GUI_FontHZ16S_PropASCII\n");
	fprintf(fpOut,"};\n");	

	fprintf(fpOut,INCLUDE_END);
		
    fclose(fpOut);
	
	/*res.h*/
	fpOut=fopen("res.h", "wb+");
	fprintf(fpOut,"#ifndef __RES_H__\n");
	fprintf(fpOut,"#define __RES_H__\n\n");
	
	res_cnt=0;
	group_cnt=0;
	rewind(fpIn);
	while(	fgets(buffer_line,256,fpIn)!=NULL )
    {
	if( strlen(buffer_line)<=2 )
	{
	fprintf(fpOut,"\n/*****************************************************");
	fprintf(fpOut,"*/\n\n");
	group_cnt++;
	res_cnt=0;
	continue;
	}
	
	fprintf(fpOut,"/*");
	fwrite(buffer_line,strlen(buffer_line),1,fpOut);
	fprintf(fpOut,"*/\n");
	
	fprintf(fpOut,"#define RES%d_%d_N \"",group_cnt+1,res_cnt+1);
	for(i=0;i<strlen(buffer_line);i++)
	{
	    word0=buffer_line[i];

		if(word0>=0xa1)
		{
			word1=buffer_line[i+1];
			
			for(j=0;j<used_cnt;j++)
			{
				if( (buffer_used[2*j]==word0)&&(buffer_used[2*j+1]==word1))
				{
						//fprintf(fpOut,"\\x%02x,\\x%02x,",USER_BASE+j%USER_SPACE,USER_BASE+j/USER_SPACE);
						fprintf(fpOut,"\\x%02x\\x%02x",USER_BASE+j/USER_SPACE,USER_BASE+j%USER_SPACE);
				}
			}
			i++;
		}
		else if (word0>=' '&&word0<='~')
		{
		//fwrite(&word0,1,1,fpOut);
		fprintf(fpOut,"\\x%02x",word0);
		}
		
	}
	fprintf(fpOut,"\"\n");
	fprintf(fpOut,"#define RES%d_%d RES%d_%d_N\"",
	group_cnt+1,res_cnt+1,group_cnt+1,res_cnt+1);
	fprintf(fpOut,"\\x0\"\n\n");
	
	res_cnt++;
	}
	
	fprintf(fpOut,"\n#endif /*__RES_H__*/\n\n");	
	fclose(fpOut);
	

	fclose(fpIn);
		
    free(buffer);
    free(buffer_used);
	
    return 0;
}


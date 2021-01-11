
#include "stm32f10x.h"

#include "ssd1963.h"

#include "english_16_8.h"
#include "chinese_16_16.h"

//#include "F24_ASCII.h"

u16 cursor_x=0;
u16 cursor_y=0;       
u16 type_color=COLOR_WHITE;
u16 back_color=COLOR_BLACK;
u16 type_font=16;

//#define CHAR_WIDTH 8
//#define CHAR_HEIGHT 16

//#define CHINESE_WIDTH 16
//#define CHINESE_HEIGHT 16

u8 CHAR_WIDTH=8;
u8 CHAR_HEIGHT=16;

u8 CHINESE_WIDTH=16;
u8 CHINESE_HEIGHT=16;

void TypeChar(u8 word)
{
	u8 i,j;
	u16 data;
	u32 index;
	u8 WidthSize;
	u32 word_data=0;
	const u8 *pData;
	
	SetWindows(cursor_x,cursor_x+CHAR_HEIGHT-1,cursor_y,cursor_y+CHAR_WIDTH-1);
	nCS;
	GramWrite();
  switch(type_font)
  {
   case 16:
   	for(i=0;i<16;i++)
   	{
   	index=0x80;
    	for(j=0;j<8;j++)
    	{
    	  data=(english[word][i]&index)?type_color:back_color;	
      	  WriteData(data);
      	  index=index>>1;
      	}
	}
	break;

   case 24:
/*
	 WidthSize=GUI_Font24_ASCII_CharInfo[word].BytesPerLine;	
	 CHAR_WIDTH=WidthSize*8;
	 pData=GUI_Font24_ASCII_CharInfo[word].pData;
	 
   	for(i=0;i<24;i++)
   	{
   	index=1<<(CHAR_WIDTH-1);
   	switch(WidthSize)
   	 {
   	 	case 1:
   	  	     word_data=pData[i];   	 	
   	 	     break;
   	 	case 2:
   	 	     word_data=pData[2*i]<<8|pData[2*i+1];
   	 	     break;
   	 	case 3:
   	 	     word_data=pData[3*i]<<16|pData[3*i+1]<<8|pData[3*i+2];
   	 	     break;   	 	        	 	      
   	 }
   	
    	for(j=0;j<CHAR_WIDTH;j++)
    	{
    	  data=(word_data&index)?type_color:back_color;	
      	  WriteData(data);
      	  index=index>>1;
      	}
	}
*/	   
        break;	
  }
	CS;
}

void TypeChinese(u16 word)
{
	u8 i,j;
	u16 data;
	u16 word_data;
	u16 index;
	int data_index=0;
	
	for(i=0;i<WORD_NUMBER;i++)
	{
	 if(chinese_16s_index[i]==word)
	  {
	   data_index=i;
	   break;
	  }
	}

	SetWindows(cursor_x,cursor_x+CHINESE_HEIGHT-1,cursor_y,cursor_y+CHINESE_WIDTH-1);
	nCS;
	GramWrite();

   	for(i=0;i<16;i++)
   	{
   	index=0x8000;
   	word_data=(chinese_16s[data_index][2*i]<<8)|chinese_16s[data_index][2*i+1];
    	for(j=0;j<16;j++)
    	{
    	  data=(word_data&index)?type_color:back_color;	
      	  WriteData(data);
      	  index=index>>1;
      	}
	}
	CS;
}

//x 0 ~ 29
//y 0  ~ 99
void SetCursor(u16 x,u16 y){
     cursor_x=x*16;//CHAR_HEIGHT;
     cursor_y=y*8;//CHAR_WIDTH;
}

void IncreaseCursor(u16 x,u16 y){
/*	
     if( cursor_x==0 )
     cursor_x=29*CHAR_HEIGHT;     
     else
     cursor_x-=x*CHAR_HEIGHT;
*/
     if( cursor_x==29*CHAR_HEIGHT )
     cursor_x=0;     
     else
     cursor_x+=x*CHAR_HEIGHT;
	 
     if( cursor_y==99*CHAR_WIDTH )
     cursor_y=0;
     else
     cursor_y+=y*CHAR_WIDTH;
}

void SetColor(u16 type,u16 back){
     type_color=type;
     back_color=back;
     }

void SetFont( u16 font )
{
  type_font=font;
  
  switch(font)
  {
   case 16:
	CHAR_WIDTH=8;
	CHAR_HEIGHT=16;
	CHINESE_WIDTH=16;
	CHINESE_HEIGHT=16;
	break;
	
   case 24:
	CHAR_WIDTH=8;
	CHAR_HEIGHT=24;
	CHINESE_WIDTH=24;
	CHINESE_HEIGHT=24;
	break;	
  }	
}

void NextLine( void )
{
    cursor_y=0;	
    if(cursor_x==29*CHAR_HEIGHT)
     {
      LCD_Clean(COLOR_BACKGROUND);
      cursor_x=0;
     }
     else
     {        
      cursor_x+=CHAR_HEIGHT;
     }	
}

void Print2LCD(char *p)
{
	u8 word0;
	u8 word1;
		
   for(;;)
      {
    	word0=*p; 

    	if (word0>=' '&&word0<='~')word0-=32;
    	else if(word0 =='\n')
    	{
	 NextLine();
      	 return;    	
    	}
    	else if( word0>=0xa1 )
    	{
    	 p++;
    	 word1=*p;
    	 if( word1>=0xa1 )
    	 {
    	  if(cursor_y==800-CHAR_WIDTH)
          {
           NextLine();	
    	  }  	
    	  TypeChinese(word0|(word1<<8));
    	  p++;
    	  cursor_y+=CHINESE_WIDTH;
    	  if(cursor_y>=800)
    	  {
           NextLine();
    	  } 
    	  continue;   	  
    	 }
    	 else
    	  return;
	}
    	else
    	 return;
            
    	TypeChar(word0);
    	p++;
    	cursor_y+=CHAR_WIDTH;
    	if(cursor_y>=800)
    	{
          NextLine();
    	}
      }
}

void DrawRectangular(u16 hs,u16 he,u16 vs,u16 ve,u16 data)
{
	u16 i,j;
	SetWindows(hs,he,vs,ve);
	nCS;
	GramWrite();
   	for(i=(he-hs+1);i>0;i--)
     	for(j=(ve-vs+1);j>0;j--)
        WriteData(data);
	CS;  
}

void PrintDate(unsigned char year,unsigned char month,unsigned char day)
{
    char Printbuf[13];
    	
    Printbuf[0]=((year&0xF0)>>4)+0x30;
    Printbuf[1]=(year&0x0F)+0x30;
    Printbuf[2]=0xc4;
    Printbuf[3]=0xea;/*年*/;
    Printbuf[4]=((month&0xF0)>>4)+0x30;
    Printbuf[5]=(month&0x0F)+0x30;
    Printbuf[6]=0xd4;
    Printbuf[7]=0xc2;/*月*/;
    Printbuf[8]=((day&0xF0)>>4)+0x30;
    Printbuf[9]=(day&0x0F)+0x30;
    Printbuf[10]=0xc8;
    Printbuf[11]=0xd5;/*日*/;
    Printbuf[12]=0;     
    Print2LCD(Printbuf);
}

unsigned char week_table[]={0xc8,0xd5,0xd2,0xbb,0xb6,0xfe,0xc8,0xfd,0xcb,0xc4,0xce,0xe5,0xc1,0xf9};
/*日一二三四五六*/
void PrintWeek(unsigned char week)
{
    char Printbuf[7];
    	
    Printbuf[0]=0xd0;
    Printbuf[1]=0xc7;/*星*/
    Printbuf[2]=0xc6;
    Printbuf[3]=0xda;/*期*/;
    Printbuf[4]=week_table[2*week];
    Printbuf[5]=week_table[2*week+1];
    Printbuf[6]=0;     
    Print2LCD(Printbuf);	
}

void PrintTime(unsigned char hour,unsigned char minute,unsigned char second)
{
    char Printbuf[9];
    	
    Printbuf[0]=((hour&0xF0)>>4)+0x30;
    Printbuf[1]=(hour&0x0F)+0x30;
    Printbuf[2]=':';
    Printbuf[3]=((minute&0xF0)>>4)+0x30;
    Printbuf[4]=(minute&0x0F)+0x30;
    Printbuf[5]=':';
    Printbuf[6]=((second&0xF0)>>4)+0x30;
    Printbuf[7]=(second&0x0F)+0x30;
    Printbuf[8]=0;             
    Print2LCD(Printbuf);	
}

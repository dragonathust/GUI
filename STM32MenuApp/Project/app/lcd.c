/*******************************************

				  7´ç QVGAÏÔÊ¾Çý¶¯³ÌÐò

********************************************/


#include "fsmc_sram.h"
#include "ssd1963.h"


void LCD_init(void);
void LCD_test(void);

extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);
void delay_time(unsigned int i);
void SetScrollArea(unsigned int top, unsigned int scroll, unsigned int bottom);
void SetScrollStart(unsigned int line);
void SetTearingCfg(unsigned char state, unsigned char mode);


void LCD_init(void)
{
	lcd_rst();	 

	LCD_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCD_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);
	
	LCD_WR_REG(0x00E0);  // PLL enable
	LCD_WR_Data(0x0001);
	delay_time(1);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);
	delay_time(5);
	LCD_WR_REG(0x0001);  // software reset
	delay_time(5);
	LCD_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	LCD_WR_Data(0x0003);
	LCD_WR_Data(0x0033);
	LCD_WR_Data(0x0033);

	//LCD_WR_Data(0x0000);
	//LCD_WR_Data(0x00b4);
	//LCD_WR_Data(0x00e7);


	LCD_WR_REG(0x00B0);	//LCD SPECIFICATION
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);
	LCD_WR_Data((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_Data(HDP&0X00FF);
	LCD_WR_Data((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_Data(VDP&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_Data((HT>>8)&0X00FF);  //Set HT
	LCD_WR_Data(HT&0X00FF);
	LCD_WR_Data((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(HPS&0X00FF);
	LCD_WR_Data(HPW);			   //Set HPW
	LCD_WR_Data((LPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(LPS&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_Data((VT>>8)&0X00FF);   //Set VT
	LCD_WR_Data(VT&0X00FF);
	LCD_WR_Data((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_Data(VPS&0X00FF);
	LCD_WR_Data(VPW);			   //Set VPW
	LCD_WR_Data((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_Data(FPS&0X00FF);

	LCD_WR_REG(0x00BA);
	LCD_WR_Data(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x00B8);
	LCD_WR_Data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_Data(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x0036); //rotation
	LCD_WR_Data(0x0000);


	LCD_WR_REG(0x00F0); //pixel data interface
	LCD_WR_Data(0x0003);

	delay_time(5);

	LCD_Clean(0xf800);
	LCD_WR_REG(0x0026); //display on
	LCD_WR_Data(0x0001);

	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0006);
	LCD_WR_Data(0x0080);
	
	LCD_WR_Data(0x0009); //enable brightness ctrl
	LCD_WR_Data(0x00c0); //brightness
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00d0);//ÉèÖÃ¶¯Ì¬±³¹â¿ØÖÆÅäÖÃ 
	LCD_WR_Data(0x000d);

	LCD_Clean(COLOR_BACKGROUND);
}

void SetWindows(u16 hs,u16 he,u16 vs,u16 ve)
{

	LCD_WR_REG(0x002a);	
	LCD_WR_Data(vs>>8);	    
	LCD_WR_Data(vs&0x00ff);
	LCD_WR_Data(ve>>8);	    
	LCD_WR_Data(ve&0x00ff);
	LCD_WR_REG(0x002b);	
	LCD_WR_Data(hs>>8);	    
	LCD_WR_Data(hs&0x00ff);
	LCD_WR_Data(he>>8);	    
	LCD_WR_Data(he&0x00ff);

}

void LCD_Clean(u16 data)
{
    int i;

	SetWindows(0,VDP,0,HDP);

	GramWrite();
	
	for(i=0;i<LCD_WIDTH*LCD_HEIGHT;i++)
	{
          	WriteData(data);
	}
}

void LCD_test( void )
{
	LCD_Clean(COLOR_BACKGROUND);
	SetCursor(0,0);
	
	SetColor(COLOR_GREEN,COLOR_BLUE);
	Print2LCD("Hello! LCD resolution is 800*480.\n");
	
	SetColor(COLOR_RED,COLOR_BLACK);
	Print2LCD("It can display char 30 lines,100 characters per line.\n");
	
	Print2LCD("\n");
	 	
	SetColor(COLOR_WHITE,COLOR_BLACK);

 	Print2LCD("The code in this directory is for Cypress FX2 (e.g. CY7C68013A) and can be\
 compiled with the SDCC compiler (I tried version 2.6 as shipped with Ubuntu\
 6.10).\n");
 
 	SetColor(COLOR_GREEN,COLOR_BLACK);

 	Print2LCD("ºº×ÖÏÔÊ¾²âÊÔ£¬²¿·Ö×Ö¿â¡£ºº×ÖÏÔÊ¾²âÊÔ£¬²¿·Ö×Ö¿â¡£ºº×ÖÏÔÊ¾²âÊÔ£¬²¿·Ö×Ö¿â¡£ºº×ÖÏÔÊ¾²âÊÔ£¬²¿·Ö×Ö¿â¡£\n");
 	
 	SetColor(COLOR_BLACK,COLOR_WHITE); 	
 	 	
 	Print2LCD("ÖÐandÓ¢»ìºÏÏÔÊ¾¡£ÖÐandÓ¢»ìºÏÏÔÊ¾¡£ÖÐandÓ¢»ìºÏÏÔÊ¾¡£ÖÐandÓ¢»ìºÏÏÔÊ¾¡£\n");
 
}

void delay_time(unsigned int i)
{
    unsigned int a;
    unsigned int b;
    for(b=0;b<i;b++)
    for(a=0;a<1000;a++);
}

/*********************************************************************
* Function:  SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* PreCondition: none
*
* Input: top - Top Fixed Area in number of lines from the top
*				of the frame buffer
*		 scroll - Vertical scrolling area in number of lines
*		 bottom - Bottom Fixed Area in number of lines
*
* Output: none
*
* Side Effects: none
*
* Overview:
*
* Note: Reference: section 9.22 Set Scroll Area, SSD1963 datasheet Rev0.20
********************************************************************/
void SetScrollArea(unsigned int top, unsigned int scroll, unsigned int bottom)
{


//	WriteCommand(CMD_SET_SCROLL_AREA);
//	CS_LAT_BIT = 0;
	LCD_WR_REG(0x33);
	LCD_WR_Data(top>>8);
	LCD_WR_Data(top);
	LCD_WR_Data(scroll>>8);
	LCD_WR_Data(scroll);
	LCD_WR_Data(bottom>>8);
	LCD_WR_Data(bottom);
//	CS_LAT_BIT = 1;	
}

/*********************************************************************
* Function:  void  SetScrollStart(SHORT line)
*
* Overview: First, we need to define the scrolling area by SetScrollArea()
*			before using this function. 
*
* PreCondition: SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* Input: line - Vertical scrolling pointer (in number of lines) as 
*		 the first display line from the Top Fixed Area defined in SetScrollArea()
*
* Output: none
*
* Note: Example -
*
*		SHORT line=0;
*		SetScrollArea(0,272,0);
*		for(line=0;line<272;line++) {SetScrollStart(line);DelayMs(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
void SetScrollStart(unsigned int line)
{

//	LCD_WR_REG(0x002A);	
//    LCD_WR_Data(0);	  
//	LCD_WR_REG(CMD_SET_SCROLL_START);
	LCD_WR_REG(0x37);
//	CS_LAT_BIT = 0;
	LCD_WR_Data(line>>8);
	LCD_WR_Data(line);	
//	CS_LAT_BIT = 1;
}

/*********************************************************************
* Function:  void  SetTearingCfg(BOOL state, BOOL mode)
*
* Overview: This function enable/disable tearing effect
*
* PreCondition: none
*
* Input: 	BOOL state -	1 to enable
*							0 to disable
*			BOOL mode -		0:  the tearing effect output line consists
*								of V-blanking information only
*							1:	the tearing effect output line consists
*								of both V-blanking and H-blanking info.
* Output: none
*
* Note:
********************************************************************/
void SetTearingCfg(unsigned char state, unsigned char mode)
{


	if(state == 1)
	{
		LCD_WR_REG(0x35);
		//CS_LAT_BIT = 0;
		LCD_WR_Data(mode&0x01);
		//CS_LAT_BIT = 1;
	}
	else
	{
		LCD_WR_REG(0x34);
	}


}




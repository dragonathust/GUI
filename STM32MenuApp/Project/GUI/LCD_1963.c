/*
*********************************************************************************************************
*
*	SSD1963 LCD Driver for ucGUI
*/

#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"

#include "fsmc_sram.h"
#include "ssd1963.h"

#if (LCD_CONTROLLER == 1963) \
    && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

//#define SET_PIXEL(x,y,index) LCD_L0_SetPixelIndex(x,y,index) 	

static U16 LineBuffer[LCD_XSIZE];

#define SET_PIXEL(x,y,index)\
{\
	LineBuffer[x]=index;\
}

#define XOR_PIXEL(x,y)\
{\
	LineBuffer[x]=LCD_NUM_COLORS-1-LineBuffer[x];\
}

#if 0
#define PUT_LINE(x0,xsize)\
{\
	for (j=0; j<xsize; j++) {\
	WriteData(LineBuffer[j+x0]);\
	}\
}
#else
#define PUT_LINE(x0,xsize)\
{\
	Rem=xsize;\
	j=x0;\
	for(;Rem>=8;Rem-=8,j+=8)\
	{\
	WriteData(LineBuffer[j]);\
	WriteData(LineBuffer[j+1]);\
	WriteData(LineBuffer[j+2]);\
	WriteData(LineBuffer[j+3]);\
	WriteData(LineBuffer[j+4]);\
	WriteData(LineBuffer[j+5]);\
	WriteData(LineBuffer[j+6]);\
	WriteData(LineBuffer[j+7]);\
	}\
	for (; Rem--; j++)\
	{\
	WriteData(LineBuffer[j]);\
	}\
}
#endif

#define WRTIE_DATA_8X(data)\
{\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
	WriteData(data);\
}	
	
static void LCD_PutLine(int x0,int y,int xsize)
{
int x1=x0+xsize-1;

/*
    for (; x0 <= x1; x0++) {
		SETPIXEL(x0,y,LineBuffer[x0]);
		}
*/
SetHLine(y,x0,x1);
GramWrite();
#if 0
    for (; x0 <= x1; x0++) {
		WriteData(LineBuffer[x0]);
		}
#else
	for(;xsize>=8;xsize-=8,x0+=8)
	{
	WriteData(LineBuffer[x0]);
	WriteData(LineBuffer[x0+1]);
	WriteData(LineBuffer[x0+2]);
	WriteData(LineBuffer[x0+3]);
	WriteData(LineBuffer[x0+4]);
	WriteData(LineBuffer[x0+5]);
	WriteData(LineBuffer[x0+6]);
	WriteData(LineBuffer[x0+7]);
	}
	for (; xsize--; x0++)
	{
	WriteData(LineBuffer[x0]);
	}
#endif
}

static void LCD_GetLine(int x0,int y,int xsize)
{
int x1=x0+xsize-1;

/*
    for (; x0 <=x1; x0++) {
		LineBuffer[x0]=LCD_L0_GetPixelIndex(x0,y);
		}
*/
SetHLine(y,x0,x1);
GramRead();
#if 0
    for (; x0 <= x1; x0++) {
		LineBuffer[x0]=LCD_RD_data();
		}
#else		
	for(;xsize>=8;xsize-=8,x0+=8)
	{
	(LineBuffer[x0])=LCD_RD_data();
	(LineBuffer[x0+1])=LCD_RD_data();
	(LineBuffer[x0+2])=LCD_RD_data();
	(LineBuffer[x0+3])=LCD_RD_data();
	(LineBuffer[x0+4])=LCD_RD_data();
	(LineBuffer[x0+5])=LCD_RD_data();
	(LineBuffer[x0+6])=LCD_RD_data();
	(LineBuffer[x0+7])=LCD_RD_data();
	}
	for (; xsize--; x0++)
	{
	(LineBuffer[x0])=LCD_RD_data();
	}
#endif		
}
	
/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
//  GUI_USE_PARA(x);
//  GUI_USE_PARA(y);
//  GUI_USE_PARA(PixelIndex);
SetPoint(y,x);
GramWrite();
WriteData(PixelIndex);
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
*/
unsigned int LCD_L0_GetPixelIndex(int x, int y) {
//  GUI_USE_PARA(x);
//  GUI_USE_PARA(y);
SetPoint(y,x);
GramRead();
  return LCD_RD_data();
}

/*********************************************************************
*
*       LCD_L0_XorPixel
*/
void LCD_L0_XorPixel(int x, int y) {
LCD_PIXELINDEX Index;
//  GUI_USE_PARA(x);
//  GUI_USE_PARA(y);
SetPoint(y,x);
GramRead();
Index=LCD_RD_data();
GramWrite();
WriteData(LCD_NUM_COLORS-1-Index);
}

/*********************************************************************
*
*       LCD_L0_DrawHLine
*/
void LCD_L0_DrawHLine(int x0, int y,  int x1) {
//  GUI_USE_PARA(x0);
//  GUI_USE_PARA(y);
//  GUI_USE_PARA(x1);
#if 0
SetWindows(y,y,x0,x1);
GramWrite();
    for (; x0 <= x1; x0++) {
		WriteData(LCD_COLORINDEX);
		}
#else
LCD_PIXELINDEX pixel=LCD_COLORINDEX;
int Rem=(x1-x0+1);
	
SetHLine(y,x0,x1);
GramWrite();

	for(;Rem>=8;Rem-=8)
	{
	WRTIE_DATA_8X(pixel);
	}
	while(Rem--)
	{
	WriteData(pixel);
	}
#endif
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
*/
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
//  GUI_USE_PARA(x);
//  GUI_USE_PARA(y0);
//  GUI_USE_PARA(y1);
#if 0
SetWindows(y0,y1,x,x);
GramWrite();
  for (; y0 <= y1; y0++) {
		WriteData(LCD_COLORINDEX);
	}
#else
LCD_PIXELINDEX pixel=LCD_COLORINDEX;
int Rem=(y1-y0+1);

SetVLine(y0,y1,x);
GramWrite();

	for(;Rem>=8;Rem-=8)
	{
	WRTIE_DATA_8X(pixel);
	}
	while(Rem--)
	{
	WriteData(pixel);
	}
#endif	
}

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
//  GUI_USE_PARA(x0);
//  GUI_USE_PARA(y0);
//  GUI_USE_PARA(x1);
//  GUI_USE_PARA(y1);
#if 0
int x,y;

SetWindows(y0,y1,x0,x1);
GramWrite();

  for (y=y0; y <= y1; y++) {
    for (x=x0; x <= x1; x++) {
		WriteData(LCD_COLORINDEX);
		}
	}
#else
LCD_PIXELINDEX pixel=LCD_COLORINDEX;
int Rem=(x1-x0+1)*(y1-y0+1);

SetRect(y0,y1,x0,x1);
GramWrite();

	for(;Rem>=32;Rem-=32)
	{
	WRTIE_DATA_8X(pixel);
	WRTIE_DATA_8X(pixel);
	WRTIE_DATA_8X(pixel);
	WRTIE_DATA_8X(pixel);
	}
	for(;Rem>=8;Rem-=8)
	{
	WRTIE_DATA_8X(pixel);
	}
	while(Rem--)
	{
	WriteData(pixel);
	}
#endif
}

static void _DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;
  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
/*
// Jump to right entry point
*/
  pixels = *p;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:
    switch (Diff&7) {
    case 0:   
      goto WriteBit0;
    case 1:   
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    case 3:
      goto WriteBit3;
    case 4:
      goto WriteBit4;
    case 5:   
      goto WriteBit5;
    case 6:   
      goto WriteBit6;
    case 7:   
      goto WriteBit7;
    }
    break;
  case LCD_DRAWMODE_TRANS:
    switch (Diff&7) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    case 3:
      goto WriteTBit3;
    case 4:
      goto WriteTBit4;
    case 5:   
      goto WriteTBit5;
    case 6:   
      goto WriteTBit6;
    case 7:   
      goto WriteTBit7;
    }
    break;
  case LCD_DRAWMODE_XOR:
    switch (Diff&7) {
    case 0:   
      goto WriteXBit0;
    case 1:   
      goto WriteXBit1;
    case 2:
      goto WriteXBit2;
    case 3:
      goto WriteXBit3;
    case 4:
      goto WriteXBit4;
    case 5:   
      goto WriteXBit5;
    case 6:   
      goto WriteXBit6;
    case 7:   
      goto WriteXBit7;
    }
  }
/*
        Write with transparency
*/

  WriteTBit0:
   if (pixels&(1<<7)) SET_PIXEL(x+0, y, Index1);
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(1<<6)) SET_PIXEL(x+1, y, Index1);
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(1<<5)) SET_PIXEL(x+2, y, Index1);
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(1<<4)) SET_PIXEL(x+3, y, Index1);
    if (!--xsize)
      return;
  WriteTBit4:
    if (pixels&(1<<3)) SET_PIXEL(x+4, y, Index1);
    if (!--xsize)
      return;
  WriteTBit5:
    if (pixels&(1<<2)) SET_PIXEL(x+5, y, Index1);
    if (!--xsize)
      return;
  WriteTBit6:
    if (pixels&(1<<1)) SET_PIXEL(x+6, y, Index1);
    if (!--xsize)
      return;
  WriteTBit7:
    if (pixels&(1<<0)) SET_PIXEL(x+7, y, Index1);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteTBit0;

/*
        Write without transparency
*/

  WriteBit0:
    SET_PIXEL(x+0, y, (pixels&(1<<7)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit1:
    SET_PIXEL(x+1, y, (pixels&(1<<6)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit2:
    SET_PIXEL(x+2, y, (pixels&(1<<5)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit3:
    SET_PIXEL(x+3, y, (pixels&(1<<4)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit4:
    SET_PIXEL(x+4, y, (pixels&(1<<3)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit5:
    SET_PIXEL(x+5, y, (pixels&(1<<2)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit6:
    SET_PIXEL(x+6, y, (pixels&(1<<1)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit7:
    SET_PIXEL(x+7, y, (pixels&(1<<0)) ? Index1 : Index0);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteBit0;

/*
        Write XOR mode
*/

  WriteXBit0:
    if (pixels&(1<<7))
      XOR_PIXEL(x+0, y);
    if (!--xsize)
      return;
  WriteXBit1:
    if (pixels&(1<<6))
      XOR_PIXEL(x+1, y);
    if (!--xsize)
      return;
  WriteXBit2:
    if (pixels&(1<<5))
      XOR_PIXEL(x+2, y);
    if (!--xsize)
      return;
  WriteXBit3:
    if (pixels&(1<<4))
      XOR_PIXEL(x+3, y);
    if (!--xsize)
      return;
  WriteXBit4:
    if (pixels&(1<<3))
      XOR_PIXEL(x+4, y);
    if (!--xsize)
      return;
  WriteXBit5:
    if (pixels&(1<<2))
      XOR_PIXEL(x+5, y);
    if (!--xsize)
      return;
  WriteXBit6:
    if (pixels&(1<<1))
      XOR_PIXEL(x+6, y);
    if (!--xsize)
      return;
  WriteXBit7:
    if (pixels&(1<<0))
      XOR_PIXEL(x+7, y);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteXBit0;
	
}

static void  _DrawBitLine2BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixels;
/*
// Jump to right entry point
*/
  pixels = *p;
  if (pTrans) {
    /*
      with palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    default:
      goto WriteTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteBit0;
    case 1:
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    default:
      goto WriteBit3;
    }
  /*
          Write without transparency
  */
  WriteBit0:
    SET_PIXEL(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteBit1:
    SET_PIXEL(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteBit2:
    SET_PIXEL(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteBit3:
    SET_PIXEL(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels&(3<<6))
      SET_PIXEL(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(3<<4))
      SET_PIXEL(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(3<<2))
      SET_PIXEL(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(3<<0))
      SET_PIXEL(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteTBit0;
  } else { 
    /* 
      without palette 
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteDDPTBit0;
    case 1:
      goto WriteDDPTBit1;
    case 2:
      goto WriteDDPTBit2;
    default:
      goto WriteDDPTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteDDPBit0;
    case 1:
      goto WriteDDPBit1;
    case 2:
      goto WriteDDPBit2;
    default:
      goto WriteDDPBit3;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    SET_PIXEL(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPBit1:
    SET_PIXEL(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPBit2:
    SET_PIXEL(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPBit3:
    SET_PIXEL(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels&(3<<6))
      SET_PIXEL(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&(3<<4))
      SET_PIXEL(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPTBit2:
    if (pixels&(3<<2))
      SET_PIXEL(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPTBit3:
    if (pixels&(3<<0))
      SET_PIXEL(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPTBit0;
  }
}

static void  _DrawBitLine4BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;

  pixels = *p;
  if (pTrans)
  {
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)
    {
      if ((Diff&1) ==0)
        goto WriteTBit0;
        goto WriteTBit1;
    }
    else
    {
      if ((Diff&1) ==0)
        goto WriteBit0;
        goto WriteBit1;
    }

  WriteBit0:
    SET_PIXEL(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteBit1:
    SET_PIXEL(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels>>4)
      SET_PIXEL(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&0xf)
      SET_PIXEL(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteTBit0;
  } else {
    /*
      without palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
      if ((Diff&1) ==0)
        goto WriteDDPTBit0;
      goto WriteDDPTBit1;
    } else {
      if ((Diff&1) ==0)
        goto WriteDDPBit0;
      goto WriteDDPBit1;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    SET_PIXEL(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPBit1:
    SET_PIXEL(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels>>4)
      SET_PIXEL(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&0xf)
      SET_PIXEL(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPTBit0;
  }
}

static void _DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize >=8; xsize-=8,x+=8,p+=8) {
        SET_PIXEL(x, y, *(pTrans+*p));
        SET_PIXEL(x+1, y, *(pTrans+*(p+1)));
        SET_PIXEL(x+2, y, *(pTrans+*(p+2)));
        SET_PIXEL(x+3, y, *(pTrans+*(p+3)));
        SET_PIXEL(x+4, y, *(pTrans+*(p+4)));
        SET_PIXEL(x+5, y, *(pTrans+*(p+5)));
        SET_PIXEL(x+6, y, *(pTrans+*(p+6)));
        SET_PIXEL(x+7, y, *(pTrans+*(p+7)));
      }
      for (;xsize > 0; xsize--,x++,p++) {
        SET_PIXEL(x, y, *(pTrans+*p));
      }
    } else {
      for (;xsize >=8; xsize-=8,x+=8,p+=8) {
        SET_PIXEL(x, y, *p);
        SET_PIXEL(x+1, y, *(p+1));
        SET_PIXEL(x+2, y, *(p+2));
        SET_PIXEL(x+3, y, *(p+3));
        SET_PIXEL(x+4, y, *(p+4));
        SET_PIXEL(x+5, y, *(p+5));
        SET_PIXEL(x+6, y, *(p+6));
        SET_PIXEL(x+7, y, *(p+7));		
      }
      for (;xsize > 0; xsize--,x++,p++) {
        SET_PIXEL(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SET_PIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SET_PIXEL(x+0, y, pixel);
        }
      }
    }
  }
}

static void _DrawBitLine16BPP(int x, int y, U16 const*p, int xsize,const LCD_PIXELINDEX*pTrans)
{
    LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize >=8; xsize-=8,x+=8,p+=8) {
        SET_PIXEL(x, y, *(pTrans+*p));
        SET_PIXEL(x+1, y, *(pTrans+*(p+1)));
        SET_PIXEL(x+2, y, *(pTrans+*(p+2)));
        SET_PIXEL(x+3, y, *(pTrans+*(p+3)));
        SET_PIXEL(x+4, y, *(pTrans+*(p+4)));
        SET_PIXEL(x+5, y, *(pTrans+*(p+5)));
        SET_PIXEL(x+6, y, *(pTrans+*(p+6)));
        SET_PIXEL(x+7, y, *(pTrans+*(p+7)));
      }		
      for (;xsize > 0; xsize--,x++,p++) {
        SET_PIXEL(x, y, *(pTrans+*p));
      }
    } else {
      for (;xsize >=8; xsize-=8,x+=8,p+=8) {
        SET_PIXEL(x, y, *p);
        SET_PIXEL(x+1, y, *(p+1));
        SET_PIXEL(x+2, y, *(p+2));
        SET_PIXEL(x+3, y, *(p+3));
        SET_PIXEL(x+4, y, *(p+4));
        SET_PIXEL(x+5, y, *(p+5));
        SET_PIXEL(x+6, y, *(p+6));
        SET_PIXEL(x+7, y, *(p+7));		
      }    
      for (;xsize > 0; xsize--,x++,p++) {
        SET_PIXEL(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SET_PIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SET_PIXEL(x+0, y, pixel);
        }
      }
    }
  }
  
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
/*
  GUI_USE_PARA(x0);
  GUI_USE_PARA(y0);
  GUI_USE_PARA(xsize);
  GUI_USE_PARA(ysize);
  GUI_USE_PARA(BitsPerPixel);
  GUI_USE_PARA(BytesPerLine);
  GUI_USE_PARA(pData);
  GUI_USE_PARA(Diff);
  GUI_USE_PARA(pTrans);
*/
  int i,j,Rem;
  int ReadMode;
  /*
     Use DrawBitLineXBPP
  */
  ReadMode=GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR);
  if(!ReadMode)
  {
   SetWindows(y0,y0+ysize-1,x0,x0+xsize-1);
   GramWrite();
  }
    switch (BitsPerPixel) {
      case 1:
	  //_DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);	
		if(ReadMode)
		{
		  for (i=0; i<ysize; i++) {
			LCD_GetLine(x0,i+y0,xsize);	  
	        _DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
			LCD_PutLine(x0,i+y0,xsize);
			pData += BytesPerLine;
			}
		}		
		else
		{
		  for (i=0; i<ysize; i++) {
	        _DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);		
			PUT_LINE(x0,xsize);
			pData += BytesPerLine;
			}
		}		
        break;
		
      case 2:
	  //_DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
		if(ReadMode)
		{
		  for (i=0; i<ysize; i++) {
			LCD_GetLine(x0,i+y0,xsize);	  
	        _DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
			LCD_PutLine(x0,i+y0,xsize);
			pData += BytesPerLine;
			}
		}		
		else
		{
		  for (i=0; i<ysize; i++) {
	        _DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);		
			PUT_LINE(x0,xsize);
			pData += BytesPerLine;
			}
		}  
        break;
		
      case 4:
	  //_DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
		if(ReadMode)
		{
		  for (i=0; i<ysize; i++) {
			LCD_GetLine(x0,i+y0,xsize);	  
	        _DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
			LCD_PutLine(x0,i+y0,xsize);
			pData += BytesPerLine;
			}
		}		
		else
		{
		  for (i=0; i<ysize; i++) {
	        _DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);		
			PUT_LINE(x0,xsize);
			pData += BytesPerLine;
			}
		}	  
        break;
		
      case 8:
	  //_DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
		if(ReadMode)
		{
		  for (i=0; i<ysize; i++) {
			LCD_GetLine(x0,i+y0,xsize);	  
	        _DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
			LCD_PutLine(x0,i+y0,xsize);
			pData += BytesPerLine;
			}
		}		
		else
		{
		  for (i=0; i<ysize; i++) {
	        _DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);		
			PUT_LINE(x0,xsize);
			pData += BytesPerLine;
			}
		}		  
        break;
		
      case 16:
	  //_DrawBitLine16BPP(x0, i+y0, (const U16 *)pData, xsize, pTrans);
		if(ReadMode)
		{
		  for (i=0; i<ysize; i++) {
			LCD_GetLine(x0,i+y0,xsize);	  
	        _DrawBitLine16BPP(x0, i+y0, (const U16 *)pData, xsize, pTrans);
			LCD_PutLine(x0,i+y0,xsize);
			pData += BytesPerLine;
			}
		}		
		else
		{
		  for (i=0; i<ysize; i++) {
	        _DrawBitLine16BPP(x0, i+y0, (const U16 *)pData, xsize, pTrans);		
			PUT_LINE(x0,xsize);
			pData += BytesPerLine;
			}
		}
        break;
    }

}

/*********************************************************************
*
*       LCD_WritePixelsRGB
*/
void LCD_WritePixelsRGB(const U8*p, int x0, int y0, int xSize) {
  U8 r,g,b;
  
  SetWindows(y0,y0,x0,x0+xSize-1);
  GramWrite();  
  while (xSize) {
    r = *p++;
    g = *p++;
    b = *p++;
    //LCD_SetColor(r | (g << 8) | (U32)((U32)b << 16));
    //LCD_DrawPixel(x0++, y0);
    LCD_SetColor(r | (g << 8) | (U32)((U32)b << 16));
	WriteData(LCD_COLORINDEX);
    xSize--;
  }
}

/*********************************************************************
*
*       LCD_WritePixelsGray
*/
void LCD_WritePixelsGray(const U8*p, int x0, int y0, int xSize) {
  U8 u;
  
  SetWindows(y0,y0,x0,x0+xSize-1);
  GramWrite();    
  while (xSize) {
    u = *p++;
    //LCD_SetColor(u * (U32)0x10101);
    //LCD_DrawPixel(x0++, y0);
	LCD_SetColor(u * (U32)0x10101);
	WriteData(LCD_COLORINDEX);
    xSize--;
  }
}

/*********************************************************************
*
*       LCD_L0_SetOrg
*/
void LCD_L0_SetOrg(int x, int y) {
  GUI_USE_PARA(x);
  GUI_USE_PARA(y);
}

/*********************************************************************
*
*       LCD_On / LCD_Off
*/
void LCD_On (void) {}
void LCD_Off(void) {}

/*********************************************************************
*
*       LCD_L0_Init
*/
int LCD_L0_Init(void) {
  return 0;
}

/*********************************************************************
*
*       LCD_L0_SetLUTEntry
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
  GUI_USE_PARA(Pos);
  GUI_USE_PARA(Color);
}

#else

void LCDNull_c(void);
void LCDNull_c(void) {} /* avoid empty object files */

#endif /* (LCD_CONTROLLER == 1963) */

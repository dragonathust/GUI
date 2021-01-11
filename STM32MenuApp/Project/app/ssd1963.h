
#ifndef __LCD_H__
#define __LCD_H__


#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR


//=================================================================================
//Æß´çÆÁµÄÏÔÊ¾ÉèÖÃ
//=================================================================================

#define  HDP    799
#define  HT       850
#define  HPS     51
#define  LPS      3
#define  HPW    48

#define  VDP     479
#define  VT        530
#define  VPS      50
#define  FPS       30
#define  VPW     3



#define LCD_WR_REG(index)\
{\
	*(__IO uint16_t *) (Bank1_LCD_C)= (index);\
}

#define LCD_WR_CMD(index,val)\
{\
	*(__IO uint16_t *) (Bank1_LCD_C)= index;\
	*(__IO uint16_t *) (Bank1_LCD_D)= val;\
}

#define LCD_RD_data()\
	*(__IO uint16_t *) (Bank1_LCD_D)

#define LCD_WR_Data(val)\
{\
	*(__IO uint16_t *) (Bank1_LCD_D)= val;\
}

#define nCS
#define CS
#define  GramWrite() 	LCD_WR_REG(0x002c)
#define  GramRead() 	LCD_WR_REG(0x002e)
#define WriteData(x)     LCD_WR_Data(x)

#define SetPoint( hs,vs)\
{\
	LCD_WR_REG(0x002a);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_REG(0x002b);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
}

#define SetHLine(hs, vs, ve)\
{\
	LCD_WR_REG(0x002a);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_Data(ve>>8);\
	LCD_WR_Data(ve&0x00ff);\
	LCD_WR_REG(0x002b);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
}

#define SetVLine( hs, he, vs)\
{\
	LCD_WR_REG(0x002a);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_REG(0x002b);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
	LCD_WR_Data(he>>8);\
	LCD_WR_Data(he&0x00ff);\
}

#define SetRect( hs, he, vs, ve)\
{\
	LCD_WR_REG(0x002a);\
	LCD_WR_Data(vs>>8);\
	LCD_WR_Data(vs&0x00ff);\
	LCD_WR_Data(ve>>8);\
	LCD_WR_Data(ve&0x00ff);\
	LCD_WR_REG(0x002b);\
	LCD_WR_Data(hs>>8);\
	LCD_WR_Data(hs&0x00ff);\
	LCD_WR_Data(he>>8);\
	LCD_WR_Data(he&0x00ff);\
}


#define LCD_WIDTH 800
#define LCD_HEIGHT 480

#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xffff
#define COLOR_RED 0xf800
#define COLOR_GREEN 0x07e0
#define COLOR_BLUE 0x001f
#define COLOR_BACKGROUND 0x5dde


void LCD_init(void);
void SetWindows(u16 hs,u16 he,u16 vs,u16 ve);
void LCD_Clean(u16 data);
void LCD_test( void );

void SetColor(u16 type,u16 back);
void SetFont( u16 font );
void SetCursor(u16 x,u16 y);
void Print2LCD(char *p);

#endif /*__LCD_H__*/



/*********************************************************************

*********************************************************************/

#include "stm32f10x.h"
#include "sdcard.h"
#include "stdio.h"
#include "stm32f10x_usart.h"

#include <stdlib.h>
#include <string.h>

#include "fs_api.h"

#include "ssd1963.h"

		
u8 LineBuffer[LCD_WIDTH*2];

static __IO uint32_t SysTicks = 0;

extern void LCD_test(void);
extern void FSMC_LCD_Init(void); 
extern void SetScrollArea(unsigned int top, unsigned int scroll, unsigned int bottom);
extern void SetScrollStart(unsigned int line);

static int time_second=0;
static int time_minute=0;
static int time_hour=0;
void IncrementTicks(void)
{
	SysTicks++;

	if( SysTicks %100 == 0 )
	{
	time_second++;
	if(time_second>=60)
		{
		time_second=0;
		time_minute++;
		 if(time_minute>=60)
		 	{
		 	time_minute=0;
			time_hour++;
			 if(time_hour>=24)
			 	time_hour=0;
		 	}
		}	
	}	
}

unsigned int GetSysTicks(void)
{
	return SysTicks;
}

void GetTime( unsigned char *data  )
{
#if 0//def WIN32
  SYSTEMTIME ti; 
  GetLocalTime(&ti);
  *data++=((ti.wSecond/10)<<4)|(ti.wSecond%10);    //秒               
  *data++=((ti.wMinute/10)<<4)|(ti.wMinute%10);    //分              
  *data++=((ti.wHour/10)<<4)|(ti.wHour%10);    //时      
  *data++=((ti.wDay/10)<<4)|(ti.wDay%10);    //日
  *data++=((ti.wDayOfWeek/10)<<4)|(ti.wDayOfWeek%10);;    //周          
  *data++=((ti.wMonth/10)<<4)|(ti.wMonth%10);    //月
  *data++=(((ti.wYear%100)/10)<<4)|(ti.wYear%10);         //年   
#else
  *data++=time_second;             
  *data++=time_minute;              
  *data++=time_hour;     
  *data++=20;
  *data++=4;          
  *data++=1;
  *data++=11;
#endif
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  { 
    /* Capture error */ 
    while (1);
  }
}

void RCC_Configuration(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
}

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);


  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
  
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Configuration(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_6|GPIO_Pin_3;		 //LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 //LCD 背光控制
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 //LCD-RST
  GPIO_Init(GPIOE, &GPIO_InitStructure);  	
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  //GPIO_Init(GPIOD, &GPIO_InitStructure); 	
  
/*-- GPIO Configuration ------------------------------------------------------*/  
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 


   /* LCD Data(D0-D7) lines configuration */
  
  //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1 ;
  //GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
  //GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* NOE and NWE configuration */  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_4;
  //GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* NE1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* RS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
  /* RST */
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 
  //GPIO_Init(GPIOE, &GPIO_InitStructure);  

  GPIO_SetBits(GPIOD, GPIO_Pin_7);			//CS=1 
  GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
  GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
  GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);		//RESET=0
  GPIO_SetBits(GPIOD, GPIO_Pin_4);		    //RD=1
  GPIO_SetBits(GPIOD, GPIO_Pin_5);			//WR=1
  GPIO_SetBits(GPIOD, GPIO_Pin_13);			//LIGHT

  //GPIO_SetBits(GPIOD, GPIO_Pin_11);			//RS
 
 }

/*************************************************************************
 * Function Name: Serial_Init
 * Description: Init USARTs
 *************************************************************************/
void Serial_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 

/* USART1 configuration ------------------------------------------------------*/
  // USART1 configured as follow:
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure the USART1 */
  USART_Init(USART1, &USART_InitStructure);

  /* Enable the USART Transmoit interrupt: this interrupt is generated when the 
     USART1 transmit data register is empty */  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  /* Enable the USART Receive interrupt: this interrupt is generated when the 
     USART1 receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
  
}


/* Implementation of putchar (also used by printf function to output data)    */
int SendChar (int ch)  {                /* Write character to Serial Port     */

  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
  return (ch);
}


int GetKey (void)  {                    /* Read character from Serial Port    */

  while (!(USART1->SR & USART_FLAG_RXNE));
  return (USART_ReceiveData(USART1));
}



/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}


/*******************************************************************************
* Function Name  : USART_Scanf
* Description    : Gets numeric values from the hyperterminal.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 USART_Scanf(u32 value)
{
  u32 index = 0;
  u32 tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(USART1));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}

//TFT 复位操作
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	Delay(0xAFFFFf);					   
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFFf);	
}


void BAR_demo( void )
{
	int i,j,k;
	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;
	unsigned char red1;
	unsigned char green1;
	unsigned char blue1;	
	u16 data;
	u16 *line_buffer=(u16*)LineBuffer;
		
	SetWindows(0,480-1,0,800-1);
	nCS;
	GramWrite(); 

	for(k=0;k<8;k++)
	{		
	  switch(k)
	      {
		case 0:
			red=31;
			green=0;
			blue=0;
		break;
		
		case 1:
			red=0;
			green=63;
			blue=0;
		break;
		
		case 2:
			red=0;
			green=0;
			blue=31;
		break;
		
		case 3:
			red=31;
			green=63;
			blue=0;
		break;
					
		case 4:
			red=0;
			green=63;
			blue=31;
		break;
		
		case 5:
			red=31;
			green=0;
			blue=31;
		break;
		
		case 6:
			red=31;
			green=63;
			blue=31;
		break;
		
		case 7:
			red=0;
			green=0;
			blue=0;
		break;	
	      }
	      
	  for(j=0;j<800;j++)
	  {
	   red1=(red+1)*j/800;
	   green1=(green+1)*j/800;
	   blue1=(blue+1)*j/800;	
	   line_buffer[j]=(red1<<11)|(green1<<5)|(blue1);
	  }
	  for(i=0;i<60;i++)
	  {
	   for(j=0;j<800;j++)
	   {
	    data=line_buffer[j];	
  	    WriteData(data);
  	   }
  	  }
  	}  
	CS;	
}

void ToggleLED(void)
{
	static int _led_status=0;
	if(_led_status)
		{
		_led_status=0;
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		}
	else
		{
		_led_status=1;
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		}
}

void PutPhoto2LCD(const char * pName)
{
 	FS_FILE *pFile; 
	char FileName[64];
	unsigned int line=0;
	unsigned int temp;
	int i;
	int ret;
	
	strcpy(FileName,"usb:\\");
	strcat(FileName,pName);
	pFile=FS_FOpen(FileName,"rb");
	if( !pFile ) return;

	while(1)
	{
	ret=FS_FRead(LineBuffer,sizeof(LineBuffer),1,pFile);
	if(!ret) break;
	
	SetWindows(line,VDP,0,HDP);
	GramWrite();				
	for(i=0;i<1600;i=i+2)
	{
	 temp=(u16)( LineBuffer[i]<<8)|LineBuffer[i+1];
	 WriteData(temp);
	}
			  
	Delay(0xffff);
	SetScrollArea(0,480,0);
			
	SetScrollStart(line++);
	if(line==480)line=0;
	Delay(0xffff); 
	  
        }

	FS_FClose(pFile);

	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	Delay(0xAFFFF);					   
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	Delay(0xAFFFF);	
	
}

void CopyPhoto2LCD(const char * pName)
{
 	FS_FILE *pFile; 
	char FileName[64];
	unsigned int line=0;
	unsigned int temp;
	int i;
	int ret;
	
	strcpy(FileName,"usb:\\");
	strcat(FileName,pName);
	pFile=FS_FOpen(FileName,"rb");
	if( !pFile ) return;

	SetWindows(line,VDP,0,HDP);
	GramWrite();	
	
	while(1)
	{
	ret=FS_FRead(LineBuffer,sizeof(LineBuffer),1,pFile);
	if(!ret) break;

	for(i=0;i<1600;i=i+2)
	{
	 temp=(u16)( LineBuffer[i]<<8)|LineBuffer[i+1];
	 WriteData(temp);
	}
			  
        }

	FS_FClose(pFile);

	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	Delay(0xAFFFF);					   
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	Delay(0xAFFFF);	
	
}

void PhotoDisplay(const char * pName,int mode)
{ 

   FS_DIR *pDir;
   struct  FS_DIRENT *pDirEnt;
   FS_FILETIME FileTime;
   
   printf("List files in directory %s.\n",pName);
        
   pDir = FS_OpenDir(pName);
   if(pDir) {
      while(1)
      	{
          pDirEnt = FS_ReadDir(pDir);

          if (pDirEnt == NULL) {
             break; /* No more files */
          }
          FS_TimeStampToFileTime(pDirEnt->TimeStamp,&FileTime);
          printf("%16d %04d-%02d-%02d %s\n",pDirEnt->Size,
          FileTime.Year,FileTime.Month,FileTime.Day,pDirEnt->DirName);
		if(mode)
		  CopyPhoto2LCD(pDirEnt->DirName);
		else
		  PutPhoto2LCD(pDirEnt->DirName);
    	};
    FS_CloseDir(pDir);
  }else
  {
     printf("Open directory %s failed.\n",pName);
  }
  
}

void NameDisplay(const char * pName)
{ 

   FS_DIR *pDir;
   struct  FS_DIRENT *pDirEnt;
   FS_FILETIME FileTime;
   char buf[128];

   	LCD_Clean(COLOR_BACKGROUND);
	SetCursor(0,0);
	
   sprintf(buf,"List files in directory %s.\n",pName);
   Print2LCD(buf);
	
   pDir = FS_OpenDir(pName);
   if(pDir) {
      while(1)
      	{
          pDirEnt = FS_ReadDir(pDir);

          if (pDirEnt == NULL) {
             break; /* No more files */
          }
          FS_TimeStampToFileTime(pDirEnt->TimeStamp,&FileTime);
/*		  
          printf("%16d %04d-%02d-%02d %s\n",pDirEnt->Size,
          FileTime.Year,FileTime.Month,FileTime.Day,pDirEnt->DirName);
*/
          sprintf(buf,"%16d %04d-%02d-%02d %s\n",pDirEnt->Size,
          FileTime.Year,FileTime.Month,FileTime.Day,pDirEnt->DirName);

	Print2LCD(buf);
	
    	};
    FS_CloseDir(pDir);
  }else
  {
     sprintf(buf,"Open directory %s failed.\n",pName);
     Print2LCD(buf);	 
  }
  
}



/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
SD_CardInfo SDCardInfo;
SD_Error Status = SD_OK;

#ifdef DEBUG
  debug();
#endif
  
 
  // Clock Config: HSE 72 MHz
  RCC_Configuration();
  
  // Interrupt Config
  NVIC_Configuration();

  /* Configure the systick */    
  SysTick_Configuration();
  
  /* Enable the FSMC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  GPIO_Configuration();
  // USART Config : 115200,8,n,1
  Serial_Init();

  FSMC_LCD_Init();

  LCD_init();

   	LCD_test();

	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);

	BAR_demo();
	
	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);
	
  /*-------------------------- SD Init ----------------------------- */
  Status = SD_Init();

  if (Status == SD_OK)
  {
    /*----------------- Read CSD/CID MSD registers ------------------*/
    Status = SD_GetCardInfo(&SDCardInfo);
  }
  
  if (Status == SD_OK)
  {
    /*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  }
  
  if (Status == SD_OK)
  {
    //Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }
  
  /* Set Device Transfer Mode to DMA */
  if (Status == SD_OK)
  {  
//    Status = SD_SetDeviceMode(SD_DMA_MODE);
//   Status = SD_SetDeviceMode(SD_POLLING_MODE);
   Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);
	printf("\nTEST OK!\n");
  }
  
  	uFS_Init(SDCardInfo.CardCapacity,SDCardInfo.CardBlockSize);

	//MainTask_Dialog(); 
	//MainTask_Gif();
	MainTask();

	while(1)
	{

 	LCD_test();

	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);

	BAR_demo();
	
	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);
	
	NameDisplay("usb:\\");

	Delay(0xafffff);
	Delay(0xafffff);
	Delay(0xafffff);
	
    	PhotoDisplay("usb:\\",0);

    	PhotoDisplay("usb:\\",1);
	}

}







#include "stm32f10x.h"
#include "sdcard.h"
#include "stm32f10x_usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fs_api.h"

#include "ssd1963.h"


u8 LineBuffer[LCD_WIDTH*2];

static __IO uint32_t TimingDelay = 0;

/************************************************************************************************/

#if 1
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
#else
void Delay(uint32_t nCount)
{
  TimingDelay = nCount;
 
  while(TimingDelay != 0)
  {
  }
}
#endif

void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void GPIO_Config(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_6|GPIO_Pin_3;		 //LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 //LCD ±³¹â¿ØÖÆ
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

void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
 
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
 
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void InterruptConfig(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);

  /* Enable the EXTI3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable DMA channel3 IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel =  DMA1_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(SystemFrequency / 100))
  { 
    /* Capture error */ 
    while (1);
  }
}

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

static int _led_status=0;
void ToggleLED(void)
{
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

void AppSystemInit(void)
{
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

#if 0
  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
         | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE);
#else
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
#endif

  /* DMA1 and DMA2 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

  /* Enable the FSMC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

/*------------------- Resources Initialization -----------------------------*/
  /* GPIO Configuration */
  GPIO_Config();

  /* ADC Configuration */
  //ADC_Config();

  /* Interrupt Configuration */
  InterruptConfig();

  /* Configure the systick */    
  SysTick_Configuration();

}

int main(void)
{
SD_CardInfo SDCardInfo;
SD_Error Status = SD_OK;

  AppSystemInit();

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
#if 0	
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
    Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
  }

  if (Status == SD_OK)
  {
    //Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }

  /* Set Device Transfer Mode */
  if (Status == SD_OK)
  {
    Status = SD_SetDeviceMode(SD_DMA_MODE);
  }

  
  	uFS_Init(SDCardInfo.CardCapacity,SDCardInfo.CardBlockSize);
#endif

	//MainTask_Dialog(); 
	//MainTask_Gif();
	MainTask();

#if 0
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
#endif

}



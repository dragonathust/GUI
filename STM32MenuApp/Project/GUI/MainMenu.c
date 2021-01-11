/***********************************************************
*
*
*/

#include <stdio.h>
#include <string.h>

#include "GUI.h"
#include "LCDConf.h"
#include "FRAMEWIN.h"
#include "MENU.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "LISTVIEW.h"
#include "PROGBAR.h"
#include "GUI_GIF.h"

//#define SIMULATOR

#include "chinese_16s.h"
#include "res.h"

#ifdef SIMULATOR
#include "bk.c"
#endif

#include "logo.c"

#include "ConfigInfo.h"

#define DESKTOP_COLOR GUI_DARKGREEN
#define BACKGROUND_COLOR 0xc0c0c0

#define GUI_ID_ABOUT (GUI_ID_USER+1)
#define GUI_ID_EDIT (GUI_ID_USER+2)
#define GUI_ID_ARROW (GUI_ID_USER+3)
#define GUI_ID_LOGO (GUI_ID_USER+4)

#define GUI_ID_CONFIG_VIEW1 (GUI_ID_USER+5)
#define GUI_ID_CONFIG_VIEW2 (GUI_ID_USER+6)
#define GUI_ID_CONFIG_VIEW3 (GUI_ID_USER+7)
#define GUI_ID_CONFIG_VIEW4 (GUI_ID_USER+8)
#define GUI_ID_CONFIG_VIEW5 (GUI_ID_USER+9)
#define GUI_ID_CONFIG_VIEW6 (GUI_ID_USER+10)

#define GUI_ID_PRINT (GUI_ID_USER+11)
#define GUI_ID_SAVE_TO_DISK (GUI_ID_USER+12)
#define GUI_ID_SWITCH (GUI_ID_USER+13)
#define GUI_ID_SAVE_TO_TABLE (GUI_ID_USER+14)

#define GUI_ID_CAPS (GUI_ID_USER+15)
#define GUI_ID_LEFT (GUI_ID_USER+16)
#define GUI_ID_RIGHT (GUI_ID_USER+17)

#define GUI_ID_EDIT1_0 (GUI_ID_USER+100)
#define GUI_ID_EDIT2_0 (GUI_ID_USER+200)
#define GUI_ID_EDIT3_0 (GUI_ID_USER+300)
#define GUI_ID_EDIT4_0 (GUI_ID_USER+400)
#define GUI_ID_EDIT5_0 (GUI_ID_USER+500)
#define GUI_ID_EDIT6_0 (GUI_ID_USER+600)

#define GUI_ID_NUMPAD_NUM (GUI_ID_USER+700)
#define GUI_ID_KEYPAD_NUM (GUI_ID_USER+800)

//#define PASSWORD_CHECK

/*********************************************************************
*
*       static data, bitmaps
*
**********************************************************************
*/
static CONFIG_INFO _config_info;

/* Bitmap data for arrow keys */
static GUI_CONST_STORAGE GUI_COLOR _aColorsArrow[] = {
  0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalArrow = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &_aColorsArrow[0]
};

static GUI_CONST_STORAGE unsigned char _acArrowRight[] = {
  ____XX__, ________,
  ____XXXX, ________,
  XXXXXXXX, XX______,
  ____XXXX, ________,
  ____XX__, ________
};

static GUI_CONST_STORAGE unsigned char _acArrowLeft[] = {
  ____XX__, ________,
  __XXXX__, ________,
  XXXXXXXX, XX______,
  __XXXX__, ________,
  ____XX__, ________
};


static GUI_CONST_STORAGE GUI_BITMAP _bmArrowRight = {
  10,           /* XSize */
  5,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowRight, /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowLeft = {
  10,           /* XSize */
  5,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowLeft,  /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};

/*********************************************************************
*
*       static code, window callbacks
*
**********************************************************************
*/

void LoadConfigInfo(CONFIG_INFO *data)
{

	memset(data,0,sizeof(CONFIG_INFO) );
	strcpy(data->model,"MODEL:ABCD");
	strcpy(data->serial_no,"SN:00010002");
	strcpy(data->vendor_name,"Vendor");
	strcpy(data->installation_date,"2011/1/1");	

	data->weight_max=6000;
	data->range_max=500;
	data->height_stop_up=500;
	data->height_stop_down=0;

	data->password_menu=123456;
	data->password_setup=123456;
	data->user_defined_no=1;
	
}

void SaveConfigInfo(CONFIG_INFO *data)
{

}

static const char * _aTable_1[][13] = {
 { "0001", "12:23", "11/1/3","12m","2300kg","2600kg","56N.M","46N.M","00:02","2","66m",RES12_14,"113%" },
 { "0002", "12:28", "11/1/3","12m","2300kg","2600kg","56N.M","46N.M","00:02","2","66m",RES12_15,"113%" },

};

static const char * _aTable_2[][13] = {
 { "0001", "12:23", "11/1/3","-","-","-","-","-","-","-","-","-",RES15_6 },
 { "0002", "12:28", "11/1/3","12m","2300kg","2600kg","56N.M","46N.M","00:02","2","66m",RES12_15,"113%" },

};

static const char * _aTable_3[][4] = {
 {  "1:23", "1:23",  "1:23", "1:23"}

};

void LoadOverLoadRecord(void **p,int * num)
{
  *p=(void*)_aTable_1;
  *num=GUI_COUNTOF(_aTable_1);
}

void LoadOperationHistoryRecord(void **p,int * num)
{
  *p=(void*)_aTable_2;
  *num=GUI_COUNTOF(_aTable_2);
}

void LoadRunTimeRecord(void **p,int * num)
{
  *p=(void*)_aTable_3;
  *num=GUI_COUNTOF(_aTable_3);
}

#ifdef SIMULATOR
void GetTime( unsigned char *data  )
{
static int time_second=0;
static int time_minute=0;
static int time_hour=0;

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
#else
void GetTime( unsigned char *data  );
#endif

unsigned char *WeekTable[]={RES1_3,RES1_10,RES1_11,RES1_12,RES1_13,RES1_14,RES1_15};
/*日一二三四五六*/
void PrintLocalTime(int x,int y)
{
    unsigned char Data[7];
    char time[128];

    GetTime(Data);	
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Font16B_ASCII);
    sprintf(time,"20%02d/%d/%d  %02d:%02d:%02d",
		Data[6],Data[5],Data[3],Data[2],Data[1],Data[0]);
    GUI_DispStringAt(time, x,y);	  

    GUI_SetFont(&GUI_FontHZ16S);
    sprintf(time,RES1_9_N"%s",WeekTable[Data[4]]);	
    GUI_DispStringAt(time, x+130,y);		
}

/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {

  switch (pMsg->MsgId) {
  case WM_PAINT:
    //GUI_SetBkColor(DESKTOP_COLOR);
    //GUI_Clear();
#ifdef SIMULATOR    
    GUI_GIF_Draw(acbk,sizeof(acbk),0,0);	
#endif    
    //GUI_JPEG_Draw(acbk,sizeof(acbk),0,0);

    break;
  default:
    WM_DefaultProc(pMsg);
  }
}


/*******************************************************************
*
*       _cbAboutBox
*/
static void _cbAboutBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
      GUI_DrawBitmap(&bmlogo, 10, 10);
	  
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_FontHZ16S);
      GUI_SetTextMode(GUI_TM_TRANS);

      GUI_DispStringHCenterAt(RES4_12, 160, 90);
      GUI_DispStringHCenterAt("(c)2010 "RES4_10, 160, 120);

      GUI_SetFont(&GUI_Font16B_ASCII);	  
      GUI_DispStringHCenterAt("Compiled " __DATE__ " "__TIME__, 160, 150);
      GUI_DispStringHCenterAt("Programmed by xiaolong yi", 160, 180);
    }
    break;
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      GUI_EndDialog(hWin, 1);
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static _CreateAboutBox(void)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  
  hFrameWin = FRAMEWIN_Create("About", _cbAboutBox, WM_CF_SHOW, 240, 80, 320, 300);
  //FRAMEWIN_AddCloseButton(hFrameWin, FRAMEWIN_BUTTON_LEFT,  0);
  //FRAMEWIN_AddMaxButton  (hFrameWin, FRAMEWIN_BUTTON_RIGHT, 0);
  //FRAMEWIN_AddMinButton  (hFrameWin, FRAMEWIN_BUTTON_RIGHT, 1);
  FRAMEWIN_SetResizeable (hFrameWin, 1);
  FRAMEWIN_SetActive(hFrameWin, 1);
  FRAMEWIN_SetMoveable(hFrameWin, 1);	

  /* Create dialog items */
  hItem = BUTTON_CreateEx(120, 240, 60, 20, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, "Ok");
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}

#include "MessageBox.c"
#include "NumPad.c"
#include "KeyPad.c"
#include "ConfigView1.c"
#include "ConfigView2.c"
#include "ConfigView3.c"
#include "ConfigView4.c"
#include "ConfigView5.c"
#include "ConfigView6.c"
#include "ConfigView.c"
#include "MainView.c"

static GUI_PID_STATE _State;
static GUI_TIMER_HANDLE _Timer;

/*********************************************************************
*
*         _NotifyMouseState
*
* Informs the application that the mouse state has changed
*/
static void _NotifyMouseState(void) {
  if (_State.Pressed) {
    GUI_TOUCH_StoreState(_State.x, _State.y);
  } else {
    GUI_TOUCH_StoreState(-1, -1);
  }
  GUI_MOUSE_StoreState(&_State);
}

static int _load_cnt;
static int _load_neg;

static int _click_cnt;
static int _click_map[][2]={
	{760,240},
	{370,260},
	{700,360},
	{360,330},

	{60,120},
	{700,350},
	{120,100},
	{700,360},
	{640,420},
	
	{60,160},
	{700,350},
	{640,420},

	{60,200},
	{700,350},
	{640,420},

	{60,240},
	{700,350},
	{640,420},

	{60,280},
	{700,350},
	{640,420},

	{60,320},
	{700,350},
	{640,420},

	{700,420},
	
	{0,0}
};

static void _cbTimer(GUI_TIMER_MESSAGE* pTM)
{
    unsigned char Data[7];
	
	_load_cnt+=_load_neg;
	if (_load_cnt<0)
	{
	_load_cnt=0;
	_load_neg=_load_neg*(-1);
	}

	if(_load_cnt>500)
	{
	_load_cnt=500;
	_load_neg=_load_neg*(-1);
	}

	UpdateLocalTime();
	SetLoadPercentageValue(_load_cnt/5);
	SetWorkStatusValue(_load_cnt,_load_cnt,_load_cnt);

#if 1
	GetTime(Data);
	if( (Data[2]!=0 ) ||( Data[1]!=0 ) )
	{

  	if(_State.Pressed == 1)
  	{	_State.Pressed=0;
    	_NotifyMouseState();  
  	}
	else	if( _load_cnt%10==0)
	{
  		if((_click_map[_click_cnt][0]==0) )
  		{
  		_click_cnt=0;
  		}
  		else
  		{
		_State.x=_click_map[_click_cnt][0]+10;
  		_State.y=_click_map[_click_cnt][1]+10;
  		_State.Pressed=1;
  		_click_cnt++;
    		_NotifyMouseState();  
  		}
	}else
	{
	_State.x+=2;
	_State.y+=2;
    	_NotifyMouseState();  
	}

	}
#endif

	GUI_TIMER_Restart(_Timer);
}

void MainTask(void)
//void MainTask_Menu(void)
{
  _load_cnt=0;
  _load_neg=1;

  _State.x=0;
  _State.y=0;
  _State.Pressed=0;
  _click_cnt=0;
  
  LoadConfigInfo(&_config_info);
  	
  GUI_Init();

  GUIDEMO_main();

  WM_SetDesktopColor(DESKTOP_COLOR);
  WM_SetCreateFlags(WM_CF_MEMDEV); 
  
  /* Set callback for background */
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);  

  GUI_Delay(1000);
  
  BUTTON_SetDefaultFont(&GUI_FontHZ16S);
  LISTVIEW_SetDefaultFont(&GUI_FontHZ16S);

 _CreateMainView();
 SetWorkStatusValue(0,0,0);

  GUI_CURSOR_Show();
  _Timer=GUI_TIMER_Create(_cbTimer,GUI_GetTime()+1000,0,0);
  GUI_TIMER_SetPeriod(_Timer,100);

    while (1) 
	{
	        GUI_Delay(10);
	}
}



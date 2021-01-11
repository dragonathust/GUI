
#include "main_bk.c"

#include "wheel.c"
#include "load.c"

#include "left_arrow_dark.c"
#include "left_arrow_bright.c"

static   WM_HWIN _hFrameWin;

static int load_range_pixel=0;
static int load_height_pixel=0;

static int load_range=0;
static int load_height=0;
static int load_weight=0;

void SetLoadPercentageValue(int v)
{
  WM_HWIN hItem;

    if(!_hFrameWin) return;
	
    hItem=WM_GetDialogItem(WM_GetClientWindow(_hFrameWin),GUI_ID_PROGBAR0);
	
     if(v>=80)
      PROGBAR_SetBarColor(hItem, 1, GUI_RED);
     else if(v>=60)
      PROGBAR_SetBarColor(hItem, 1, GUI_YELLOW);
     else
      PROGBAR_SetBarColor(hItem, 1, GUI_LIGHTGRAY);
	 
      PROGBAR_SetValue(hItem, v);
}

void SetWorkStatusValue(int range,int height,int weight)
{
  GUI_RECT rect;
  
  int range_percent,height_percent;

  if(!_hFrameWin) return;
	  
  range_percent=(range*_config_info.range_max)/500;
  height_percent=(height*(_config_info.height_stop_up-_config_info.height_stop_down))/500;

/*240->720*/
  load_range_pixel=240+4*range_percent/5;
/*160->360*/
  load_height_pixel=160+2*height_percent/5;

  load_range=range;
  load_height=height;	
  load_weight=weight;

  rect.x0=load_range_pixel-24;
  rect.x1=load_range_pixel+96;
  rect.y0=115;
  rect.y1=load_height_pixel+72;

  WM_InvalidateRect(WM_GetClientWindow(_hFrameWin), &rect);
  //WM_InvalidateWindow(WM_GetClientWindow(_hFrameWin));
}

void UpdateLocalTime(void)
{
  static int second=-1;
  unsigned char Data[7];
  GUI_RECT rect;

  if(!_hFrameWin) return;
	
  GetTime(Data);	
  if( Data[0] != second )
  {
  second = Data[0];
  rect.x0=560;
  rect.x1=680;
  rect.y0=20;
  rect.y1=36;  
  WM_InvalidateRect(WM_GetClientWindow(_hFrameWin), &rect);
  }
  
}

/*********************************************************************
*
*       _cbTriangleButton
*/
static void _cbTriangleButton(WM_MESSAGE* pMsg) {
  int Index;
  GUI_RECT Rect;
	
  switch (pMsg->MsgId) {
  case WM_PAINT:
      Index = (WIDGET_GetState(pMsg->hWin) & BUTTON_STATE_PRESSED) ? 1 : 0;

      WM_GetClientRect(&Rect);
      GUI_SetColor(BUTTON_GetBkColor(pMsg->hWin, Index));
      GUI_FillRect(Rect.x0,Rect.y0,Rect.x1,Rect.y1);

      if(Index)
      GUI_DrawBitmap(&bmleft_arrow_bright, 0, 0);  
      else
      GUI_DrawBitmap(&bmleft_arrow_dark, 0, 0);  
	  
      break;

  default:
    BUTTON_Callback(pMsg);
  }
}


/*******************************************************************
*
*       _cbMainView
*/
static void _cbMainView(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  char buffer[128];
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    GUI_DrawBitmap(&bmmain_bk, 0, 0);			

    bmwheel.YSize=load_height_pixel-120+26;

    GUI_DrawBitmap(&bmwheel, load_range_pixel, 115);
    GUI_DrawBitmap(&bmload, load_range_pixel-7, load_height_pixel);

    PrintLocalTime(560,20);

    GUI_SetColor(GUI_WHITE);		
    GUI_SetFont(&GUI_Font24_ASCII);

    sprintf(buffer,"%d.%dm",load_range_pixel/10,load_range_pixel%10);	
    GUI_DispStringAt(buffer, load_range_pixel+30,130);	

    sprintf(buffer,"%d.%dm",load_height_pixel/10,load_height_pixel%10);	
    GUI_DispStringAt(buffer, load_range_pixel+30,load_height_pixel+20);	

    sprintf(buffer,"%d.%dt",load_weight/10,load_weight%10);	
    GUI_DispStringAt(buffer, load_range_pixel,load_height_pixel+50);	
	
    GUI_SetFont(&GUI_FontHZ16S);
    sprintf(buffer,RES15_1_N":%dm/s",3);	
    GUI_DispStringAt(buffer, 20,320);	

    sprintf(buffer,RES15_2_N":%ddeg",85);	
    GUI_DispStringAt(buffer, 20,350);	

    sprintf(buffer,RES15_3_N":%d",2);	
    GUI_DispStringAt(buffer, 20,380);	

    sprintf(buffer,RES15_4_N":%s",RES15_6);		
    GUI_DispStringAt(buffer, 20,410);	

    sprintf(buffer,RES15_5_N":%s",RES15_7);		
    GUI_DispStringAt(buffer, 20,440);		

    GUI_DispStringAt(RES15_8, 480,440);
    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_OK:
			GUI_EndDialog(hWin, 1);
			break;

		case GUI_ID_EDIT:
			//GUI_EndDialog(hWin, 0);
			_CreateMessageBox(RES5_11,RES5_10);
			_CreateConfigView();
			break;
			
		default:
			break;
	}
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static _CreateMainView(void)
{
  //WM_HWIN hFrameWin;
  WM_HWIN hItem;
  PROGBAR_Handle hProgBar;
 
  _hFrameWin = FRAMEWIN_Create("MainView", _cbMainView, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(_hFrameWin,0);
  FRAMEWIN_SetClientColor(_hFrameWin,GUI_BLACK);
  FRAMEWIN_SetActive(_hFrameWin, 1);

  /* Create dialog items */
/*  
  hItem = BUTTON_CreateEx(700, 420, 60, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES4_6);
*/

  hItem = BUTTON_CreateEx(800-48, 240, 48, 48, WM_GetClientWindow(_hFrameWin), WM_CF_SHOW, 0, GUI_ID_EDIT);
  WM_SetCallback(hItem,_cbTriangleButton);

  hProgBar=PROGBAR_CreateAsChild(560,440,200,24, WM_GetClientWindow(_hFrameWin),GUI_ID_PROGBAR0,WM_CF_SHOW);
  PROGBAR_SetBarColor(hProgBar, 0, GUI_GREEN);
  PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
  PROGBAR_SetMinMax(hProgBar, 0, 100);
}


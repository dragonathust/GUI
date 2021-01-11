/*******************************************************************
*
*       _cbNumPad
*/

#include "yes_48.c"
#include "no_48.c"

#define NUMPAD_SIZE 48
#define NUMPAD_FONT GUI_FontD24x32
#define NUMPAD_X_OFFSET 30
#define NUMPAD_Y_OFFSET 110
#define NUMPAD_X_SPACE 64
#define NUMPAD_Y_SPACE 64

#define NUMPAD_INPUT_LEN 16

static char *_NumPadTitle;
static  EDIT_Handle _hEdit_numpad;
static int _Value_numpad;

static int _cnt_numpad;
	
static void _cbNumPad(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  char aBuffer[NUMPAD_INPUT_LEN];

  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    GUI_SetColor(GUI_DARKBLUE);			
    GUI_SetFont(&GUI_FontHZ16S);		
    GUI_DispStringHCenterAt(_NumPadTitle, 120, 10);
    GUI_DispStringHCenterAt(RES4_11, 120, 35);
	
    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(0,90,240,93);	
    }
    break;
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_CLOSE:
      			GUI_EndDialog(hWin, 1);			
			break;

		default:
			if( (Id>=GUI_ID_NUMPAD_NUM)&&(Id<=GUI_ID_NUMPAD_NUM+9) )
				{
				 _cnt_numpad++;
				 	if( _cnt_numpad<NUMPAD_INPUT_LEN )
				 	{
				 		memset(aBuffer,'*',_cnt_numpad);
						aBuffer[_cnt_numpad]=0;
				 		EDIT_SetText(_hEdit_numpad, aBuffer);
				 		_Value_numpad = _Value_numpad*10+Id-GUI_ID_NUMPAD_NUM;
				 	}	
				}
			break;
	}
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static char* numpad_string[]={"1","2","3","4","5","6","7","8","9"};

static int _CreateNumPad(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  int i;


  _NumPadTitle=title;
  _Value_numpad=0;
  _cnt_numpad=0;
  
  hFrameWin = FRAMEWIN_Create("NumPad", _cbNumPad, WM_CF_SHOW, 540,50, 240, 360);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  for(i=0;i<9;i++)
  {
  hItem = BUTTON_CreateEx(NUMPAD_X_OFFSET+(i%3)*NUMPAD_X_SPACE, NUMPAD_Y_OFFSET+(i/3)*NUMPAD_Y_SPACE, 
  	NUMPAD_SIZE,NUMPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_NUMPAD_NUM+i+1);
  BUTTON_SetBkColor(hItem,0,GUI_BLACK);
  BUTTON_SetTextColor(hItem,0,GUI_WHITE);
  BUTTON_SetFont(hItem,&NUMPAD_FONT);
  BUTTON_SetText(hItem, numpad_string[i]);
  }

  hItem = BUTTON_CreateEx(NUMPAD_X_OFFSET+NUMPAD_X_SPACE, NUMPAD_Y_OFFSET+3*NUMPAD_Y_SPACE,
  	NUMPAD_SIZE, NUMPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_NUMPAD_NUM);
  BUTTON_SetBkColor(hItem,0,GUI_BLACK);
  BUTTON_SetTextColor(hItem,0,GUI_WHITE);
  BUTTON_SetFont(hItem,&NUMPAD_FONT);
  BUTTON_SetText(hItem, "0");

  hItem = BUTTON_CreateEx(NUMPAD_X_OFFSET+2*NUMPAD_X_SPACE, NUMPAD_Y_OFFSET+3*NUMPAD_Y_SPACE,
  	NUMPAD_SIZE, NUMPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CLOSE);
  BUTTON_SetBitmapEx(hItem, 0, &bmyes_48, 0, 0);
  BUTTON_SetBitmapEx(hItem, 1, &bmyes_48, 0, 0);  

  hItem = BUTTON_CreateEx(NUMPAD_X_OFFSET, NUMPAD_Y_OFFSET+3*NUMPAD_Y_SPACE,
  	NUMPAD_SIZE, NUMPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CLOSE);
  BUTTON_SetBitmapEx(hItem, 0, &bmno_48, 0, 0);
  BUTTON_SetBitmapEx(hItem, 1, &bmno_48, 0, 0);  

    /* Create edit widget */
  _hEdit_numpad = EDIT_CreateAsChild( 40, 65, 160, 24, hFrameWin,' ', 0,NUMPAD_INPUT_LEN);
  /* Modify edit widget */
  EDIT_SetText(_hEdit_numpad, "");
  EDIT_SetFont(_hEdit_numpad, &GUI_Font8x16);
  EDIT_SetTextColor(_hEdit_numpad, 0, GUI_BLACK);
  
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);

  return _Value_numpad;
  
}


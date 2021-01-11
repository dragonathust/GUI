
void EDIT__Callback (WM_MESSAGE * pMsg) ;

/*******************************************************************
*
*       _cbKeyPad
*/

#define KEYPAD_SIZE 48
#define KEYPAD_FONT GUI_Font32B_ASCII
#define KEYPAD_X_OFFSET 10
#define KEYPAD_Y_OFFSET 60
#define KEYPAD_X_SPACE 64
#define KEYPAD_Y_SPACE 64

static char *_KeyPadTitle;
static  EDIT_Handle _hEdit_keypad;
static int _caps_lock;

static char* keypad_string[]={
"1","2","3","4","5","6","7","8","9","0",\
"Q","W","E","R","T","Y","U","I","O","P",\
"A","S","D","F","G","H","J","K","L",\
"Z","X","C","V","B","N","M"\
};

static char* keypad_string_little[]={
"1","2","3","4","5","6","7","8","9","0",\
"q","w","e","r","t","y","u","i","o","p",\
"a","s","d","f","g","h","j","k","l",\
"z","x","c","v","b","n","m"\
};

static char keypad_char[]={
'1','2','3','4','5','6','7','8','9','0',\
'Q','W','E','R','T','Y','U','I','O','P',\
'A','S','D','F','G','H','J','K','L',\
'Z','X','C','V','B','N','M'\
};

static char keypad_char_little[]={
'1','2','3','4','5','6','7','8','9','0',\
'q','w','e','r','t','y','u','i','o','p',\
'a','s','d','f','g','h','j','k','l',\
'z','x','c','v','b','n','m'\
};

static void _cbKeyPad(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  WM_HWIN hItem;
  WM_MESSAGE Msg;
  WM_KEY_INFO Info;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    GUI_SetColor(GUI_DARKBLUE);			
    GUI_SetFont(&GUI_FontHZ16S);		
    GUI_DispStringHCenterAt(_KeyPadTitle, 120, 10);
	
    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(0,40,650,43);	
    }
    break;
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_OK:
			WM_DetachWindow(_hEdit_keypad);
      			GUI_EndDialog(hWin, 0);			
			break;
			
		case GUI_ID_CANCEL:
			WM_DetachWindow(_hEdit_keypad);
      			GUI_EndDialog(hWin, 1);			
			break;

  		case GUI_ID_CAPS:
			hItem=WM_GetDialogItem(hWin,GUI_ID_CAPS);
			if(_caps_lock)
				{
				  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
				_caps_lock=0;
				}
			else
				{
				  BUTTON_SetBkColor(hItem,0,GUI_LIGHTCYAN);
				_caps_lock=1;
				}
			break;

		case GUI_ID_LEFT:
    				Info.Key = GUI_KEY_LEFT;
    				Info.PressedCnt = 1;
    				Msg.MsgId = WM_KEY;
    				Msg.Data.p = &Info;
				WM_SetFocus(_hEdit_keypad);	
				WM_SendMessage(_hEdit_keypad, &Msg);
			break;	

		case GUI_ID_RIGHT:
			    	Info.Key = GUI_KEY_RIGHT;
    				Info.PressedCnt = 1;
    				Msg.MsgId = WM_KEY;
    				Msg.Data.p = &Info;
				WM_SetFocus(_hEdit_keypad);	
				WM_SendMessage(_hEdit_keypad, &Msg);
			break;
			
		default:
			if( (Id>=GUI_ID_KEYPAD_NUM)&&(Id<=GUI_ID_KEYPAD_NUM+35) )
				{
				if( _caps_lock )
    				Info.Key = keypad_char[Id-GUI_ID_KEYPAD_NUM];
				else
    				Info.Key = keypad_char_little[Id-GUI_ID_KEYPAD_NUM];
				
    				Info.PressedCnt = 1;
    				Msg.MsgId = WM_KEY;
    				Msg.Data.p = &Info;
				WM_SetFocus(_hEdit_keypad);	
				WM_SendMessage(_hEdit_keypad, &Msg);
				}
			break;
	}
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void _CreateKeyPad(char *title,EDIT_Handle hEdit)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  int i;


  _KeyPadTitle=title;
  _caps_lock=0;
  _hEdit_keypad=hEdit;

  hFrameWin = FRAMEWIN_Create("KeyPad", _cbKeyPad, WM_CF_SHOW, 100,100, 650, 320);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  for(i=0;i<10;i++)
  {
  hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+i*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_KEYPAD_NUM+i);
  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
  BUTTON_SetTextColor(hItem,0,GUI_BLACK);
  BUTTON_SetFont(hItem,&KEYPAD_FONT);
  BUTTON_SetText(hItem, keypad_string[i]);
  }

  for(i=0;i<10;i++)
  {
  hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+i*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_KEYPAD_NUM+i+10);
  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
  BUTTON_SetTextColor(hItem,0,GUI_BLACK);
  BUTTON_SetFont(hItem,&KEYPAD_FONT);
  BUTTON_SetText(hItem, keypad_string[i+10]);
  }

    for(i=0;i<9;i++)
  {
  hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+i*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+2*KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_KEYPAD_NUM+i+20);
  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
  if( (i==3)||(i==6) )
  BUTTON_SetTextColor(hItem,0,GUI_RED);
  else  	
  BUTTON_SetTextColor(hItem,0,GUI_BLACK);
  BUTTON_SetFont(hItem,&KEYPAD_FONT);
  BUTTON_SetText(hItem, keypad_string[i+20]);
  }
	
  for(i=0;i<7;i++)
  {
  hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+i*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+3*KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_KEYPAD_NUM+i+29);
  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
  BUTTON_SetTextColor(hItem,0,GUI_BLACK);
  BUTTON_SetFont(hItem,&KEYPAD_FONT);
  BUTTON_SetText(hItem, keypad_string[i+29]);
  }

    hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+7*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+3*KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE*2,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CAPS);
  BUTTON_SetBkColor(hItem,0,BACKGROUND_COLOR);
  BUTTON_SetTextColor(hItem,0,GUI_BLACK);
  BUTTON_SetFont(hItem,&KEYPAD_FONT);
  BUTTON_SetText(hItem, "Caps");

    hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+9*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+3*KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetBitmapEx(hItem, 0, &bmyes_48, 0, 0);
  BUTTON_SetBitmapEx(hItem, 1, &bmyes_48, 0, 0);  

    hItem = BUTTON_CreateEx(KEYPAD_X_OFFSET+9*KEYPAD_X_SPACE, KEYPAD_Y_OFFSET+2*KEYPAD_Y_SPACE, 
  	KEYPAD_SIZE,KEYPAD_SIZE, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CANCEL);
  BUTTON_SetBitmapEx(hItem, 0, &bmno_48, 0, 0);
  BUTTON_SetBitmapEx(hItem, 1, &bmno_48, 0, 0);  
  
  /* Create edit widget */
  WM_SetCallback(hEdit, EDIT__Callback);   
  WM_AttachWindowAt(hEdit,hFrameWin,400, 10);
  WM_SetFocus(hEdit);
  
  /* Exec modal dialog */
  //WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);

  return ;
}

static void _cbEdit(WM_MESSAGE * pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  WM_HWIN hParent;
  GUI_RECT rect;
  
  switch (pMsg->MsgId) {

  case WM_TOUCH:
    if (pMsg->Data.p) {  /* Something happened in our area (pressed or released) */
      const GUI_PID_STATE* pState;
      pState = (const GUI_PID_STATE*)pMsg->Data.p;
      if (!pState->Pressed) {
	hParent=WM_GetParent(hWin);
	WM_GetWindowRectEx(hWin,&rect);
	_CreateKeyPad("Please input:",hWin);
    	WM_SetCallback(hWin, _cbEdit);    	
	WM_AttachWindowAt(hWin,hParent,rect.x0, rect.y0);
      }
    }
    break;
    default:
	EDIT__Callback(pMsg);	
	break;	
  }

}



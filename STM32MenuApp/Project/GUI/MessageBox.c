
/*******************************************************************
*
*       _cbMessageBox
*/

static char *_message_MessageBox;
static char *_title_MessageBox;

static void _cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_FontHZ16S);

      if(_title_MessageBox)
      GUI_DispStringHCenterAt(_title_MessageBox, 160, 40);	  	
      GUI_DispStringHCenterAt(_message_MessageBox, 160, 80);
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

static _CreateMessageBox(char *message,char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;

  _message_MessageBox=message;
  _title_MessageBox=title;
  	
  hFrameWin = FRAMEWIN_Create("MessageBox", _cbMessageBox, WM_CF_SHOW, 240, 120, 320, 200);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);

  /* Create dialog items */
  hItem = BUTTON_CreateEx(130, 140, 60, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES4_2);
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


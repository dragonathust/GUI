

static char *_title_Config4View;

#define EDIT4_OFFSET 280

static void _Config4Done(WM_HWIN hWin)
{
  WM_HWIN hItem;
  int i;

  for(i=0;i<6;i++)
  {
    hItem=WM_GetDialogItem(hWin,GUI_ID_EDIT4_0+i);
	switch(i)
		{
			case 0:
				EDIT_GetText(hItem,_config_info.phone_num1,INFO_STRING_LEN);
				break;

			case 1:
				EDIT_GetText(hItem,_config_info.phone_num2,INFO_STRING_LEN);
				break;

			case 2:
				EDIT_GetText(hItem,_config_info.phone_num3,INFO_STRING_LEN);
				break;

			case 3:
				EDIT_GetText(hItem,_config_info.address1,INFO_STRING_LEN);
				break;				

			case 4:
				EDIT_GetText(hItem,_config_info.address2,INFO_STRING_LEN);
				break;

			case 5:
				EDIT_GetText(hItem,_config_info.address3,INFO_STRING_LEN);
				break;

		}
  }
  
}


/*******************************************************************
*
*       _cbConfig4View
*/
static void _cbConfig4View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config4View, 400,40);

    GUI_SetColor(GUI_BLACK);	
    GUI_DispStringAt(RES9_1, 80,90);
    GUI_DispStringAt(RES9_2, 120,120);
    GUI_DispStringAt(RES9_3, 120,150);
    GUI_DispStringAt(RES9_4, 120,180);	
    GUI_DispStringAt(RES9_5, 80,210);
    GUI_DispStringAt(RES9_6, 120,240);
    GUI_DispStringAt(RES9_7, 120,270);
    GUI_DispStringAt(RES9_8, 120,300);	

    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
			
		case GUI_ID_OK:
			_Config4Done(hWin);
      			GUI_EndDialog(hWin, 0);			
			break;

		case GUI_ID_CANCEL:
      			GUI_EndDialog(hWin, 1);			
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

static void _CreateConfig4View(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  EDIT_Handle hEdit;
  int i;
  
  _title_Config4View=title;
	
  hFrameWin = FRAMEWIN_Create("Config4View", _cbConfig4View, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  hItem = BUTTON_CreateEx(80, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CANCEL);
  BUTTON_SetText(hItem, RES8_14);
  
  hItem = BUTTON_CreateEx(640, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES8_15);


  /* Create edit widget */
  EDIT_SetDefaultFont(&GUI_Font8x16);
  EDIT_SetDefaultTextColor(0, GUI_BLACK);
  
  hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 120, 120, 24, hFrameWin,GUI_ID_EDIT4_0, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.phone_num1);

  hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 150, 120, 24, hFrameWin,GUI_ID_EDIT4_0+1, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.phone_num2);

  hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 180, 120, 24, hFrameWin,GUI_ID_EDIT4_0+2, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.phone_num3);

   hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 240, 120, 24, hFrameWin,GUI_ID_EDIT4_0+3, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.address1);

  hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 270, 120, 24, hFrameWin,GUI_ID_EDIT4_0+4, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.address2);

  hEdit = EDIT_CreateAsChild( EDIT4_OFFSET, 300, 120, 24, hFrameWin,GUI_ID_EDIT4_0+5, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.address3);

   for(i=0;i<6;i++)
  {
      hItem=WM_GetDialogItem(hFrameWin,GUI_ID_EDIT4_0+i);
      WM_SetCallback(hItem,_cbEdit);
  }

   
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


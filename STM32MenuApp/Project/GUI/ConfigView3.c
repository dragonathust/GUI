

static char *_title_Config3View;
static int _control_switch;

#define EDIT3_OFFSET 280

static void _Config3Done(WM_HWIN hWin)
{
  WM_HWIN hItem;
  int i;

  for(i=0;i<7;i++)
  {
    hItem=WM_GetDialogItem(hWin,GUI_ID_EDIT3_0+i);
	switch(i)
		{
			case 0:
				_config_info.range_stop_out=EDIT_GetValue(hItem);
				break;

			case 1:
				_config_info.range_stop_in=EDIT_GetValue(hItem);
				break;

			case 2:
				_config_info.height_stop_up=EDIT_GetValue(hItem);
				break;

			case 3:
				_config_info.height_stop_down=EDIT_GetValue(hItem);
				break;				

			case 4:
				_config_info.angle_stop_clockwise=EDIT_GetValue(hItem);
				break;

			case 5:
				_config_info.angle_stop_anti_clockwise=EDIT_GetValue(hItem);
				break;

			case 6:
				_config_info.angle_stop_pre=EDIT_GetValue(hItem);
				break;
				
		}
  }

    _config_info.control_switch=_control_switch;
}

/*******************************************************************
*
*       _cbConfig3View
*/
static void _cbConfig3View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  WM_HWIN hItem;
  int Id;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config3View, 400,40);

    GUI_SetColor(GUI_BLACK);	
    GUI_DispStringAt(RES8_1, 40,90);
    GUI_DispStringAt(RES8_2, 40,120);
    GUI_DispStringAt(RES8_3, 120,150);
    GUI_DispStringAt(RES8_4, 120,180);	
    GUI_DispStringAt(RES8_5, 40,210);
    GUI_DispStringAt(RES8_6, 120,240);
    GUI_DispStringAt(RES8_7, 120,270);
    GUI_DispStringAt(RES8_8, 40,300);	
    GUI_DispStringAt(RES8_9, 120,330);	
    GUI_DispStringAt(RES8_10, 120,360);
    GUI_DispStringAt(RES8_11, 120,390);

	
    GUI_SetFont(&GUI_Font16B_ASCII);
    GUI_DispStringAt("m", 360,150);	
    GUI_DispStringAt("m", 360,180);	
    GUI_DispStringAt("m", 360,240);	
    GUI_DispStringAt("m", 360,270);		
    GUI_DispStringAt("deg", 360,330);	
    GUI_DispStringAt("deg", 360,360);	
    GUI_DispStringAt("deg", 360,390);		

    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){

		case GUI_ID_SWITCH:
			hItem=WM_GetDialogItem(hWin,GUI_ID_SWITCH);
			if(_control_switch)
			{
			  _control_switch=0;
  			  BUTTON_SetText(hItem, RES8_13);
			}
  			else
  			{
  			  _control_switch=1;
  			  BUTTON_SetText(hItem, RES8_12);
  			}
			break;
			
		case GUI_ID_OK:
			_Config3Done(hWin);
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

static void _CreateConfig3View(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  EDIT_Handle hEdit;
  int i;
  
  _title_Config3View=title;
  _control_switch=_config_info.control_switch;
  
  hFrameWin = FRAMEWIN_Create("Config3View", _cbConfig3View, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  hItem = BUTTON_CreateEx(80, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CANCEL);
  BUTTON_SetText(hItem, RES8_14);
  
  hItem = BUTTON_CreateEx(640, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES8_15);

  hItem = BUTTON_CreateEx(280, 90, 48, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_SWITCH);
  if(_control_switch)
  BUTTON_SetText(hItem, RES8_12);
  else
  BUTTON_SetText(hItem, RES8_13);
  
  /* Create edit widget */
  EDIT_SetDefaultFont(&GUI_Font8x16);
  EDIT_SetDefaultTextColor(0, GUI_BLACK);
  
  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 150, 80, 24, hFrameWin,GUI_ID_EDIT3_0, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_stop_out,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 180, 80, 24, hFrameWin,GUI_ID_EDIT3_0+1, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_stop_in,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 240, 80, 24, hFrameWin,GUI_ID_EDIT3_0+2, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.height_stop_up,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 270, 80, 24, hFrameWin,GUI_ID_EDIT3_0+3, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.height_stop_down,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 330, 80, 24, hFrameWin,GUI_ID_EDIT3_0+4, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.angle_stop_clockwise,0,999,0,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 360, 80, 24, hFrameWin,GUI_ID_EDIT3_0+5, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.angle_stop_anti_clockwise,0,999,0,0);

  hEdit = EDIT_CreateAsChild( EDIT3_OFFSET, 390, 80, 24, hFrameWin,GUI_ID_EDIT3_0+6, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.angle_stop_pre,0,999,0,0);

  for(i=0;i<7;i++)
  {
      hItem=WM_GetDialogItem(hFrameWin,GUI_ID_EDIT3_0+i);
      WM_SetCallback(hItem,_cbEdit);
  }

  
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


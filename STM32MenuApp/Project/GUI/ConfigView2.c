

static char *_title_Config2View;

#define EDIT2_OFFSET 240

static void _Config2Done(WM_HWIN hWin)
{
  WM_HWIN hItem;
  int i;

  for(i=0;i<8;i++)
  {
    hItem=WM_GetDialogItem(hWin,GUI_ID_EDIT2_0+i);
	switch(i)
		{
			case 0:
				_config_info.range_begin=EDIT_GetValue(hItem);
				break;

			case 1:
				_config_info.range_end=EDIT_GetValue(hItem);
				break;

			case 2:
				_config_info.height_begin=EDIT_GetValue(hItem);
				break;

			case 3:
				_config_info.height_end=EDIT_GetValue(hItem);
				break;				

			case 4:
				_config_info.weight_calibration_zero=EDIT_GetValue(hItem);
				break;

			case 5:
				_config_info.weight_calibration_standard=EDIT_GetValue(hItem);
				break;

			case 6:
				_config_info.angle_begin=EDIT_GetValue(hItem);
				break;

			case 7:
				_config_info.angle_end=EDIT_GetValue(hItem);
				break;	
				
		}
  }
  
}

/*******************************************************************
*
*       _cbConfig1View
*/
static void _cbConfig2View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config2View, 400,40);

    GUI_SetColor(GUI_BLACK);	
    GUI_DispStringAt(RES7_1, 40,60);
    GUI_DispStringAt(RES7_2, 120,90);
    GUI_DispStringAt(RES7_3, 120,120);
    GUI_DispStringAt(RES7_4, 40,150);	
    GUI_DispStringAt(RES7_5, 120,180);
    GUI_DispStringAt(RES7_6, 120,210);
    GUI_DispStringAt(RES7_7, 40,240);
    GUI_DispStringAt(RES7_8, 120,270);	
    GUI_DispStringAt(RES7_9, 120,300);	
    GUI_DispStringAt(RES7_10, 40,330);
    GUI_DispStringAt(RES7_11, 120,360);
    GUI_DispStringAt(RES7_12, 120,390);
	
    GUI_SetFont(&GUI_Font16B_ASCII);
    GUI_DispStringAt("m", 320,90);	
    GUI_DispStringAt("m", 320,120);	
    GUI_DispStringAt("m", 320,180);	
    GUI_DispStringAt("m", 320,210);		
    GUI_DispStringAt("kg", 320,270);	
    GUI_DispStringAt("kg", 320,300);	
    GUI_DispStringAt("deg", 320,360);	
    GUI_DispStringAt("deg", 320,390);		

    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_OK:
			_Config2Done(hWin);
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

static void _CreateConfig2View(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  EDIT_Handle hEdit;
  int i;
  
  _title_Config2View=title;
	
  hFrameWin = FRAMEWIN_Create("Config2View", _cbConfig2View, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  hItem = BUTTON_CreateEx(80, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CANCEL);
  BUTTON_SetText(hItem, RES6_14);
  
  hItem = BUTTON_CreateEx(640, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES6_16);

  
  /* Create edit widget */
  EDIT_SetDefaultFont(&GUI_Font8x16);
  EDIT_SetDefaultTextColor(0, GUI_BLACK);
  
  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 90, 80, 24, hFrameWin,GUI_ID_EDIT2_0, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_begin,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 120, 80, 24, hFrameWin,GUI_ID_EDIT2_0+1, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_end,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 180, 80, 24, hFrameWin,GUI_ID_EDIT2_0+2, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.height_begin,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 210, 80, 24, hFrameWin,GUI_ID_EDIT2_0+3, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.height_end,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 270, 80, 24, hFrameWin,GUI_ID_EDIT2_0+4, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.weight_calibration_zero,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 300, 80, 24, hFrameWin,GUI_ID_EDIT2_0+5, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.weight_calibration_standard,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 360, 80, 24, hFrameWin,GUI_ID_EDIT2_0+6, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.angle_begin,0,999,0,0);

  hEdit = EDIT_CreateAsChild( EDIT2_OFFSET, 390, 80, 24, hFrameWin,GUI_ID_EDIT2_0+7, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.angle_end,0,999,0,0);

  for(i=0;i<8;i++)
  {
      hItem=WM_GetDialogItem(hFrameWin,GUI_ID_EDIT2_0+i);
      WM_SetCallback(hItem,_cbEdit);
  }
  
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


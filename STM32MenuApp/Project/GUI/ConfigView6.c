

static char *_title_Config6View;

#define EDIT6_OFFSET 40

static void _Config6Done(WM_HWIN hWin)
{
  WM_HWIN hItem;
  int i;

  for(i=0;i<17;i++)
  {
    hItem=WM_GetDialogItem(hWin,GUI_ID_EDIT6_0+i);
	switch(i)
		{
			case 0:
				_config_info.password_menu=EDIT_GetValue(hItem);
				break;

			case 1:
				_config_info.password_menu=EDIT_GetValue(hItem);
				break;

			case 2:
				_config_info.password_setup=EDIT_GetValue(hItem);
				break;

			case 3:
				_config_info.password_setup=EDIT_GetValue(hItem);
				break;

			case 4:
				_config_info.user_defined_no=EDIT_GetValue(hItem);
				break;
				
			case 5:
				EDIT_GetText(hItem,_config_info.date,INFO_STRING_LEN);
				break;

			case 6:
				EDIT_GetText(hItem,_config_info.time,INFO_STRING_LEN);
				break;

			case 7:
				_config_info.pre_warning_ratio=EDIT_GetValue(hItem);
				break;

			case 8:
				_config_info.warning_ratio=EDIT_GetValue(hItem);				
				break;				

			case 9:
				_config_info.impacting_eliminate_time=EDIT_GetValue(hItem);
				break;

			case 10:
				_config_info.ratio=EDIT_GetValue(hItem);
				break;

			case 11:
				_config_info.cord_weight_per_meter=EDIT_GetValue(hItem);
				break;

			case 12:
				_config_info.cord_diameter=EDIT_GetValue(hItem);
				break;	

			case 13:
				_config_info.cord_layer=EDIT_GetValue(hItem);
				break;	

			case 14:
				_config_info.height_maintenance_interval=EDIT_GetValue(hItem);
				break;	

			case 15:
				_config_info.range_maintenance_interval=EDIT_GetValue(hItem);
				break;					

			case 16:
				_config_info.angle_maintenance_interval=EDIT_GetValue(hItem);
				break;	
		}
  }
  
}


/*******************************************************************
*
*       _cbConfig6View
*/
static void _cbConfig6View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config6View, 400,40);

    GUI_SetColor(GUI_BLACK);	
    GUI_DispStringAt(RES11_1, 20,90);
    GUI_DispStringAt(RES11_2, 40,120);
    GUI_DispStringAt(RES11_3, 40,150);
    GUI_DispStringAt(RES11_4, 20,180);	
    GUI_DispStringAt(RES11_2, 40,210);
    GUI_DispStringAt(RES11_3, 40,240);
    GUI_DispStringAt(RES11_5, 20,270);
    GUI_DispStringAt(RES11_6, 20,330);	

    GUI_DispStringAt(RES11_7, 240,90);
    GUI_DispStringAt(RES11_8, 240,150);
    GUI_DispStringAt(RES11_9, 240,210);
    GUI_DispStringAt(RES11_10, 240,270);	
    GUI_DispStringAt(RES11_11, 240,330);
	
    GUI_DispStringAt(RES11_12, 480,90);
    GUI_DispStringAt(RES11_13, 480,150);
    GUI_DispStringAt(RES11_14, 480,210);	
    GUI_DispStringAt(RES11_15, 480,270);
    GUI_DispStringAt(RES11_16, 480,330);
    GUI_DispStringAt(RES11_17, 480,390);	
	
    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_OK:
			/*get value from widget*/
			_Config6Done(hWin);
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

static void _CreateConfig6View(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  EDIT_Handle hEdit;
  int i;
  
  _title_Config6View=title;
	
  hFrameWin = FRAMEWIN_Create("Config6View", _cbConfig6View, WM_CF_SHOW, 0, 0, 800, 480);
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

  hEdit = EDIT_CreateAsChild( 120, 120, 80, 24, hFrameWin,GUI_ID_EDIT6_0, 0,INFO_NUM_LEN );
  EDIT_SetDecMode(hEdit,_config_info.password_menu,0,999999,0,0);

  hEdit = EDIT_CreateAsChild( 120, 150, 80, 24, hFrameWin,GUI_ID_EDIT6_0+1, 0,INFO_NUM_LEN );
  EDIT_SetDecMode(hEdit,_config_info.password_menu,0,999999,0,0);

  hEdit = EDIT_CreateAsChild( 120, 210, 80, 24, hFrameWin,GUI_ID_EDIT6_0+2, 0,INFO_NUM_LEN );
  EDIT_SetDecMode(hEdit,_config_info.password_setup,0,999999,0,0);

  hEdit = EDIT_CreateAsChild( 120, 240, 80, 24, hFrameWin,GUI_ID_EDIT6_0+3, 0,INFO_NUM_LEN );
  EDIT_SetDecMode(hEdit,_config_info.password_setup,0,999999,0,0);

  hEdit = EDIT_CreateAsChild( 120, 300, 80, 24, hFrameWin,GUI_ID_EDIT6_0+4, 0,INFO_NUM_LEN );
  EDIT_SetDecMode(hEdit,_config_info.user_defined_no,0,999999,0,0);

  hEdit = EDIT_CreateAsChild( 120, 360, 80, 24, hFrameWin,GUI_ID_EDIT6_0+5, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.date);

  
  hEdit = EDIT_CreateAsChild( 260, 120, 80, 24, hFrameWin,GUI_ID_EDIT6_0+6, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.time);
  
  hEdit = EDIT_CreateAsChild( 260, 180, 80, 24, hFrameWin,GUI_ID_EDIT6_0+7, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.pre_warning_ratio,0,999,0,0);

  hEdit = EDIT_CreateAsChild( 260, 240, 80, 24, hFrameWin,GUI_ID_EDIT6_0+8, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.warning_ratio,0,999,0,0);

  hEdit = EDIT_CreateAsChild( 260, 300, 80, 24, hFrameWin,GUI_ID_EDIT6_0+9, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.impacting_eliminate_time,0,9999,0,0);

  hEdit= EDIT_CreateAsChild( 260, 360, 80, 24, hFrameWin,GUI_ID_EDIT6_0+10, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.ratio,0,999,0,0);


  hEdit = EDIT_CreateAsChild( 500, 120, 80, 24, hFrameWin,GUI_ID_EDIT6_0+11, 0,6 );
  EDIT_SetDecMode(hEdit,_config_info.cord_weight_per_meter,0,99999,3,0);

  hEdit = EDIT_CreateAsChild( 500, 180, 80, 24, hFrameWin,GUI_ID_EDIT6_0+12, 0,6 );
  EDIT_SetDecMode(hEdit,_config_info.cord_diameter,0,99999,3,0);

  hEdit = EDIT_CreateAsChild( 500, 240, 80, 24, hFrameWin,GUI_ID_EDIT6_0+13, 0,3 );
  EDIT_SetDecMode(hEdit,_config_info.cord_layer,0,999,0,0);

  hEdit = EDIT_CreateAsChild( 500, 300, 80, 24, hFrameWin,GUI_ID_EDIT6_0+14, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.height_maintenance_interval,0,9999,0,0);

  hEdit= EDIT_CreateAsChild( 500, 360, 80, 24, hFrameWin,GUI_ID_EDIT6_0+15, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.range_maintenance_interval,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( 500, 420, 80, 24, hFrameWin,GUI_ID_EDIT6_0+16, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.angle_maintenance_interval,0,9999,0,0);

  for(i=0;i<17;i++)
  {
      hItem=WM_GetDialogItem(hFrameWin,GUI_ID_EDIT6_0+i);
      WM_SetCallback(hItem,_cbEdit);
  }
    
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


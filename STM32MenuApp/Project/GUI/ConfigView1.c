


#define CFG_VIEW1_Y_OFFSET 20

static char *_title_Config1View;

static void _Config1Done(WM_HWIN hWin)
{
  WM_HWIN hItem;
  int i;

  for(i=0;i<8;i++)
  {
    hItem=WM_GetDialogItem(hWin,GUI_ID_EDIT1_0+i);
	switch(i)
		{
			case 0:
				EDIT_GetText(hItem,_config_info.model,INFO_STRING_LEN);
				break;

			case 1:
				EDIT_GetText(hItem,_config_info.serial_no,INFO_STRING_LEN);
				break;

			case 2:
				EDIT_GetText(hItem,_config_info.vendor_name,INFO_STRING_LEN);
				break;

			case 3:
				EDIT_GetText(hItem,_config_info.installation_date,INFO_STRING_LEN);
				break;				

			case 4:
				_config_info.weight_max=EDIT_GetValue(hItem);
				break;

			case 5:
				_config_info.range_max=EDIT_GetValue(hItem);
				break;

			case 6:
				_config_info.error_ratio_convert=EDIT_GetValue(hItem);
				break;

			case 7:
				_config_info.range_max_weight_max=EDIT_GetValue(hItem);
				break;	
				
		}
  }
  
}

/*******************************************************************
*
*       _cbConfig1View
*/
static void _cbConfig1View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config1View, 400,40+CFG_VIEW1_Y_OFFSET);

    GUI_SetColor(GUI_BLACK);
	
    GUI_DispStringAt(RES6_1, 40,80+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_2, 40,120+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_3, 40,160+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_4, 40,200+CFG_VIEW1_Y_OFFSET);	
    GUI_DispStringAt(RES6_5, 40,240+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_6, 40,280+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_7, 40,320+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_8, 40,360+CFG_VIEW1_Y_OFFSET);	

    GUI_DispStringAt(RES6_9, 400,80+CFG_VIEW1_Y_OFFSET);	
    GUI_DispStringAt(RES6_10, 400,120+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_11, 560,120+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_12, 400,160+CFG_VIEW1_Y_OFFSET);
    GUI_DispStringAt(RES6_13, 560,160+CFG_VIEW1_Y_OFFSET);	
	
    GUI_SetFont(&GUI_Font16B_ASCII);
    GUI_DispStringAt("kg", 280,240+CFG_VIEW1_Y_OFFSET);	
    GUI_DispStringAt("m", 280,280+CFG_VIEW1_Y_OFFSET);	
    GUI_DispStringAt("kg", 320,320+CFG_VIEW1_Y_OFFSET);	
    GUI_DispStringAt("m", 380,360+CFG_VIEW1_Y_OFFSET);		
    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_SAVE_TO_TABLE:
		
			break;
		
		case GUI_ID_OK:
  			_Config1Done(hWin);
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

static void _CreateConfig1View(char*title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  EDIT_Handle hEdit;
  int i;
  
  _title_Config1View=title;
  	
  hFrameWin = FRAMEWIN_Create("Config1View", _cbConfig1View, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  hItem = BUTTON_CreateEx(80, 400+CFG_VIEW1_Y_OFFSET, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CANCEL);
  BUTTON_SetText(hItem, RES6_14);

  hItem = BUTTON_CreateEx(200, 400+CFG_VIEW1_Y_OFFSET, 120, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_SAVE_TO_TABLE);
  BUTTON_SetText(hItem, RES6_15);
  
  hItem = BUTTON_CreateEx(640, 400+CFG_VIEW1_Y_OFFSET, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES6_16);

  
  /* Create edit widget */
  EDIT_SetDefaultFont(&GUI_Font8x16);
  EDIT_SetDefaultTextColor(0, GUI_BLACK);
  
  hEdit = EDIT_CreateAsChild( 120, 80+CFG_VIEW1_Y_OFFSET, 160, 24, hFrameWin,GUI_ID_EDIT1_0, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.model);
  
  hEdit = EDIT_CreateAsChild( 120, 120+CFG_VIEW1_Y_OFFSET, 160, 24, hFrameWin,GUI_ID_EDIT1_0+1, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.serial_no);

  hEdit = EDIT_CreateAsChild( 120, 160+CFG_VIEW1_Y_OFFSET, 160, 24, hFrameWin,GUI_ID_EDIT1_0+2, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.vendor_name);

   hEdit = EDIT_CreateAsChild( 120, 200+CFG_VIEW1_Y_OFFSET, 160, 24, hFrameWin,GUI_ID_EDIT1_0+3, 0,INFO_STRING_LEN );
  EDIT_SetText(hEdit, _config_info.installation_date);

  hEdit = EDIT_CreateAsChild( 160, 240+CFG_VIEW1_Y_OFFSET, 120, 24, hFrameWin,GUI_ID_EDIT1_0+4, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.weight_max,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( 160, 280+CFG_VIEW1_Y_OFFSET, 120, 24, hFrameWin,GUI_ID_EDIT1_0+5, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_max,0,9999,INFO_LENGTH_SHIFT,0);

  hEdit = EDIT_CreateAsChild( 200, 320+CFG_VIEW1_Y_OFFSET, 120, 24, hFrameWin,GUI_ID_EDIT1_0+6, 0,4 );
  EDIT_SetDecMode(hEdit,_config_info.error_ratio_convert,0,9999,0,0);

   hEdit = EDIT_CreateAsChild( 260, 360+CFG_VIEW1_Y_OFFSET, 120, 24, hFrameWin,GUI_ID_EDIT1_0+7, 0,5 );
  EDIT_SetDecMode(hEdit,_config_info.range_max_weight_max,0,9999,INFO_LENGTH_SHIFT,0);

   hEdit = EDIT_CreateAsChild( 460, 120+CFG_VIEW1_Y_OFFSET, 80, 24, hFrameWin,GUI_ID_EDIT1_0+8, 0,4 );
  EDIT_SetDecMode(hEdit,9,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( 680, 120+CFG_VIEW1_Y_OFFSET, 80, 24, hFrameWin,GUI_ID_EDIT1_0+9, 0,4 );
  EDIT_SetDecMode(hEdit,10,0,9999,0,0);

  hEdit = EDIT_CreateAsChild( 460, 160+CFG_VIEW1_Y_OFFSET, 80, 24, hFrameWin,GUI_ID_EDIT1_0+10, 0,4 );
  EDIT_SetDecMode(hEdit,11,0,9999,0,0);

  hEdit= EDIT_CreateAsChild( 680, 160+CFG_VIEW1_Y_OFFSET, 80, 24, hFrameWin,GUI_ID_EDIT1_0+11, 0,4 );
  EDIT_SetDecMode(hEdit,12,0,9999,0,0);

  for(i=0;i<8;i++)
  {
      hItem=WM_GetDialogItem(hFrameWin,GUI_ID_EDIT1_0+i);
      WM_SetCallback(hItem,_cbEdit);
  }
  
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}


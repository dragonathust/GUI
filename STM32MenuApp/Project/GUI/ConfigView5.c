


static char *_title_Config5View;



/*******************************************************************
*
*       _cbConfig5View
*/
static void _cbConfig5View(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
    PrintLocalTime(560,20);
    GUI_DrawBitmap(&bmlogo, 10, 10);
	  
    GUI_SetColor(GUI_DARKBLUE);		
    GUI_SetFont(&GUI_FontHZ16S);	
	
    GUI_DispStringHCenterAt(_title_Config5View, 400,40);

    GUI_SetColor(GUI_BLACK);	
    GUI_DispStringAt(RES10_1, 10,70);
    GUI_DispStringAt(RES10_2, 10,220);
    GUI_DispStringAt(RES10_3, 10,370);


    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_PRINT:
			break;

		case GUI_ID_SAVE_TO_DISK:
			
			break;
		
		case GUI_ID_OK:
      			GUI_EndDialog(hWin, 0);			
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

static void _CreateConfig5View(char *title)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  LISTVIEW_Handle hListView;

  int i;
  HEADER_Handle hHeader;
  const GUI_ConstString* ppText;
  int NumRows;

  _title_Config5View=title;
	
  hFrameWin = FRAMEWIN_Create("Config5View", _cbConfig5View, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);
  
  /* Create dialog items */
  hItem = BUTTON_CreateEx(560, 70, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_PRINT);
  BUTTON_SetText(hItem, RES10_4);
  
  hItem = BUTTON_CreateEx(640, 70, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_SAVE_TO_DISK);
  BUTTON_SetText(hItem, RES10_5);

  hItem = BUTTON_CreateEx(560, 220, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_PRINT);
  BUTTON_SetText(hItem, RES10_4);
  
  hItem = BUTTON_CreateEx(640, 220, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_SAVE_TO_DISK);
  BUTTON_SetText(hItem, RES10_5);

  hItem = BUTTON_CreateEx(640, 420, 80, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetText(hItem, RES10_6);

  hListView = LISTVIEW_Create(10, 100, 780, 120, hFrameWin, GUI_ID_LISTVIEW1, WM_CF_SHOW, 0);
  hHeader = LISTVIEW_GetHeader(hListView);
  HEADER_SetHeight(hHeader, 20);
  HEADER_SetFont(hHeader,&GUI_FontHZ16S);
  LISTVIEW_SetGridVis(hListView, 1);
  LISTVIEW_SetBkColor(hListView, 0, GUI_LIGHTYELLOW);
  SCROLLBAR_CreateAttached(hListView, SCROLLBAR_CF_VERTICAL);

  LISTVIEW_AddColumn(hListView, 40, RES12_1, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES12_2, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES12_3, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_4, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_5, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_6, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_7, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_8, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_9, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES12_10, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_11, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_12, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES12_13, GUI_TA_CENTER);

/*  
    for (i = 0; i < GUI_COUNTOF(_aTable_1); i++) {
    LISTVIEW_AddRow(hListView, _aTable_1[i]);
  }
*/
  LoadOverLoadRecord((void**)&ppText,&NumRows);
    for (i = 0; i < NumRows; i++) {
    LISTVIEW_AddRow(hListView, ppText+i*13);
  }
  
  hListView = LISTVIEW_Create(10, 250, 780, 120, hFrameWin, GUI_ID_LISTVIEW2, WM_CF_SHOW, 0);
  hHeader = LISTVIEW_GetHeader(hListView);
  HEADER_SetHeight(hHeader, 20);
  HEADER_SetFont(hHeader,&GUI_FontHZ16S);
  LISTVIEW_SetGridVis(hListView, 1);
  LISTVIEW_SetBkColor(hListView, 0, GUI_LIGHTYELLOW);
  SCROLLBAR_CreateAttached(hListView, SCROLLBAR_CF_VERTICAL);

  LISTVIEW_AddColumn(hListView, 40, RES13_1, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES13_2, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES13_3, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_4, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_5, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_6, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_7, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_8, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES13_9, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_10, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_11, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 72, RES13_12, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 40, RES13_13, GUI_TA_CENTER);

  LoadOperationHistoryRecord((void**)&ppText,&NumRows);
    for (i = 0; i < NumRows; i++) {
    LISTVIEW_AddRow(hListView, ppText+i*13);
  }
	
  hListView = LISTVIEW_Create(10, 400, 360, 60, hFrameWin, GUI_ID_LISTVIEW3, WM_CF_SHOW, 0);
  hHeader = LISTVIEW_GetHeader(hListView);
  HEADER_SetHeight(hHeader, 20);
  HEADER_SetFont(hHeader,&GUI_FontHZ16S);
  LISTVIEW_SetGridVis(hListView, 1);
  LISTVIEW_SetBkColor(hListView, 0, GUI_LIGHTYELLOW);

  LISTVIEW_AddColumn(hListView, 80, RES14_1, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 80, RES14_2, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 80, RES14_3, GUI_TA_CENTER);
  LISTVIEW_AddColumn(hListView, 80, RES14_4, GUI_TA_CENTER);

  LoadRunTimeRecord((void**)&ppText,&NumRows);
    for (i = 0; i < NumRows; i++) {
    LISTVIEW_AddRow(hListView, ppText+i*4);
  }

	
  /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin);
}



static GUI_CONST_STORAGE unsigned char _acArrowRight2[] = {
  __X_____,________,
  __XXX___,________,
  __XXXXX_,________,
  __X__XXX,X_______,
  __X_____,XXX_____,
  __X_____,__XXX___,
  __X_____,____XX__,
  __X_____,__XXX___,
  __X_____,XXX_____,
  __X__XXX,X_______,
  __XXXXX_,________,
  __XXX___,________,
  __X_____,________
};

static GUI_CONST_STORAGE unsigned char _acArrowLeft2[] = {
  ________,_____X__,
  ________,___XXX__,
  ________,_XXXXX__,
  _______X,XXX__X__,
  _____XXX,_____X__,
  ___XXX__,_____X__,
  __XX____,_____X__,
  ___XXX__,_____X__,
  _____XXX,_____X__,
  _______X,XXX__X__,
  ________,_XXXXX__,
  ________,___XXX__,
  ________,_____X__,
};


static GUI_CONST_STORAGE GUI_BITMAP _bmArrowRight2 = {
  16,           /* XSize */
  13,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowRight2, /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowLeft2 = {
  16,           /* XSize */
  13,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowLeft2,  /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};


/*********************************************************************
*
*       _cbTextButton
*/
static void _cbTextButton(WM_MESSAGE* pMsg) {
  int Index;
  char ac[128];
  GUI_RECT Rect;
	
  switch (pMsg->MsgId) {
  case WM_PAINT:
      Index = (WIDGET_GetState(pMsg->hWin) & BUTTON_STATE_PRESSED) ? 1 : 0;

      WM_GetClientRect(&Rect);
      GUI_SetColor(BUTTON_GetBkColor(pMsg->hWin, Index));
      GUI_FillRect(Rect.x0,Rect.y0,Rect.x1,Rect.y1);

      GUI_SetColor(BUTTON_GetTextColor(pMsg->hWin, Index));
      GUI_SetBkColor(BUTTON_GetBkColor(pMsg->hWin, Index));
      GUI_SetFont(BUTTON_GetFont(pMsg->hWin));
      BUTTON_GetText(pMsg->hWin, ac, sizeof(ac));
      Rect.x0+=24;
      GUI_DispStringInRect(ac, &Rect, /*GUI_TA_HCENTER |*/ GUI_TA_VCENTER);
      GUI_DrawBitmap(&_bmArrowRight2, 0, 5);  
      break;

  default:
    BUTTON_Callback(pMsg);
  }
}
  
/*******************************************************************
*
*       _cbConfigView
*/
static void _cbConfigView(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  int Id;
  int value;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    {
      PrintLocalTime(560,20);
      GUI_DrawBitmap(&bmlogo, 10, 10);
	
    }
    break;

  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
  	Id = WM_GetId(pMsg->hWinSrc);
	switch(Id){
		case GUI_ID_CLOSE:
			SaveConfigInfo(&_config_info);
      			GUI_EndDialog(hWin, 1);
			//_CreateMainView();	
			break;
			
		case GUI_ID_ABOUT:
			_CreateAboutBox();
			break;
			
		case GUI_ID_CONFIG_VIEW1:
			value = _CreateNumPad(RES5_1);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_setup)
			_CreateConfig1View(RES5_1);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig1View(RES5_1);			
			#endif
			
			break;

		case GUI_ID_CONFIG_VIEW2:
			value = _CreateNumPad(RES5_2);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_menu)
			_CreateConfig2View(RES5_2);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig2View(RES5_2);			
			#endif
			break;
			
		case GUI_ID_CONFIG_VIEW3:
			value = _CreateNumPad(RES5_3);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_menu )
			_CreateConfig3View(RES5_3);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig3View(RES5_3);			
			#endif
			break;

		case GUI_ID_CONFIG_VIEW4:
			value = _CreateNumPad(RES5_4);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_menu )
			_CreateConfig4View(RES5_4);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig4View(RES5_4);			
			#endif			
			break;

		case GUI_ID_CONFIG_VIEW5:
			value = _CreateNumPad(RES5_5);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_menu )
			_CreateConfig5View(RES5_5);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig5View(RES5_5);			
			#endif				
			break;
			
		case GUI_ID_CONFIG_VIEW6:
			value = _CreateNumPad(RES5_6);
			#ifdef PASSWORD_CHECK
			if( value == _config_info.password_menu )
			_CreateConfig6View(RES5_6);
			else
			_CreateMessageBox(RES5_9,NULL);
			#else
			_CreateConfig6View(RES5_6);	
			#endif	
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

/*******************************************************************
*
*       _AddMenuItem
*/
/*
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu, const char* pText, U16 Id, U16 Flags) {
  MENU_ITEM_DATA Item;
  Item.pText    = (char*)pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}
*/

static _CreateConfigView(void)
{
  WM_HWIN hFrameWin;
  WM_HWIN hItem;
  //MENU_Handle hMenu, hMenuGame, hMenuOptions, hMenuHelp;
  
  hFrameWin = FRAMEWIN_Create("ConfigView", _cbConfigView, WM_CF_SHOW, 0, 0, 800, 480);
  FRAMEWIN_SetTitleVis(hFrameWin,0);
  FRAMEWIN_SetClientColor(hFrameWin,BACKGROUND_COLOR);
  FRAMEWIN_SetActive(hFrameWin, 1);

  /* Create dialog items */
  hItem = BUTTON_CreateEx(700,420, 60, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CLOSE);
  BUTTON_SetText(hItem, RES4_6);

  hItem = BUTTON_CreateEx(700,360, 60, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_ABOUT);
  BUTTON_SetText(hItem, RES4_3);
  
/*
  hItem = BUTTON_CreateEx(600-100,480-100, 80, 20, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_ARROW);
  BUTTON_SetText(hItem, "Arrow");
  BUTTON_SetBitmapEx(hItem, 0, &_bmArrowLeft, 7, 7);
  BUTTON_SetBitmapEx(hItem, 1, &_bmArrowRight, 7, 7);  

  hItem = BUTTON_CreateEx(400-100,480-100, 80, 80, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_LOGO);
  BUTTON_SetText(hItem, "Logo");
  BUTTON_SetBitmapEx(hItem, 0, &bmSeggerLogoBlue, 7, 7);
  BUTTON_SetBitmapEx(hItem, 1, &bmSeggerLogoBlue, 7, 7);  
*/

  hItem = BUTTON_CreateEx(60,120, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW1);
  BUTTON_SetText(hItem, RES5_1);
  WM_SetCallback(hItem,_cbTextButton);

  hItem = BUTTON_CreateEx(60,160, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW2);
  BUTTON_SetText(hItem, RES5_2);
  WM_SetCallback(hItem,_cbTextButton);

  hItem = BUTTON_CreateEx(60,200, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW3);
  BUTTON_SetText(hItem, RES5_3);
  WM_SetCallback(hItem,_cbTextButton);

  hItem = BUTTON_CreateEx(60,240, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW4);
  BUTTON_SetText(hItem, RES5_4);
  WM_SetCallback(hItem,_cbTextButton);

  hItem = BUTTON_CreateEx(60,280, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW5);
  BUTTON_SetText(hItem, RES5_5);
  WM_SetCallback(hItem,_cbTextButton);

  hItem = BUTTON_CreateEx(60,320, 240, 24, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, GUI_ID_CONFIG_VIEW6);
  BUTTON_SetText(hItem, RES5_6);
  WM_SetCallback(hItem,_cbTextButton);
  
	
  //MENU_SetDefaultFont(&GUI_Font10_1);

    /* Create main menu */
  //hMenu = MENU_CreateEx(100, 100, 400, 100, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
  //_AddMenuItem(hMenu, 0,    "Game",    0, 0);
  //_AddMenuItem(hMenu, 0, "Options", 0, 0);
  //_AddMenuItem(hMenu, 0,    "Help",    0, 0);
  /* Attach menu to framewin */
  //FRAMEWIN_AddMenu(hFrameWin, hMenu);

#if 1
   /* Exec modal dialog */
  WM_SetFocus(hFrameWin);
  WM_MakeModal(hFrameWin);
  GUI_ExecCreatedDialog(hFrameWin); 
#endif
}


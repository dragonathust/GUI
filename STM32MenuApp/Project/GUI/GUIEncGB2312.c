/*
*********************************************************************************************************
GB2312 support
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       DB2GB2312
*/
static U16 DB2GB2312(U8 Byte0, U8 Byte1) {
  return Byte1 | (((U16)Byte0)<<8);
}

/*********************************************************************
*
*       _GetLineDistX_GB2312
*/
static int _GetLineDistX_GB2312(const char GUI_UNI_PTR *s, int Len) {
  int Dist =0;
  if (s) {
    U8 c0;
    while (((c0=*(const U8*)s) !=0) && Len >=0) {
      s++; Len--;
      if (c0 >= 0xa1) {
        U8  c1 = *(const U8*)s++;
        Len--;
        Dist += GUI_GetCharDistX(DB2GB2312(c0, c1));
      } else {
        Dist += GUI_GetCharDistX(c0);
      }
    }
  }
  return Dist;
}

/*********************************************************************
*
*       _GetLineLen_GB2312
* Purpose:
*   Returns the number of characters in a string.
*
* NOTE:
*   The return value can be used as offset into the
*   string, which means that double characters count double
*/
static int _GetLineLen_GB2312(const char GUI_UNI_PTR *s, int MaxLen) {
  int Len =0;
  U8 c0;
  while (((c0=*(const U8*)s) !=0) && Len < MaxLen) {
    s++;
    if (c0 >= 0xa1) {
      Len++; s++;
    } else {
      switch (c0) {
      case '\n': return Len;
      }
    }
    Len++;
  }
  return Len;
}

/*********************************************************************
*
*       _DispLine_GB2312
*/
static void _DispLine_GB2312(const char GUI_UNI_PTR *s, int Len) {
  U8 c0;
  while (--Len >=0) {
    c0=*(const U8*)s++;
    if (c0 >= 0xa1) {
      U8  c1 = *(const U8*)s++;
      Len--;
      GL_DispChar (DB2GB2312(c0, c1));
    } else {
      GL_DispChar(c0);
    }
  }
}

/*********************************************************************
*
*       GUI_ENC_APIList_GB2312, API list
*/
const tGUI_ENC_APIList GUI_ENC_APIList_GB2312 = {
  _GetLineDistX_GB2312,
  _GetLineLen_GB2312,
  _DispLine_GB2312
};

/*************************** End of file ****************************/

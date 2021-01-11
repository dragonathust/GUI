#ifndef __GUI_GIF_H__
#define __GUI_GIF_H__


typedef struct {
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  int Delay;
} GUI_GIF_IMAGE_INFO;

typedef struct {
  int xSize;
  int ySize;
  int NumImages;
} GUI_GIF_INFO;


int GUI_GIF_DrawEx(const void * pGIF, U32 NumBytes, int x0, int y0, int Index);
int GUI_GIF_Draw(const void * pGIF, U32 NumBytes, int x0, int y0);
int GUI_GIF_GetImageInfo(const void * pGIF, U32 NumBytes, GUI_GIF_IMAGE_INFO * pInfo, int Index);
int GUI_GIF_GetInfo(const void * pGIF, U32 NumBytes, GUI_GIF_INFO * pInfo);


#endif /*__GUI_GIF_H__*/


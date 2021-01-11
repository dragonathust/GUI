
#include "GUI.h"
#include "GUI_GIF.h"

#include "GifData.h"
#include "GifData2.h"
#include "GifData3.h"
#include "GifData4.h"

//#include "bk.h"

void ShowGif(const void * Pgif, U32 NumBytes , int x0, int y0)
{
    U16 i = 0;
    GUI_GIF_INFO InfoGif1;
    GUI_GIF_IMAGE_INFO InfoGif2;

    while (1)
    {
    GUI_GIF_GetInfo(Pgif, NumBytes, &InfoGif1);
    if(i < InfoGif1.NumImages)
    {
        GUI_GIF_GetImageInfo(Pgif, NumBytes, &InfoGif2, i );
        //if(!GUI_GIF_DrawEx(Pgif, NumBytes, x0 + InfoGif2.xPos, y0 + InfoGif2.yPos, i++))
        if(!GUI_GIF_DrawEx(Pgif, NumBytes, x0, y0, i++))
        {
            GUI_Delay(InfoGif2.Delay * 20); 
        }
    }
    else
	break;	
	}
}

//void MainTask(void)  
void MainTask_Gif(void)  
{
    GUI_Init();

    while (1)
    {
/*
		 GUI_Clear();
	     ShowGif(GifData, sizeof(GifData), 100, 100);

		 GUI_Clear();
	     ShowGif(GifData2, sizeof(GifData2), 100, 100);
	
		 GUI_Clear();
		 ShowGif(GifData3, sizeof(GifData3), 100, 100);

		 GUI_Clear();
		 ShowGif(GifData4, sizeof(GifData4), 0, 0);
		 GUI_Delay(300); 
*/
		//GUI_JPEG_Draw(JpegData,sizeof(JpegData),0,0);
		GUI_Delay(300); 
	}
	
}

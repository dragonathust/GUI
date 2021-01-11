/*
**********************************************************************
*                          Micrium, Inc.
*                      949 Crestview Circle
*                     Weston,  FL 33327-1848
*
*                            uC/FS
*
*             (c) Copyright 2001 - 2003, Micrium, Inc.
*                      All rights reserved.
*
***********************************************************************

----------------------------------------------------------------------
File        : fs_fat.h
Purpose     : FAT File System Layer header
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_FAT_H_
#define _FS_FAT_H_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "fs_int.h"

/*********************************************************************
*
*       fat_in
*/

FS_SIZE_T FS_FAT_FRead(FS_FILE *pFile, void *pData, FS_U32 NumBytes);


/*********************************************************************
*
*       fat_out
*/
FS_SIZE_T FS_FAT_FWrite (FS_FILE *pFile, const void *pData, FS_U32 NumBytes);
void      FS_FAT_FClose (FS_FILE *pFile);
void      FS_FAT_Unmount(FS_VOLUME  * pVolume);


/*********************************************************************
*
*       fat_open
*/
char      FS_FAT_FOpen            (const char *pFileName, FS_FILE *pFile, char DoDel, char DoOpen, char DoCreate);

/*********************************************************************
*
*       FAT_Misc
*/
int       FS_FAT_CheckBPB         (FS_VOLUME * pVolume, FS_U8 * pBuffer);
FS_U8     FS_FAT_ReadBPB          (FS_VOLUME * pVolume);

/*********************************************************************
*
*       fat_ioctl
*/
int       FS_FAT_Ioctl(FS_VOLUME * pVolume, FS_I32 Cmd, FS_I32 Aux, void *pBuffer);



/*********************************************************************
*
*       FAT_Move
*/
int       FS_FAT_Move(const char * sOldName, const char * sNewName, FS_VOLUME * pVolume);


/*********************************************************************
*
*       FS_FAT_DirEntry
*
*/
char FS_FAT_SetDirEntryInfo(FS_VOLUME * pVolume, const char * sName, const void * p, int Mask);
char FS_FAT_GetDirEntryInfo(FS_VOLUME * pVolume, const char * sName,       void * p, int Mask);

/*********************************************************************
*
*       FAT_Rename
*/
int FS_FAT_Rename(const char * sOldName, const char * sNewName, FS_VOLUME * pVolume);

/*********************************************************************
*
*       fat_dir
*/

#if FS_POSIX_DIR_SUPPORT
  int         FS_FAT_OpenDir (const char *pDirName, FS_DIR *pDir);
  int         FS_FAT_CloseDir(FS_DIR *pDir);
  FS_DIRENT * FS_FAT_ReadDir (FS_DIR *pDir);
  int         FS_FAT_RemoveDir(FS_VOLUME * pVolume, const char *pDirName);
  int         FS_FAT_CreateDir(FS_VOLUME * pVolume, const char *pDirName);
#endif /* FS_POSIX_DIR_SUPPORT */


/*********************************************************************
*
*       FAT_SetEndOfFile
*
*/
int FS_FAT_SetEndOfFile(FS_FILE * pFile);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif  /* _FS_FAT_H_ */

/*************************** End of file ****************************/
	 	 			 		    	 				 	   		   	 		  	 	      	   		 	 	  	  		  	 		 	    	 		     			       	   	 			  	 	   	  		 			 	    		   	 			  	  			 				 		  			 		 	  	 		    				 

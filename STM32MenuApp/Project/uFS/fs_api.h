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
File        : fs_api.h
Purpose     : Define global functions and types to be used by an
              application using the file system.

              This file needs to be included by any modul using the
              file system.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_API_H_
#define _FS_API_H_


/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "FS_ConfDefaults.h"        /* FS Configuration */


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

/* File system version */
#define FS_VERSION                0x00024800UL

/* Global error codes */
#define FS_ERR_OK                 0
#define FS_ERR_EOF                -1
#define FS_ERR_DISKFULL           -32
#define FS_ERR_INVALIDPAR         -48
#define FS_ERR_CMDNOTSUPPORTED    -40
#define FS_ERR_WRITEONLY          -64
#define FS_ERR_READONLY           -80
#define FS_ERR_READERROR          -96
#define FS_ERR_WRITEERROR         -112
#define FS_ERR_DISKCHANGED        -128
#define FS_ERR_CLOSE              -144
#define FS_ERR_INVALIDDESC        -160 /* Invalid media descriptor, media corrupt */
#define FS_ERR_DEVICETIMEOUT      -176 /* Device did not respond or timed out */
#define FS_ERR_NOMEDIAPRESENT     -192 /* No media in device present */

/* Global constants*/
/* File Positions */
#define FS_SEEK_CUR               1
#define FS_SEEK_END               2
#define FS_SEEK_SET               0

#define FS_FILE_CURRENT           FS_SEEK_CUR
#define FS_FILE_END               FS_SEEK_END
#define FS_FILE_BEGIN             FS_SEEK_SET

/* I/O commands */
#define FS_CMD_FLUSH_CACHE        1000L
#define FS_CMD_CHK_DSKCHANGE      1010L
#define FS_CMD_READ_SECTOR        1100L
#define FS_CMD_WRITE_SECTOR       1110L
#define FS_CMD_FORMAT_MEDIA       2222L
#define FS_CMD_FORMAT_AUTO        2333L
#define FS_CMD_FORMAT_EX          2334L
#define FS_CMD_FORMAT_LOW_LEVEL   9001L
#define FS_CMD_REQUIRES_FORMAT    9002L
#define FS_CMD_INC_BUSYCNT        3001L
#define FS_CMD_DEC_BUSYCNT        3002L
#define FS_CMD_GET_DISKFREE       4000L
#define FS_CMD_GET_TOTAL_SPACE    4001L
#define FS_CMD_GET_DEVINFO        4011L
#define FS_CMD_FREE_SECTORS       4012L


#define FS_CMD_FLASH_ERASE_CHIP   FS_CMD_FORMAT_LOW_LEVEL  /* Obsolete, do no use any more (name is confusing) */
#define FS_CMD_SET_DELAY          4013L
#define FS_CMD_UNMOUNT_VOLUME     4014L

#define FS_CMD_CACHE_SET_MODE     6000L
#define FS_CMD_CACHE_CLEAN        6001L   /* Write out all dirty sectors */

/* known media types */
#define FS_MEDIA_SMC_1MB          1010L
#define FS_MEDIA_SMC_2MB          1020L
#define FS_MEDIA_SMC_4MB          1030L
#define FS_MEDIA_SMC_8MB          1040L
#define FS_MEDIA_SMC_16MB         1050L
#define FS_MEDIA_SMC_32MB         1060L
#define FS_MEDIA_SMC_64MB         1070L
#define FS_MEDIA_SMC_128MB        1080L
#define FS_MEDIA_SMC_256MB        1090L

#define FS_MEDIA_FD_144MB         3040L

/* attribute definitions */
#define FS_ATTR_READ_ONLY    0x01
#define FS_ATTR_HIDDEN       0x02
#define FS_ATTR_SYSTEM       0x04
#define FS_ATTR_ARCHIVE      0x20
#define FS_ATTR_DIRECTORY    0x10

/*********************************************************************
*
*       Sector type defines
*/
#define FS_SECTOR_TYPE_DATA    0
#define FS_SECTOR_TYPE_DIR     1
#define FS_SECTOR_TYPE_MAN     2
#define FS_SECTOR_TYPE_COUNT   3

/*********************************************************************
*
*       Sector type defines
*/
#define FS_SECTOR_TYPE_MASK_DATA    (1 << FS_SECTOR_TYPE_DATA)
#define FS_SECTOR_TYPE_MASK_DIR     (1 << FS_SECTOR_TYPE_DIR)
#define FS_SECTOR_TYPE_MASK_MAN     (1 << FS_SECTOR_TYPE_MAN)
#define FS_SECTOR_TYPE_MASK_ALL     (FS_SECTOR_TYPE_MASK_DATA | FS_SECTOR_TYPE_MASK_DIR | FS_SECTOR_TYPE_MASK_MAN)

/*********************************************************************
*
*       Internal data types, used as opaque types for pointers
*
**********************************************************************
*/
typedef struct FS_CACHE_API   FS_CACHE_API;
typedef struct FS_DEVICE      FS_DEVICE;
typedef struct FS_DIR         FS_DIR;
typedef struct FS_FILE        FS_FILE;
typedef struct FS_PARTITION   FS_PARTITION;
typedef struct FS_VOLUME      FS_VOLUME;
typedef struct FS_DEVICE_TYPE FS_DEVICE_TYPE;

/*********************************************************************
*
*       Global data types
*
**********************************************************************
*/

typedef struct {
  FS_U32 total_clusters;
  FS_U32 avail_clusters;
  FS_U16 sectors_per_cluster;
  FS_U16 bytes_per_sector;
} FS_DISKFREE_T;

typedef struct {
  FS_U32 NumTotalClusters;
  FS_U32 NumFreeClusters;
  FS_U16 SectorsPerCluster;
  FS_U16 BytesPerSector;
} FS_DISK_INFO;

typedef struct {
  FS_U16 NumHeads;          /* Relevant only for mechanical drives   */
  FS_U16 SectorsPerTrack;   /* Relevant only for mechanical drives   */
  FS_U32 NumSectors;        /* Total number of sectors on the medium */
  FS_U16 BytesPerSector;    /* Number of bytes per sector            */
} FS_DEV_INFO;

typedef struct {
  FS_U16        SectorsPerCluster;
  FS_U16        NumRootDirEntries;    /* Proposed, actual value depends on FATType */
  FS_DEV_INFO * pDevInfo;
} FS_FORMAT_INFO;


typedef struct {
  FS_U16 Year;
  FS_U16 Month;
  FS_U16 Day;
  FS_U16 Hour;
  FS_U16 Minute;
  FS_U16 Second;
} FS_FILETIME;

/*********************************************************************
*
*             Non blocking (backgrounded) file I/O functions
*/
/* Information for background data */
typedef struct FS_BG_DATA {
  struct FS_BG_DATA * pNext;
  FS_U32              NumBytes;
  FS_U32              NumBytesRem;         /* Remaining bytes to transfer */
  void              * pData;
  FS_FILE           * pFile;
  FS_U8               Operation;
  void (*pfOnCompletion) (void * pData);   /* Optional completion routine */
  void              * pCompletionData;     /* Optional data for completion routine */
  char                IsCompleted;
} FS_BG_DATA;

void FS_Daemon(void);
void FS_FReadNonBlock (void       * pData,
                       FS_U32       NumBytes,
                       FS_FILE    * pFile,
                       FS_BG_DATA * pBGData,                    /* User supplied management block */
                       void (*pfOnCompletion) (void * pData),   /* Optional completion routine */
                       void * pCompletionData                   /* Optional data for completion routine */
                      );
void FS_FWriteNonBlock(const void * pData,
                       FS_U32       NumBytes,
                       FS_FILE    * pFile,
                       FS_BG_DATA * pBGData,                    /* User supplied management block */
                       void (*pfOnCompletion) (void * pData),   /* Optional completion routine */
                       void * pCompletionData                   /* Optional data for completion routine */
                       );
char FS_IsCompleted(FS_BG_DATA * pBGData);

/*********************************************************************
*
*       Directory types
*/
typedef struct FS_DIRENT {
  char     DirName[FS_DIRNAME_MAX];
  FS_U8    Attributes;
  FS_U32   Size;
  FS_U32   TimeStamp;
} FS_DIRENT;


/*********************************************************************
*
*       Global function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       "Standard" file I/O functions
*/
FS_FILE *        FS_FOpen (const char * pFileName, const char * pMode);
int              FS_FClose(FS_FILE    * pFile);
FS_SIZE_T        FS_FRead (      void * pData, FS_SIZE_T Size, FS_SIZE_T N, FS_FILE * pFile);
FS_SIZE_T        FS_FWrite(const void * pData, FS_SIZE_T Size, FS_SIZE_T N, FS_FILE * pFile);

/*********************************************************************
*
*       Non-standard file I/O functions
*/
FS_U32           FS_Read (FS_FILE * pFile,       void * pData, FS_U32 NumBytes);
FS_U32           FS_Write(FS_FILE * pFile, const void * pData, FS_U32 NumBytes);

/*********************************************************************
*
*       file pointer handling
*/
int              FS_FSeek       (FS_FILE * pFile, FS_I32 Offset,         int Origin);
int              FS_SetEndOfFile(FS_FILE * pFile);
int              FS_SetFilePos  (FS_FILE * pFile, FS_I32 DistanceToMove, int MoveMethod);
FS_I32           FS_GetFilePos  (FS_FILE * pFile);
FS_I32           FS_FTell       (FS_FILE * pFile);

/*********************************************************************
*
*       I/O error handling
*/
int              FS_FEof        (FS_FILE * pFile);
FS_I16           FS_FError      (FS_FILE * pFile);
void             FS_ClearErr    (FS_FILE * pFile);
const char *     FS_ErrorNo2Text(int       ErrCode);

/*********************************************************************
*
*       file functions
*/

int              FS_CopyFile   (const char * sSource,       const char * sDest);
FS_U32           FS_GetFileSize(FS_FILE    * pFile);
int              FS_Move       (const char * sExistingName, const char * sNewName);
int              FS_Remove     (const char * pFileName);
int              FS_Rename     (const char * sOldName,      const char * sNewName);
int              FS_Truncate   (FS_FILE    * pFile,         FS_U32       NewSize);
int              FS_Verify     (FS_FILE    * pFile,         const void * pData, FS_U32 NumBytes);

/*********************************************************************
*
*       IOCTL
*/
int              FS_IoCtl(const char *pDevName, FS_I32 Cmd, FS_I32 Aux, void *pBuffer);

/*********************************************************************
*
*       FS_Volume
*/
int              FS_GetVolumeName     (int Index, char * pBuffer, int MaxSize);
FS_U32           FS_GetVolumeSize     (const char * sVolume);
FS_U32           FS_GetVolumeFreeSpace(const char * sVolume);
int              FS_GetNumVolumes     (void);
FS_VOLUME *      FS_AddDevice         (const FS_DEVICE_TYPE * pDevType);
void             FS_Unmount           (const char * sVolume);
int              FS_GetVolumeInfo     (const char * sVolume, FS_DISK_INFO * pInfo);
int              FS_IsVolumeMounted   (const char * sVolumeName);
FS_U8            FS_CheckMediumPresent(const char * sVolume);

/*********************************************************************
*
*       FS_Attrib
*/
int              FS_SetFileAttributes(const char * pName, FS_U8 Attributes);
FS_U8            FS_GetFileAttributes(const char * pName);

/*********************************************************************
*
*       FS_Time
*/
int              FS_SetFileTime        (const char * pName, FS_U32    TimeStamp);
int              FS_GetFileTime        (const char * pName, FS_U32 * pTimeStamp);
void             FS_TimeStampToFileTime(FS_U32 TimeStamp, FS_FILETIME * pFileTime);
void             FS_FileTimeToTimeStamp(const FS_FILETIME * pFileTime, FS_U32 * pTimeStamp);


/*********************************************************************
*
*       FS_GetNumFilesOpen
*/
int              FS_GetNumFilesOpen(void);


/*********************************************************************
*
*       directory functions
*/
#if FS_POSIX_DIR_SUPPORT
void             FS_DirEnt2Attr(FS_DIRENT *pDirEnt, FS_U8* pAttr);
void             FS_DirEnt2Name(FS_DIRENT *pDirEnt, char* pBuffer);
FS_U32           FS_DirEnt2Size(FS_DIRENT *pDirEnt);
FS_U32           FS_DirEnt2Time(FS_DIRENT *pDirEnt);
FS_U32           FS_GetNumFiles(FS_DIR *pDir);
FS_DIR    *      FS_OpenDir(const char *pDirName);
int              FS_CloseDir(FS_DIR *pDir);
FS_DIRENT *      FS_ReadDir(FS_DIR *pDir);
void             FS_RewindDir(FS_DIR *pDir);
int              FS_MkDir(const char *pDirName);
int              FS_RmDir(const char *pDirName);
#endif  /* FS_POSIX_DIR_SUPPORT */


/*********************************************************************
*
*       file system control functions
*/
int              FS_Init          (void);
int              FS_Exit          (void);

/*********************************************************************
*
*       Formatting
*/
int              FS_Format   (const char *pDevice, FS_FORMAT_INFO * pFormatInfo);
int              FS_FormatLow(const char *pDevice);


/*********************************************************************
*
*       Space information functions
*/
#if FS_SUPPORT_DISKINFO 
FS_U32           FS_GetFreeSpace (const char *pDevName, FS_U8 DevIndex);
FS_U32           FS_GetTotalSpace(const char *pDevName, FS_U8 DevIndex);
#endif

/*********************************************************************
*
*             Debug support
*/
void             FS_X_Log     (const char *s);
void             FS_X_Warn    (const char *s);
void             FS_X_ErrorOut(const char *s);

/*********************************************************************
*
*             FLASH driver specific
*/

/*********************************************************************
*
*             FLASH driver types
*/
typedef struct {
  FS_U32 Off;
  FS_U32 Size;
  FS_U32 EraseCnt;
  FS_U16 NumUsedSectors;
  FS_U16 NumFreeSectors;
  FS_U16 NumEraseableSectors;
} FS_FLASH_SECTOR_INFO;

typedef struct {
  FS_U32 NumPhysSectors;
  FS_U32 NumLogSectors;
  FS_U32 NumUsedSectors;   /* Number of used logical sectors */
} FS_FLASH_DISK_INFO;

/*********************************************************************
*
*             FLASH driver prototypes
*/
void             FS_FLASH_GetDiskInfo  (FS_U8 Unit, FS_FLASH_DISK_INFO * pDiskInfo);
void             FS_FLASH_GetSectorInfo(FS_U8 Unit, FS_U32 PhysSectorIndex, FS_FLASH_SECTOR_INFO * pSectorInfo);



/*********************************************************************
*
*       Cache handling
*/
#if FS_SUPPORT_CACHE


/*********************************************************************
*
*       Cache mode defines
*/
#define FS_CACHE_MODE_R         0x01
#define FS_CACHE_MODE_W         0x02
#define FS_CACHE_MODE_D         0x04
#define FS_CACHE_MODE_FULL      (FS_CACHE_MODE_R | FS_CACHE_MODE_W | FS_CACHE_MODE_D)


/*********************************************************************
*
*       Cache specific defines
*/
#define FS_CACHE_NONE    NULL
#define FS_CACHE_MAN     FS_CacheMan_Init
#define FS_CACHE_RW      FS_CacheRW_Init 
#define FS_CACHE_ALL     FS_CacheAll_Init
#define FS_CACHEALL_API  FS_CacheAll_Init    /* For compatibility with older version */
#define FS_CACHEMAN_API  FS_CacheMan_Init    /* For compatibility with older version */

/*********************************************************************
*
*       Cache specific types
*/
typedef FS_U32 FS_INIT_CACHE (FS_DEVICE * pDevice, void * pData,   FS_I32 NumBytes);

/*********************************************************************
*
*       Cache specific prototypes
*/
FS_U32 FS_AssignCache  (const char * pName, void * pData, FS_I32 NumBytes, FS_INIT_CACHE * pfInit);
void   FS_CACHE_Clean  (const char * pName);
int    FS_CACHE_Command(const char * pName, int Cmd, void * pData);
int    FS_CACHE_SetMode(const char * pName, int TypeMask, int ModeMask);

/*********************************************************************
*
*       Cache initialization prototypes
*/
FS_U32 FS_CacheAll_Init(FS_DEVICE * pDev, void * pData, FS_I32 NumBytes);
FS_U32 FS_CacheMan_Init(FS_DEVICE * pDev, void * pData, FS_I32 NumBytes);
FS_U32 FS_CacheRW_Init (FS_DEVICE * pDev, void * pData, FS_I32 NumBytes);

#endif

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif  /* _FS_API_H_ */

/*************************** End of file ****************************/
	 	 			 		    	 				 	   		   	 		  	 	      	   		 	 	  	  		  	 		 	    	 		     			       	   	 			  	 	   	  		 			 	    		   	 			  	  			 				 		  			 		 	  	 		    				 

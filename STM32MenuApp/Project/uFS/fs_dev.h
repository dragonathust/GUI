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
File        : fs_dev.h
Purpose     : Define structures for Device Drivers
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_DEV_H_
#define _FS_DEV_H_

#include "FS_ConfDefaults.h"
#include "fs_int.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

struct FS_DEVICE_TYPE {
  const char *      name;
  int               MaxUnits;
  int               (*pfGetStatus)    (FS_U8 Unit);
  int               (*pfRead)         (FS_U8 Unit, FS_U32 SectorNo, void *pBuffer);
  int               (*pfWrite)        (FS_U8 Unit, FS_U32 SectorNo, const void *pBuffer);
  int               (*pfIoCtl)        (FS_U8 Unit, FS_I32 Cmd, FS_I32 Aux, void *pBuffer);
  int               (*pfReadBurst)    (FS_U8 Unit, FS_U32 SectorNo, FS_U32 NumSectors,       void *pBuffer);
  int               (*pfWriteBurst)   (FS_U8 Unit, FS_U32 SectorNo, FS_U32 NumSectors, const void *pBuffer);
  int               (*pfInitDevice)   (FS_U8 Unit);
  void              (*pfInitMedium)   (FS_U8 Unit);
};




/*********************************************************************
*
*             Device Driver Function Tables
*
*  If you add an own device driver,
*  for its function table here.
*/


extern const FS_DEVICE_TYPE    FS__ramdevice_driver;     /* RAMDISK_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__windrive_driver;      /* WINDRIVE_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__smcdevice_driver;     /* SMC_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__mmcdevice_driver;     /* MMC_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__idedevice_driver;     /* IDE_CF_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__flashdevice_driver;   /* FLASH_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__SFlash_Driver;        /* Serial FLASH_DRIVER function table */
extern const FS_DEVICE_TYPE    FS__genericdevice_driver; /* GENERIC_DRIVER function table */


#if defined(__cplusplus)
 }               /* Make sure we have C-declarations in C++ programs */
#endif

#endif  /* _FS_DEV_H_ */

/*************************** End of file ****************************/
	 	 			 		    	 				 	   		   	 		  	 	      	   		 	 	  	  		  	 		 	    	 		     			       	   	 			  	 	   	  		 			 	    		   	 			  	  			 				 		  			 		 	  	 		    				 

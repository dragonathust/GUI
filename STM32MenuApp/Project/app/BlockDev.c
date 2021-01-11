
#include "stm32f10x.h"
#include "sdcard.h"
#include "stdio.h"
#include "stm32f10x_usart.h"

#include <stdlib.h>
#include <string.h>


#include "fs_api.h"
#include "fs_int.h"


static unsigned int DevSectorCount=0;
static unsigned int DevSectorSize=0;

//===================================================================

static int ReadSector(void *Buffer, FS_U32 StartSector)
{
    SD_Error Status = SD_OK;
    
    Status = SD_ReadBlock(StartSector << 9 , Buffer, DevSectorSize);
      
    if (Status == SD_OK)
      return 0;
    else
      return -1;
   
}

static int ReadMultiSectors(void *Buffer, FS_U16 SectorCount, FS_U32 StartSector)
{
    SD_Error Status = SD_OK;
    
    Status = SD_ReadMultiBlocks(StartSector << 9 , Buffer, DevSectorSize,SectorCount );
      
    if (Status == SD_OK)
      return 0;
    else
      return -1;
   
}

static int WriteSector(const void *Buffer, FS_U32 StartSector)
{
    SD_Error Status = SD_OK;

    Status = SD_WriteBlock(StartSector << 9,(u32 *) Buffer, DevSectorSize);

    if (Status == SD_OK)
      return 0;
    else
      return -1;
}

static int WriteMultiSectors(const void *Buffer, FS_U16 SectorCount, FS_U32 StartSector)
{
    SD_Error Status = SD_OK;

    Status = SD_WriteMultiBlocks(StartSector << 9, (u32 *)Buffer, DevSectorSize,SectorCount);

    if (Status == SD_OK)
      return 0;
    else
      return -1;
}

#if 0
#define BLKGETSIZE _IO(0x12,96)
#define BLKSSZGET _IO(0x12,104)

unsigned int BlockCount = 0;
unsigned int BlockOffset = 0;
static 	int fd;

static int ReadSector(void *Buffer, FS_U16 SectorCount, FS_U32 StartSector) // Public
{
	lseek(fd,(BlockOffset+StartSector)*512,SEEK_SET);
	read(fd,Buffer,SectorCount*512);
	return 0;	
}

static int WriteSector(const void *Buffer, FS_U16 SectorCount, FS_U32 StartSector) // Public
{
	lseek(fd,(BlockOffset+StartSector)*512,SEEK_SET);
	write(fd,Buffer,SectorCount*512);
	return 0;
}
#endif

static int _USB_InitDevice(FS_U8 Unit)
{
	return 0;
}

static int _USB_DevStatus(FS_U8 Unit)
{
	return FS_MEDIA_IS_PRESENT;
}

static int _USB_DevIoCtl(FS_U8 Unit, FS_I32 Cmd, FS_I32 Aux, void *pBuffer) 
{
	FS_DEV_INFO *pInfo;
	
	if(Cmd == FS_CMD_GET_DEVINFO)
	{
		if(!pBuffer)
			return -1;

		pInfo = (FS_DEV_INFO *) pBuffer;
		pInfo->BytesPerSector = DevSectorSize;
		pInfo->NumSectors = DevSectorCount;
	}
	return 0;
}

static int _USB_DevRead(FS_U8 Unit, FS_U32 Sector, void *pBuffer)
{
	return ReadSector(pBuffer, Sector);
}

static int _USB_DevWrite(FS_U8 Unit, FS_U32 Sector, const void *pBuffer)
{
	return WriteSector(pBuffer, Sector);
}

static int _USB_ReadBurst(FS_U8 Unit, FS_U32 SectorNo, FS_U32 NumSectors,
	void *pBuffer)
{
	return ReadMultiSectors(pBuffer, NumSectors, SectorNo);
}

static int _USB_WriteBurst(FS_U8 Unit, FS_U32 SectorNo, FS_U32 NumSectors,
	const void *pBuffer)
{
	return WriteMultiSectors(pBuffer, NumSectors, SectorNo);
}

const FS_DEVICE_TYPE FS__usbdevice_driver =
{
	"usb",
	1,
	_USB_DevStatus,
	_USB_DevRead,
	_USB_DevWrite,
	_USB_DevIoCtl,
	_USB_ReadBurst,
	_USB_WriteBurst,
	_USB_InitDevice,
	NULL
};

void ListFiles(const char * pName);
#if 0
int main(int argc, char *argv[])
{

	struct hd_driveid id;
	struct hd_geometry geom;
	unsigned long size=0,secsize=0;
	unsigned long long lba_capacity;

    	if( argc !=2 )
    	{
    		printf("Usage: device name.\n");
    		return -1;
	}		
	
	fd=open(argv[1],O_RDWR|O_NONBLOCK);
	
	if(fd>0)
	{
		
	if( !ioctl(fd,HDIO_GET_IDENTITY,&id) )
	{
		lba_capacity = id.lba_capacity_2;	
		printf("Serial No=%s,lba_capacity=%ld\n",id.serial_no,lba_capacity);
		BlockCount=(unsigned int)lba_capacity;
	}	    	

	if( !ioctl(fd,BLKGETSIZE,&size) )
	{
		printf("Sectors:%d\n",size);
		BlockCount=size;
	}

	if( !ioctl(fd,BLKSSZGET,&secsize) )
	{
		printf("Sector size:%d\n",secsize);
	}
	
	if( !ioctl(fd,HDIO_GETGEO,&geom) )
	{
		printf("Cylinders:%d,Headers:%d,Sectors:%d\n",geom.cylinders,geom.heads,geom.sectors);
		printf("First sector:%d,Last sector:%d\n",geom.start,geom.start+size-1);
		BlockOffset=geom.start;
	}
	
		
  	// FAT file system initialization
  	if(FS_Init() == -1)
  	{    close(fd);
	     return -1;
   	} 
    	printf("FS_Init() ok!\n");
    
    	if( FS_AddDevice(&FS__usbdevice_driver) != NULL )
    	printf("FS__AddDevice() ok!\n");
        
        ListFiles("");
        ListFiles("usb:\\");
        FS_Exit();
        
        close(fd);
	}
	else
	{
	        printf("can't open %s.\n",argv[1]);
		return -1;
	}	
	return 0;
}
#endif

void ListFiles(const char * pName)
{
   FS_DIR *pDir;
   struct  FS_DIRENT *pDirEnt;
   FS_FILETIME FileTime;
   
   printf("List files in directory %s.\n",pName);
        
   pDir = FS_OpenDir(pName);
   if(pDir) {
      while(1)
      	{
          pDirEnt = FS_ReadDir(pDir);

          if (pDirEnt == NULL) {
             break; /* No more files */
          }
          FS_TimeStampToFileTime(pDirEnt->TimeStamp,&FileTime);
          printf("%16d %04d-%02d-%02d %s\n",pDirEnt->Size,
          FileTime.Year,FileTime.Month,FileTime.Day,pDirEnt->DirName);
    	};
    FS_CloseDir(pDir);
  }else
  {
     printf("Open directory %s failed.\n",pName);
  }
	
}

int uFS_Init(unsigned int DevCapacity,unsigned int DevBlockSize)
{
	DevSectorCount=DevCapacity;
	DevSectorSize=DevBlockSize;
		
  	// FAT file system initialization
  	if(FS_Init() == -1)
  	{
  	  return -1;
   	} 
    	printf("FS_Init() ok!\n");
    
    	if( FS_AddDevice(&FS__usbdevice_driver) != NULL )
    	printf("FS__AddDevice() ok!\n");

#if 0        
        ListFiles("");
        ListFiles("usb:\\");
        FS_Exit();
#endif

	return 0;
}



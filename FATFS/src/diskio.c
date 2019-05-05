/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sdmmc.h"
#include "w25q256.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SD_CARD 	0 //SD ��,���Ϊ 0
#define EX_FLASH 	1 //�ⲿ spi flash,���Ϊ 1
#define EX_NAND 	2 //�ⲿ nand flash,���Ϊ 2

//����W25Q256
//ǰ25M�ֽڸ�fatfs��,25M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��6.01M.	ʣ�ಿ��,���ͻ��Լ���	 
#define FLASH_SECTOR_SIZE 	512	
#define FLASH_SECTOR_COUNT 	1024*25*2	//W25Q256,ǰ25M�ֽڸ�FATFSռ��	
#define FLASH_BLOCK_SIZE   	8     		//ÿ��BLOCK��8������		
  
 
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	stat = RES_OK;
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	int result;

	switch (pdrv) {
	case SD_CARD :
		MX_SDMMC1_SD_Init();
		read_sdinfo();
		result = 0;
		// translate the reslut code here

		break;

	case EX_FLASH :
		W25QXX_Init();

		// translate the reslut code here
		result = 0;
		break;

	case EX_NAND :
//		result = USB_disk_initialize();

		// translate the reslut code here

		break;
	default :
		result=1;
		break;
	}
	if(result)
		return STA_NOINIT;
	else
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	uint8_t result;
	if (!count)return RES_PARERR;//count ���ܵ��� 0�����򷵻ز�������
	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here
		result = SD_ReadDisk(buff,sector,count);
		// translate the reslut code here

		break;

	case EX_FLASH :
		// translate the arguments here
		for(;count>0;count--)
		{
			W25QXX_Read(buff,sector*512,512);
			sector++;
			buff+=512;
		}
		result = 0;
		// translate the reslut code here

		break;

	case EX_NAND :
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		break;
	default :result=1;break;
	}
	if(result==1)
		return RES_PARERR;
	else
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint8_t result;
	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		result = SD_WriteDisk((uint8_t*)buff,sector,count);
		while(result)//д����
		{
			MX_SDMMC1_SD_Init();	//���³�ʼ��SD��
			result=SD_WriteDisk((uint8_t*)buff,sector,count);	
			//printf("sd wr error:%d\r\n",res);
		}
		// translate the reslut code here

		break;

	case EX_FLASH :
		// translate the arguments here
		for(;count>0;count--)
		{
			W25QXX_Write((uint8_t*)buff,sector*512,512);
			sector++;
			buff+=512;
		}
		result=0;

		// translate the reslut code here

		break;

	case EX_NAND :
		// translate the arguments here

//		res = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		break;
	default : 
		result=1;
		break;
	}

	if(result==1)
		return RES_PARERR;
	else
		return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	switch (pdrv) {
	case SD_CARD :
		switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.BlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = (SDCardInfo.BlockNbr*SDCardInfo.BlockSize)/512;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
		// Process of the command for the RAM drive
	case EX_FLASH :
		 switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
		// Process of the command for the MMC/SD card

	case EX_NAND :

		// Process of the command the USB drive

		break;
	}
	return res;
}
//���ʱ��
//User defined function to give a current time to fatfs module */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime (void)
{
	return 0;
}


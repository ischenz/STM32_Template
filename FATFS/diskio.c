/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h"
#include "sdio_sdcard.h"

#define FLASH_SECTOR_SIZE 	512		 //����������С
#define FLASH_BLOCK_SIZE 	8		 //ÿ��block��8������
//#define FLASH_SECTOR_COUNT  2048*1  //ָ����������-->FATFS���ô�СΪ2M
uint16_t FLASH_SECTOR_COUNT = 2048*2;
/* Definitions of physical drive number for each drive */
//#define DEV_RAM		  0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		  1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB	      2	/* Example: Map USB MSD to physical drive 2 */
#define SD_CARD		  0
#define DEV_SPI_FLASH 1


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_Init();//SD����ʼ�� 
  			break;
		case DEV_SPI_FLASH://�ⲿflash
			W25QXX_Init();
			FLASH_SECTOR_COUNT=2048*2;//W25Q1218,ǰ2M�ֽڸ�FATFSռ�� 
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ�
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_ReadDisk(buff,sector,count);	 
			while(res)//������
			{
				SD_Init();	//���³�ʼ��SD��
				res=SD_ReadDisk(buff,sector,count);	
				//printf("sd rd error:%d\r\n",res);
			}
			break;
		case DEV_SPI_FLASH://�ⲿflash
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_WriteDisk((u8*)buff,sector,count);
			while(res)//д����
			{
				SD_Init();	//���³�ʼ��SD��
				res=SD_WriteDisk((u8*)buff,sector,count);	
				//printf("sd wr error:%d\r\n",res);
			}
			break;
		case DEV_SPI_FLASH://�ⲿflash
			for(;count>0;count--)
			{										    
				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	
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
	if(pdrv==SD_CARD)//SD��
	{
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
				*(WORD*)buff = SDCardInfo.CardBlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SDCardInfo.CardCapacity/512;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else if(pdrv==DEV_SPI_FLASH)	//�ⲿFLASH  
	{
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
	}else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}




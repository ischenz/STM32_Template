#include "fatfs.h"
#include "usart.h"
#include "delay.h"

DIR dir;  
FILINFO fileinfo;
FATFS fs_flash,fs_sd;
FIL fil;        /* File object */
uint8_t mf_scan_files(uint8_t * path)
{
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			printf("%s/", path);//打印路径	
			printf("%s\r\n",  fn);//打印文件名	  
		} 
    }	  
    return res;	  
}

void fatfs_init()
{
	int res;
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */
	res = f_mount(&fs_flash,"1:",1);
	printf("res:%#X \r\n",res);
	if(!res)//挂载外部FLASH)
	{
		printf("mount FLASH success!\r\n");
	}
	else printf("mount failure!\r\n");
	
	res = f_mount(&fs_sd,"0:",1);
	printf("res:%#X \r\n",res);
	if(!res)//挂载外部FLASH)
	{
		printf("mount SD success!\r\n");
	}
	else printf("mount failure!\r\n");
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		printf("Flash Disk Formatting...\r\n");	//格式化FLASH
		res=f_mkfs("1:", 0, NULL,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			printf("Flash Disk Format Finish \r\n");	//格式化完成
		}else printf("Flash Disk Format Error \r\n");	//格式化失败
		delay_ms(1000);
	}
	
	mf_scan_files("1:");
	/* Open a text file */
    fr = f_open(&fil, "1:log.txt", FA_READ); 
    if (fr) printf("Open fail :%d\r\n",(int)fr);
    while (f_gets(line, sizeof line, &fil)) {
        printf("%s\r\n",line);
    }
	f_close(&fil);

	printf("End.......\r\n");

//	Mylog("Hello");
//	Mylog("I am number 2\r\n");
//	copytosd();
}

UINT* Mylog(const char* log)
{
	FRESULT fr;     /* FatFs return code */
	UINT* bw;
	fr = f_open(&fil, "1:log.txt", FA_WRITE|FA_OPEN_APPEND); 
	fr = f_write(&fil, log,strlen(log), bw);
	if (fr) printf("Write fail :%d\r\n",(int)fr);
	f_close(&fil);
	return bw;
}

int copytosd(void)
{
    FIL fsrc, fdst;      /* File objects */
    BYTE buffer[4096];   /* File copy buffer */
    FRESULT fr;          /* FatFs function common result code */
    UINT br, bw;         /* File read/write count */

    /* Open source file on the drive 1 */
    fr = f_open(&fsrc, "1:log.txt", FA_READ);
    if (fr) return (int)fr;

    /* Create destination file on the drive 0 */
    fr = f_open(&fdst, "0:file.bin", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr) return (int)fr;

    /* Copy source to destination */
    for (;;) {
        fr = f_read(&fsrc, buffer, sizeof buffer, &br); /* Read a chunk of data from the source file */
        if (br == 0) break; /* error or eof */
        fr = f_write(&fdst, buffer, br, &bw);           /* Write it to the destination file */
        if (bw < br) break; /* error or disk full */
    }

    /* Close open files */
    f_close(&fsrc);
    f_close(&fdst);

    return (int)fr;
}


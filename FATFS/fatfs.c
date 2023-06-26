#include "fatfs.h"
#include "usart.h"
#include "delay.h"

 
FATFS fs_flash,fs_sd;

uint8_t mf_scan_files(uint8_t * path)
{
	DIR dir; 
	FILINFO fileinfo;
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	{	
		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
	        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			printf("%s/", path);//��ӡ·��	
			printf("%s\r\n",  fn);//��ӡ�ļ���	  
		} 
		printf("\r\n");
    }	  
    return res;	  
}

void fatfs_init()
{
	int res;
	printf("Fatfs Init Start.......\r\n");
	res = f_mount(&fs_flash,"1:",1);
	if(!res)//�����ⲿFLASH)
	{
		printf("mount FLASH success!\r\n");
	}
	else printf("mount FLASH failure:%d\r\n",res);
	
	res = f_mount(&fs_sd,"0:",1);
	if(!res)//�����ⲿFLASH)
	{
		printf("mount SD success!\r\n");
	}
	else printf("mount SD failure:%d\r\n",res);
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		BYTE work[1024];
		printf("Flash Disk Formatting...\r\n");	//��ʽ��FLASH
		res=f_mkfs("1:", 0, work,sizeof work);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
			printf("Flash Disk Format Finish \r\n");	//��ʽ�����
		}else printf("Flash Disk Format Error: %d\r\n",res);	//��ʽ��ʧ��
		delay_ms(1000);
	}
	
	mf_scan_files("1:");
//	/* Open a text file */
//    fr = f_open(&fil, "1:log.txt", FA_READ); 
//    if (fr) printf("Open fail :%d\r\n",(int)fr);
//    while (f_gets(line, sizeof line, &fil)) {
//        printf("%s\r\n",line);
//    }
//	f_close(&fil);

	printf("Fatfs Init End.......\r\n\r\n");

//	Mylog("Hello");
//	Mylog("I am number 2\r\n");
//	copytosd();
}

UINT* Mylog(const char* log)
{
	FIL fil;        /* File object */
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


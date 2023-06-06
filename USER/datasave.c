#include "datasave.h"
#include "cJSON.h"
#include "fatfs.h"
#include "usart.h"

//∂¡»°
void read_pid(const char *pid, double *p, double *i, double *d)
{
	cJSON *cjson,*l_pid,*pid_item;
	char filename[12];
	BYTE buffer[100];
	UINT br;         /* File read/write count */
	FIL fil;        /* File object */
	FRESULT fr;     /* FatFs return code */
	sprintf(filename, "1:%s", pid);
	printf("Read file:%s\r\n",filename);
	fr = f_open(&fil, filename, FA_READ | FA_OPEN_EXISTING); 
	if(fr)	{
		printf("Open fail :%d\r\n",(int)fr);
	}
	
	fr = f_read(&fil, buffer, sizeof buffer, &br);
	printf("Read sixe %d:\r\n",br);
	for(int i=0;i<br;i++){
		printf("%c",buffer[i]);
	}
	printf("\r\n");
	
	cjson = cJSON_Parse((char*)buffer);
	l_pid = cJSON_GetObjectItem(cjson, pid);
	pid_item = cJSON_GetArrayItem(l_pid, 0);
	*p = pid_item->valuedouble;
	pid_item = cJSON_GetArrayItem(l_pid, 1);
	*i = pid_item->valuedouble;
	pid_item = cJSON_GetArrayItem(l_pid, 2);
	*d = pid_item->valuedouble;
	f_close(&fil);
}

//–¥»Î
void write_pid(const char *pid, double p, double i, double d)
{
	char buffer[50],filename[12];
	sprintf(filename, "1:%s", pid);
	sprintf(buffer,"{\"%s\":[%f,%f,%f]}", pid, p, i, d);
	
	UINT bw;         /* File read/write count */
	FIL fil;        /* File object */
	FRESULT fr;     /* FatFs return code */
	printf("Create file: %s\r\n", filename);
	fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS); 
	if(fr)	{
		printf("Open fail :%d\r\n",(int)fr);
	}
	mf_scan_files("1:");
	
	fr = f_write(&fil, buffer, strlen(buffer), &bw);           /* Write it to the destination file */
	if(!fr){
		printf("Write string: %s\r\n",buffer);
	} else {
		printf("Write err: %d\r\n",fr);
	}
	
	f_close(&fil);
}


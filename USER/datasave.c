#include "datasave.h"
#include "cJSON.h"
#include "fatfs.h"
#include "usart.h"
#include "stmflash.h"

//��ȡ
void read_pid_from_spiflash(const char *pid, double *p, double *i, double *d)
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
	cJSON_Delete(cjson);
}

int8_t read_json_pid(const char *str, const char *pid, double *p, double *i, double *d)
{
	int8_t ret = 0;
	cJSON *cjson,*cj_pid,*pid_item;
	const char *buffer = str;
	cjson = cJSON_Parse(buffer);
	cj_pid = cJSON_GetObjectItem(cjson, pid);
	if(strcmp(cj_pid->string, pid) == 0){
		ret = 0;
		pid_item = cJSON_GetArrayItem(cj_pid, 0);
		*p = pid_item->valuedouble;
		pid_item = cJSON_GetArrayItem(cj_pid, 1);
		*i = pid_item->valuedouble;
		pid_item = cJSON_GetArrayItem(cj_pid, 2);
		*d = pid_item->valuedouble;
	} else {
		ret = 1;
	}
	cJSON_Delete(cjson);
	return ret;
}

void write_pid_to_spiflash(const char *pid, double p, double i, double d)
{
	char buffer[80],filename[12];
	sprintf(filename, "1:%s", pid);
	sprintf(buffer,"{\"%s\":[%f,%f,%f]}", pid, p, i, d);
	
	UINT bw;        /* File read/write count */
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
	
	fr = f_close(&fil);
	if(!fr){
		printf("Close OK\r\n");
	} else {
		printf("Close err: %d\r\n",fr);
	}
}

void pid_flash_root_init(uint32_t addr)
{
	uint32_t writebuff = 0xffffffff;
	for(uint16_t i=0;i<128*1024/4;i++){
		STMFLASH_Write(addr, &writebuff, 1);
	}	
}

void write_pid_to_flash(uint32_t addr, const char *pid, double p, double i, double d)
{
	char buffer[80] = {0};
	char byte_size = sizeof(buffer);
	char size = byte_size/4+((byte_size%4)?1:0);
	sprintf(buffer,"{\"%s\":[%f,%f,%f]}", pid, p, i, d);
	STMFLASH_Write(addr, (uint32_t *)buffer, size);
}

void read_pid_from_flash(uint32_t addr, const char *pid, double *p, double *i, double *d)
{
	char readbuff[80] = {0};
	STMFLASH_Read(addr,(uint32_t *)readbuff,sizeof(readbuff)/4);
	if(read_json_pid(readbuff, pid, p, i, d) == 0){
		//printf("read pid json ok\r\n");
	}else{
		printf("read pid json err\r\n");
	}
}

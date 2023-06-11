/*
 ******************************************************
 STM32F407ZG KEIL例程 大越创新开发板

 202207081915	创建例程
 202305281734	改进
 202305302021	添加版本管理
 202306051313	移植u8g2,cJSON
 202306061142	在spiflash中存取pid参数

 作者：			ischenz						V2.0
 ******************************************************
 */
#include "oled.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "motor.h"
#include "u8g2.h"
#include "u8g2_init.h"
#include "fatfs.h"
#include "datasave.h"

char str[100] = {0};

int main(void)
{
	double p,i,d;
	int j = 0;
	uint8_t readbuff,reading = 0,ok = 0;
	char showstr[50];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(9600);
	LED_Init();
	RingBuff_Init(&Uart1_RingBuff);
//	fatfs_init();
//	write_pid("l_pid",140.2547, 3450.16575, 3253.6450);
//	read_pid("l_pid",&p,&i,&d);
//	printf("p=%4.2f   i=%4.2f   d=%4.2f\r\n",p,i,d);
	
	u8g2_init();
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_template);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

	KEY_Init();//weak不能使用 与定时器通道冲突
	
	u8g2_SetFont(&u8g2,u8g2_font_12x6LED_tf);
	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawStr(&u8g2,0,11,"Hello World");
	u8g2_SendBuffer(&u8g2); 
	printf("hello chen\r\n");
	while(1)
	{
		if(Read_RingBuff(&Uart1_RingBuff,&readbuff) == RINGBUFF_OK){
			if(readbuff == '{'){
				reading = 1;
				str[j++] = readbuff;
				ok = 0;
			}else if(readbuff == '}' && reading == 1){
				reading = 0;
				str[j++] = readbuff;  
				str[j++] = '\0';
				ok = 1;
			}else if(reading == 1){
				str[j++] = readbuff;
			}else {
				j = 0;
				reading = 0;
				ok = 0;
			}
		}
		if(ok){
			ok = 0;
			printf("%s \r\n", str);
			u8g2_ClearBuffer(&u8g2);
			read_json_pid(str, "pid", &p, &i, &d);
			sprintf(showstr, "P:%5.2f",p);
			u8g2_DrawStr(&u8g2, 0, 25, showstr);
			sprintf(showstr, "I:%5.2f ",i);
			u8g2_DrawStr(&u8g2, 0, 40, showstr);
			sprintf(showstr, "D:%5.2f",d);
			u8g2_DrawStr(&u8g2, 0, 55, showstr);
			u8g2_SendBuffer(&u8g2);
		}
	}
}

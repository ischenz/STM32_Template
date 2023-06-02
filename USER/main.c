/*
 ******************************************************
 STM32F407ZG KEIL例程 大越创新开发板

 202207081915	创建例程
 202305281734	改进
 202305302021	添加版本管理

 作者：			xc						V2.0
 ******************************************************
 */
#include "oled.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
	uint8_t sentdata = 0x55;
	uint16_t distance;
	RingBuff_Init(&Uart3_RingBuff);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(115200);
	uart3_init(9600);
	LED_Init();
	OLED_Init();
	Init_Timer3();
	Init_Timer4();
	Timer7_init();
	//KEY_Init();

	OLED_ShowString(0,0,"Hello !!!",16,1);
	OLED_Refresh();
	delay_ms(500);
	OLED_Clear();
	
	printf("Hello world\r\n");
	

	usart_send(USART3,&sentdata,1);
	while(1)
	{	
		//printf("coder1=%d coder2=%d\r\n", coder1, coder2);
		distance = get_distance();
		if(distance){
			printf("%5d mm\r\n", distance);
		}
		
		
	}
}

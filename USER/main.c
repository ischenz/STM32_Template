/*
 ******************************************************
 STM32F407ZG KEIL例程 大越创新开发板

 202207081915	创建例程
 202305281734	改进

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
	uint16_t cnt;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(115200);
	Init_Timer2();
	LED_Init();
	OLED_Init();
	//KEY_Init();

	OLED_ShowString(0,0,"Hello !!!",16,1);
	delay_ms(500);
	OLED_Clear();
	
	printf("Hello world\r\n");
	
	while(1)
	{	
		cnt = TIM_GetCounter(TIM2);
		printf("cnt=%d\r\n" ,cnt);
		delay_ms(100);
	}
}

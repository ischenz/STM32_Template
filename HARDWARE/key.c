#include "key.h"
#include "led.h"
#include "delay.h"  

int8_t key_value = 0;

//按键初始化函数
void KEY_Init(void)//PA0与定时器2通道1冲突
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3; //KEY0 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE
	
	PWR_WakeUpPinCmd(DISABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
} 
//按键处理函数

uint8_t KEY_Scan(void)
{	
	volatile uint8_t ret = 0;
	static uint8_t release=1;//按键按松开标志	  
	if( release && (KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		release = 0;
		if(KEY0==0)				ret = KEY0_PRES;
		else if(KEY1 == 0)		ret = KEY1_PRES;
		else if(WK_UP == 1)		ret = WKUP_PRES;
	}
	else if( KEY0==1 && KEY1==1 &&  WK_UP==0){
		release = 1;
		ret = 0;
	}
 	return ret;// 无按键按下
}


void KeyAction(void)
{
	uint8_t KEY_Val = 0;
	KEY_Val = KEY_Scan();
	if(KEY_Val)
	{
		if(KEY_Val == WKUP_PRES){
			//LED2 = 0;
		}
		if(KEY_Val == KEY1_PRES){
			LED2 = !LED2;
		}		
	}
}


















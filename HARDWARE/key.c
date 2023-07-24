#include "key.h"
#include "led.h"
#include "delay.h"  
#include "multi_button.h"
#include "timer.h"
#include "usart.h"
#include "gw_grayscale_sensor.h"


int8_t key_value = 0;
enum Button_IDs {
	btn1_id,
	btn2_id,
};

struct Button btn1;
struct Button btn2;

uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case btn1_id:
			return KEY0;
		case btn2_id:
			return KEY1;
		default:
			return 0;
	}
}
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
	
//	PWR_WakeUpPinCmd(DISABLE); 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
	button_init(&btn1, read_button_GPIO, 0, btn1_id);
	button_init(&btn2, read_button_GPIO, 0, btn2_id);

	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

	button_attach(&btn2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
	button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
	button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
	button_attach(&btn2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
	button_attach(&btn2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
	button_attach(&btn2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
	button_attach(&btn2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);

	button_start(&btn1);
	button_start(&btn2);
	Timer9_init();
} 

void BTN1_PRESS_DOWN_Handler(void* btn)
{
}

void BTN1_PRESS_UP_Handler(void* btn)
{
}
void BTN1_PRESS_REPEAT_Handler(void* btn)
{
	//struct Button* button = (Button*)btn;
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
}
void BTN1_DOUBLE_Click_Handler(void* btn)
{
}

void BTN1_LONG_PRESS_START_Handler(void* btn)
{
}
void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
}

void BTN2_PRESS_DOWN_Handler(void* btn)
{
}

void BTN2_PRESS_UP_Handler(void* btn)
{
}
void BTN2_PRESS_REPEAT_Handler(void* btn)
{
	//struct Button* button = (Button*)btn;
}

void BTN2_SINGLE_Click_Handler(void* btn)
{
}
void BTN2_DOUBLE_Click_Handler(void* btn)
{
}

void BTN2_LONG_PRESS_START_Handler(void* btn)
{
}
void BTN2_LONG_PRESS_HOLD_Handler(void* btn)
{
}

//按键处理函数

//uint8_t KEY_Scan(void)
//{	
//	volatile uint8_t ret = 0;
//	static uint8_t release=1;//按键按松开标志	  
//	if( release && (KEY0==0||KEY1==0||WK_UP==1))
//	{
//		delay_ms(10);//去抖动 
//		release = 0;
//		if(KEY0==0)				ret = KEY0_PRES;
//		else if(KEY1 == 0)		ret = KEY1_PRES;
//		else if(WK_UP == 1)		ret = WKUP_PRES;
//	}
//	else if( KEY0==1 && KEY1==1 &&  WK_UP==0){
//		release = 1;
//		ret = 0;
//	}
// 	return ret;// 无按键按下
//}

uint8_t KEY_Scan(void)
{	
	volatile uint8_t ret = 0;
	static uint8_t release=1;//按键按松开标志	  
	if( release && (KEY0==0||KEY1==0))
	{
		delay_ms(10);//去抖动 
		release = 0;
		if(KEY0==0)				ret = KEY0_PRES;
		else if(KEY1 == 0)		ret = KEY1_PRES;
	}
	else if( KEY0==1 && KEY1==1 ){
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


















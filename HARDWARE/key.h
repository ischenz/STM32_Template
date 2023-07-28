#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 


/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define WK_UP 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2

/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/


#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP)

void KEY_Init(void);	//IO初始化
uint8_t KEY_Scan(void); 		//按键扫描函数	
void KeyAction(void);

void BTN1_PRESS_DOWN_Handler(void* btn);
void BTN1_PRESS_UP_Handler(void* btn);
void BTN1_PRESS_REPEAT_Handler(void* btn);
void BTN1_SINGLE_Click_Handler(void* btn);
void BTN1_DOUBLE_Click_Handler(void* btn);
void BTN1_LONG_PRESS_START_Handler(void* btn);
void BTN1_LONG_PRESS_HOLD_Handler(void* btn);

void BTN2_PRESS_DOWN_Handler(void* btn);
void BTN2_PRESS_UP_Handler(void* btn);
void BTN2_PRESS_REPEAT_Handler(void* btn);
void BTN2_SINGLE_Click_Handler(void* btn);
void BTN2_DOUBLE_Click_Handler(void* btn);
void BTN2_LONG_PRESS_START_Handler(void* btn);
void BTN2_LONG_PRESS_HOLD_Handler(void* btn);
extern int8_t key_value;

#endif

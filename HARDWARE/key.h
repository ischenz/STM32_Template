#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 


/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define WK_UP 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/


#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define WKUP_PRES   4	//KEY_UP����(��WK_UP)

void KEY_Init(void);	//IO��ʼ��
uint8_t KEY_Scan(void); 		//����ɨ�躯��	
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

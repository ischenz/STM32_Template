#ifndef __CH451KEY_H
#define __CH451KEY_H

#include "sys.h"

extern uint8_t Ch451Key_Num,Flag_Ch45KeyRead;

#define Ch451KEY_LOAD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_9);
#define CH451KEY_LOAD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_9)
#define Ch451KEY_DIN_Set()  GPIO_SetBits(GPIOG,GPIO_Pin_10)
#define Ch451KEY_DIN_Clr()  GPIO_ResetBits(GPIOG,GPIO_Pin_10)
#define Ch451KEY_DCLK_Set() GPIO_SetBits(GPIOG,GPIO_Pin_11);
#define Ch451KEY_DCLK_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_11)
#define Ch451KEY_DOUT()     GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)

#define 	KeyNum0       0x40
#define  	KeyNum1       0x41       
#define  	KeyNum2       0x42       
#define  	KeyNum3       0x43       
#define  	KeyNum4       0x48		  
#define  	KeyNum5       0x49       
#define  	KeyNum6       0x4a       
#define  	KeyNum7       0x4b		  
#define  	KeyNum8       0x50		  
#define  	KeyNum9       0x51       		  
#define  	KeyNum10      0x52
#define  	KeyNum11 	  0x53
#define  	KeyNum12	  0x58				  
#define  	KeyNum13      0x59
#define  	KeyNum14      0x5a
#define  	KeyNum15      0x5b

void Ch451Key_init(void);
void ch451_write(uint16_t command);
void Get_ch451_value(int *value);

int8_t InputValue(float *value);

#endif

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

void Ch451Key_init(void);
void ch451_write(uint16_t command);
static void Get_ch451Key_Value(uint8_t * KeyNum);

#endif

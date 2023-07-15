#ifndef __LED_H
#define __LED_H

#include "sys.h"

//LED端口定义
#define LED1 PFout(9)
#define LED2 PFout(10)

void LED_Init(void);//初始化		 

#endif

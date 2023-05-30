#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern uint8_t timeout;

void Init_Timer2(void);
void Init_Timer3(void);
void Timer1_PWM_GPIO_Init(uint16_t Psc, uint16_t Per);
void Timer6_init(void);
void set_time(uint16_t time_ms);

#endif

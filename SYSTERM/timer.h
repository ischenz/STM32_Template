#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern uint8_t timeout;

void Timer1_PWM_GPIO_Init(uint16_t Psc, uint16_t Per);
void Init_Timer3(void);
void Init_Timer4(void);
void Timer5_Init(uint16_t Psc, uint16_t Per);
void Timer6_init(void);
void Timer7_init(void);
void Timer9_init(void);
void set_time(uint16_t time_ms);

#endif

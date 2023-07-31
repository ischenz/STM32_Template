/**
  ************************************************
  * @file     servo.h
  * @author   xiangchen
  * @version  V1.0
  * @date     2023/7/28
  * @brief    Servo Controller
  * @Log
  ************************************************
  */

#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx.h"

typedef struct {
	uint16_t pwm;
	const uint16_t MAXPWM;
	const uint16_t MINPWM;
	const uint16_t MIDPWM;
}ServTypdef;
extern ServTypdef Xserv,Yserv;

void servo_init(void);
void servo_ctr(ServTypdef *servo, int PidInput);

#endif /* __SERVO_H */



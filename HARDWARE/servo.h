#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx.h"

typedef struct {
	uint16_t pwm;
	const uint16_t MAXPWM;
	const uint16_t MINPWM;
	const uint16_t MIDPWM;
}ServTyp;

extern ServTyp Xserv,Yserv;

void servo_init(void);
void servo_ctr(ServTyp *servo, int PidInput);

#endif /* __SERVO_H */



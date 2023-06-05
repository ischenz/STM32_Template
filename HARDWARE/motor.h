#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "sys.h" 

#define Xin1  PDout(0)
#define Xin2  PDout(1)
			   
#define Yin1  PDout(2)
#define Yin2  PDout(3)

typedef struct{
	int16_t pwm;
	int16_t coder_v;
	const int16_t PWM_MAX;
}motor;

extern motor motor_l, motor_r;

void Motor_Gpio_Init(void);
int My_abs(int x);
void PWM_Load(motor* moto, int16_t pwmin);
void motor_stop(void);
void motor_start(void);
#endif


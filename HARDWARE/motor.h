#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "sys.h" 

#define Xin1  PBout(6)
#define Xin2  PBout(7)

#define Yin1  PBout(9)
#define Yin2  PBout(8)


void Motor_Gpio_Init(void);
void PWM_Limit(int16_t *moto);
int My_abs(int x);
void PWM_Load(int moto_x,int moto_y);
void motor_stop(void);
void motor_start(void);
#endif


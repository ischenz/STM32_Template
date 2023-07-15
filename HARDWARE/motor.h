#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "sys.h" 

#define Xin1  PDout(0)
#define Xin2  PDout(1)
			   
#define Yin1  PDout(2)
#define Yin2  PDout(3)

typedef enum
{ 
	MOTOR_STATUS_STOP 	= 0x00,
	MOTOR_STATUS_ACTIVE = 0x01,
	MOTOR_STATUS_FREE   = 0x02
}MOTOR_STATUS_TypeDef;

typedef struct{
	int16_t pwm;
	int16_t coder_v;
	const int16_t PWM_MAX;
	uint8_t status;
}motor;

extern motor motor_l, motor_r;

void Motor_Gpio_Init(void);
int My_abs(int x);
void PWM_Load(motor* moto, int16_t pwmin);
void motor_set_status(motor* moto, MOTOR_STATUS_TypeDef status);
void motor_stop(void);
void motor_start(void);
#endif


#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

#define SET_BASIC_TIM_PERIOD(T)	TIM_SetAutoreload(TIM10, (T)*1000 - 1)

typedef struct{
    float Kp;
    float Ki;
	float Kd;
    float Err;
    float LastErr;
	float PenultErr;
    float Integral;//积分和
    float Target;
	float PID_out;
	
	int8_t KP_polarity;
	int8_t KI_polarity;
	int8_t KD_polarity;
}PID_TypeDef;


float pid_calculate(PID_TypeDef *PID,float CurrentValue);
float pid_calculate_inc(PID_TypeDef *PID,float CurrentValue);
void set_pid_target(PID_TypeDef *pid, float target);
float get_pid_target(PID_TypeDef *pid);
void set_pid_param(PID_TypeDef *pid, float p, float i, float d);
void pid_timer_init(void);
void pid_param_init(PID_TypeDef *pid);
void set_pid_polarity(PID_TypeDef *pid, int8_t p_polarity, int8_t i_polarity, int8_t d_polarity);

extern PID_TypeDef l_pid, r_pid, veer_pid;

#endif /* PID_H */


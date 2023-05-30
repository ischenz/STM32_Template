#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

#define SET_BASIC_TIM_PERIOD(T)	TIM_SetAutoreload(TIM10, (T)*1000 - 1)

typedef struct{
    float ProportionConstant;
    float IntegralConstant;
	float DerivativeConstant;
    float Err;
    float LastErr;
	float PenultErr;
    float Integral;//»ý·ÖºÍ
    float Target;
	int16_t PID_out;
	
	int8_t KP_polarity;
	int8_t KI_polarity;
	int8_t KD_polarity;
}PID_TypeDef;


int16_t PID_Calculate(PID_TypeDef *PID,float CurrentValue);
void set_pid_target(PID_TypeDef *pid, float target);
float get_pid_target(PID_TypeDef *pid);
void set_p_i_d(PID_TypeDef *pid, float p, float i, float d);
void PID_TimerInit(void);
void PID_param_init(PID_TypeDef *pid);
void set_pid_polarity(PID_TypeDef *pid, int8_t p_polarity, int8_t i_polarity, int8_t d_polarity);

extern PID_TypeDef x_pid,y_pid;

#endif /* PID_H */


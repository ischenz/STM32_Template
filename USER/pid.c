#include "pid.h"
#include "control.h"
#include "motor.h"

PID_TypeDef l_pid, r_pid, veer_pid;
extern float line_P;

/**
 * @brief       PID初始化
 * @param       目标值
 * @retval      无
 */
void PID_param_init(PID_TypeDef *pid)
{
	pid->Target = 0;//目标值
	pid->PID_out = 0;
    pid->Kp = 0;
    pid->Ki= 0;
	pid->Kd = 0;
    pid->Err = 0.0f;
    pid->LastErr = 0.0f;
	pid->PenultErr = 0.0f;
    pid->Integral = 0.0f;//积分值
	
	pid->KP_polarity = 1;
	pid->KI_polarity = 1;
	pid->KD_polarity = 1;
}

/**
 * @brief       pid闭环控制计算
 * @param       *PID：PID结构体变量地址
 * @param       CurrentValue：当前测量值
 * @retval      期望输出值
 */
float PID_Calculate(PID_TypeDef *PID,float CurrentValue)
{
	PID->Integral += PID->Err;
	/*积分分离*/
//	if( (PID->Err > 36) || (PID->Err < -36) ){
//		PID->Integral = 0;
//		//PID->PID_out += PID->IntegralConstant * PID->Integral;
//	}
	
    PID->Err =  PID->Target - CurrentValue;
    PID->PID_out = PID->Kp * PID->Err 										/*比例*/
				 + PID->Ki * PID->Integral  								/*积分*/
			     + PID->Kd * (PID->Err - PID->LastErr);						/*微分*/
	
	/*积分限幅*/
	if(PID->Integral > 3000){
		PID->Integral = 3000;
	}
	if(PID->Integral < -3000){
		PID->Integral = -3000;
	}
	PID->LastErr = PID->Err;
    return PID->PID_out;
}

/**
 * @brief       pid闭环控制计算（增量式）
 * @param       *PID：PID结构体变量地址
 * @param       CurrentValue：当前测量值
 * @retval      期望输出值
 */
float PID_Calculate_Inc(PID_TypeDef *PID,float CurrentValue)
{
	float increment_val;
    PID->Err =  PID->Target - CurrentValue;
	
    increment_val =  PID->Kp * (PID->Err - PID->LastErr) 						/*比例*/
				   + PID->Ki *  PID->Err  										/*积分*/
			       + PID->Kd * (PID->Err - 2*PID->LastErr + PID->PenultErr);	/*微分*/

	PID->PID_out += increment_val;
	
	PID->PenultErr = PID->LastErr;
	PID->LastErr = PID->Err;
    return PID->PID_out;
}

void PID_TimerInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//10ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=167;
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);	
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//TIM_Cmd(TIM10,ENABLE);
}

void TIM1_UP_TIM10_IRQHandler(void)//10ms一次pid运算
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //溢出中断
	{	
//		if(RecCoorFlag){
//			RecCoorFlag = 0;
//			//mode_task();
			//PID_Calculate(&veer_pid, line_P);
			PID_Calculate_Inc(&r_pid, motor_r.coder_v);
			PID_Calculate_Inc(&l_pid, motor_l.coder_v);
			
			PWM_Load(&motor_l,l_pid.PID_out );
			PWM_Load(&motor_r,r_pid.PID_out );
//		}
	}
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update); //清除中断标志位	
}

/**
  * @brief  获取目标值
  * @param  无
  *	@note 	无
  * @retval 目标值
  */
float get_pid_target(PID_TypeDef *pid)
{
  return pid->Target;    // 设置当前的目标值
}

void set_pid_target(PID_TypeDef *pid, float target)
{
	pid->Target = target;
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  *	@note 	无
  * @retval 无
  */
void set_p_i_d(PID_TypeDef *pid, float p, float i, float d)
{
  	pid->Kp = p * (pid->KP_polarity);    // 设置比例系数 P
	pid->Ki = i * (pid->KI_polarity);    // 设置积分系数 I
	pid->Kd = d * (pid->KD_polarity);    // 设置微分系数 D
}

void set_pid_polarity(PID_TypeDef *pid, int8_t p_polarity, int8_t i_polarity, int8_t d_polarity)
{
	pid->KP_polarity = p_polarity;
	pid->KI_polarity = i_polarity;
	pid->KD_polarity = d_polarity;
}




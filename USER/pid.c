#include "pid.h"
#include "pidtool.h"

/**
 * @brief       PID初始化
 * @param       目标值
 * @retval      无
 */
void PID_param_init(PID_TypeDef *pid)
{
	pid->Target = 0;//目标值
	pid->PID_out = 0;
    pid->ProportionConstant = 0;
    pid->IntegralConstant = 0;
	pid->DerivativeConstant = 0;
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
int16_t PID_Calculate(PID_TypeDef *PID,float CurrentValue)
{
	PID->Integral += PID->Err;
	/*积分分离*/
	if( (PID->Err > 36) || (PID->Err < -36) ){
		PID->Integral = 0;
		//PID->PID_out += PID->IntegralConstant * PID->Integral;
	}
	
    PID->Err =  PID->Target - CurrentValue;
    PID->PID_out = PID->ProportionConstant * PID->Err 										/*比例*/
				 + PID->IntegralConstant * PID->Integral  									/*积分*/
			     + PID->DerivativeConstant * (PID->Err - PID->LastErr);						/*微分*/
	
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

void PID_TimerInit(void)
{
//	uint32_t temp = 5000;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=5000-1;//5ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=167;
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);	
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//TIM_Cmd(TIM10,ENABLE);
//	set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);
//	set_computer_value(SEND_PERIOD_CMD, CURVES_CH2, &temp, 1);
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
  	pid->ProportionConstant = p * (pid->KP_polarity);    // 设置比例系数 P
	pid->IntegralConstant = i * (pid->KI_polarity);    // 设置积分系数 I
	pid->DerivativeConstant = d * (pid->KD_polarity);    // 设置微分系数 D
}

void set_pid_polarity(PID_TypeDef *pid, int8_t p_polarity, int8_t i_polarity, int8_t d_polarity)
{
	pid->KP_polarity = p_polarity;
	pid->KI_polarity = i_polarity;
	pid->KD_polarity = d_polarity;
}




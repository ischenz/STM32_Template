#include "pid.h"
#include "control.h"
#include "motor.h"

PID_TypeDef l_pid, r_pid, veer_pid;
extern float line_P;

/**
 * @brief       PID��ʼ��
 * @param       Ŀ��ֵ
 * @retval      ��
 */
void PID_param_init(PID_TypeDef *pid)
{
	pid->Target = 0;//Ŀ��ֵ
	pid->PID_out = 0;
    pid->Kp = 0;
    pid->Ki= 0;
	pid->Kd = 0;
    pid->Err = 0.0f;
    pid->LastErr = 0.0f;
	pid->PenultErr = 0.0f;
    pid->Integral = 0.0f;//����ֵ
	
	pid->KP_polarity = 1;
	pid->KI_polarity = 1;
	pid->KD_polarity = 1;
}

/**
 * @brief       pid�ջ����Ƽ���
 * @param       *PID��PID�ṹ�������ַ
 * @param       CurrentValue����ǰ����ֵ
 * @retval      �������ֵ
 */
int16_t PID_Calculate(PID_TypeDef *PID,float CurrentValue)
{
	PID->Integral += PID->Err;
	/*���ַ���*/
//	if( (PID->Err > 36) || (PID->Err < -36) ){
//		PID->Integral = 0;
//		//PID->PID_out += PID->IntegralConstant * PID->Integral;
//	}
	
    PID->Err =  PID->Target - CurrentValue;
    PID->PID_out = PID->Kp * PID->Err 										/*����*/
				 + PID->Ki * PID->Integral  								/*����*/
			     + PID->Kd * (PID->Err - PID->LastErr);						/*΢��*/
	
	/*�����޷�*/
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
 * @brief       pid�ջ����Ƽ��㣨����ʽ��
 * @param       *PID��PID�ṹ�������ַ
 * @param       CurrentValue����ǰ����ֵ
 * @retval      �������ֵ
 */
int16_t PID_Calculate_Inc(PID_TypeDef *PID,float CurrentValue)
{
	float increment_val;
    PID->Err =  PID->Target - CurrentValue;
	
    increment_val =  PID->Kp * (PID->Err - PID->LastErr) 						/*����*/
				   + PID->Ki *  PID->Err  										/*����*/
			       + PID->Kd * (PID->Err - 2*PID->LastErr + PID->PenultErr);	/*΢��*/

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

void TIM1_UP_TIM10_IRQHandler(void)//10msһ��pid����
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update); //����жϱ�־λ	
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(PID_TypeDef *pid)
{
  return pid->Target;    // ���õ�ǰ��Ŀ��ֵ
}

void set_pid_target(PID_TypeDef *pid, float target)
{
	pid->Target = target;
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
  *	@note 	��
  * @retval ��
  */
void set_p_i_d(PID_TypeDef *pid, float p, float i, float d)
{
  	pid->Kp = p * (pid->KP_polarity);    // ���ñ���ϵ�� P
	pid->Ki = i * (pid->KI_polarity);    // ���û���ϵ�� I
	pid->Kd = d * (pid->KD_polarity);    // ����΢��ϵ�� D
}

void set_pid_polarity(PID_TypeDef *pid, int8_t p_polarity, int8_t i_polarity, int8_t d_polarity)
{
	pid->KP_polarity = p_polarity;
	pid->KI_polarity = i_polarity;
	pid->KD_polarity = d_polarity;
}




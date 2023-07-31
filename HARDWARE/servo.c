/**
  * @file    servo.c
  * @brief   servo controller
  * @version V1.0
  * @author  xiangchen
  * @date    2023/7/28
  */
#include "servo.h"
#include "usart.h"
#include "pid.h"

ServTypdef Xserv = {
    .pwm = 1500,
	.MAXPWM = 2000,
	.MINPWM = 1000,
	.MIDPWM = 1500
};

ServTypdef Yserv = {
    .pwm = 1500,
	.MAXPWM = 2000,
	.MINPWM = 1000,
	.MIDPWM = 1500
};

/*
 * @Description: 函数描述，描述本函数的基本功能9
 * @param 1 – 参数 1.
 * @param 2 – 参数 2
 * @return – 返回值
 */
void servo_ctr(ServTypdef *servo, int PidInput)
{
	uint16_t CompSet = servo->MIDPWM + PidInput;
	if( CompSet > servo->MAXPWM ){
		servo->pwm = servo->MAXPWM;
	}
	else if( CompSet < servo->MINPWM  ){
		servo->pwm = servo->MINPWM;
	}
	else {
		servo->pwm = CompSet;
	}
	
	if( servo == &Xserv ){
		TIM_SetCompare1(TIM2, servo->pwm);
	}
	else if( servo == &Yserv ){
		TIM_SetCompare2(TIM2, servo->pwm);
		//printf("comp:%d \t y:%.2f \t pid:%d \r\n", servo->pwm, after_kalman_y-120, PidInput);
	}
}

void servo_init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure PA5 as alternate function (TIM2_CH1) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM2 pins to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* Enable TIM2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Configure TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 3100 - 1;  // PWM period
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 84MHz clock, 1kHz PWM frequency
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Configure TIM2 channel 1 in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	/* Configure TIM2 channel 2 in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* Start TIM2 */
	TIM_Cmd(TIM2, ENABLE);
}

/*  若启用定时器溢出中断，则溢出时可以改变通道比较值(pwm占空比)
 *  void TIM2_IRQHandler(void)
 *  {
 *  	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
 *  	{
 *  		servo_ctr(&Xserv, x_pid.PID_out);
 *  		servo_ctr(&Yserv, y_pid.PID_out);
 *  	}
 *  	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除中断标志位
 *  }
 */

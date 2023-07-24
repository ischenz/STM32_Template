#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "sys.h"

//#define	USART_ULTRASONIC

/*usart模式*/
#ifdef 	USART_ULTRASONIC
#define ULTRASONIC_USART					USART3
#define ULTRASONIC_RINGBUFF					Uart3_RingBuff

uint16_t Ultrasound_GetDistance(void);
void ultrasonic_init(void);

/*电平模式*/
#else
//超声波模块引脚配置
#define ULTRASOUND_GPIO_PORT				GPIOA
#define ULTRASOUND_GPIO_CLK 				RCC_AHB1Periph_GPIOA
#define ULTRASOUND_GPIO_TRIG_PIN 			GPIO_Pin_1
#define ULTRASOUND_GPIO_ECHO_PIN			GPIO_Pin_0

//超声波模块ECHO中断配置
#define ULTRASOUND_GPIOSourceGPIO   		EXTI_PortSourceGPIOE
#define ULTRASOUND_PINSOURCE        		GPIO_PinSource0
#define ULTRASOUND_EXTI_LINE				EXTI_Line0
#define ULTRASOUND_EXTI_IRQHandler  		EXTI0_IRQHandler	//ECHO引脚中断服务函数

//超声波模块定时器配置
#define ULTRASOUND_TIM 						TIM10
#define ULTRASOUND_TIM_CLK_ENABLE			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE)
#define ULTRASOUND_TIM_IRQ      			TIM1_UP_TIM10_IRQn
#define ULTRASOUND_TIM_IRQHandler			TIM1_UP_TIM10_IRQHandler	//TIM2中断服务函数

#define ULTRASOUND_DIS_INDEX 				10

extern uint8_t ultrasound_flag;//获取完成标志位变量
extern float ultrasound_data[ULTRASOUND_DIS_INDEX];//距离数据存储数组

void Ultrasound_Init(void);//模块使用初始
void Ultrasound_start(void);//启动一次
float Ultrasound_GetDistance(void);//获取距离数据

#endif

#endif /* __ULTRASONIC_H */


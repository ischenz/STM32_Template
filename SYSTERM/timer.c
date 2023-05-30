#include "timer.h"
#include "led.h"
#include "usart.h"
#include "oled.h"
#include "pid.h"

uint8_t timeout = 0;

void Timer1_PWM_GPIO_Init(uint16_t Psc, uint16_t Per)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA11复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;  //TIM1_CH1  TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=Per;
	TIM_TimeBaseInitStruct.TIM_Prescaler=Psc;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//初始化输出比较,TIMx_CNT<TIM_CCRx的时候为有效电平
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//有效电平为高
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	//TIM_OCInitStruct.TIM_Pulse=0;//对应CCR的值 比较值
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//ENABLE//OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//ENABLE//OC4预装载寄存器使能	
	
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	TIM_SetCompare1(TIM1 ,0);
	TIM_SetCompare4(TIM1 ,0);
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

//Tim2-5通用定时器

void Init_Timer2(void)//配置为编码器模式 A0->CH1 A1->CH2 *******A0与按键冲突
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  //TIM1_CH1  TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2); //GPIOA11复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); 	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//APB1:42Mhz 定时器3：84Mhz/840 = 100Khz  50ms
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//编码器模式
	TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//TIM_ICPolarity_Rising捕获发生在ICx的上升沿
    TIM_ICStructInit(&TIM_ICInitStructure); //将结构体中的内容缺省输入
    TIM_ICInitStructure.TIM_ICFilter = 0;//滤波器值
    TIM_ICInit(TIM2, &TIM_ICInitStructure);  
	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
	
}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
//	{
//		
//	}
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除中断标志位
//}

void Init_Timer3(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//APB1:42Mhz 定时器3：84Mhz/840 = 100Khz  50ms
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//计时
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 840-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位
}


void Timer6_init(void)//基本定时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//APB1:42Mhz 定时器6：84Mhz/840 = 10Khz  0.0001s/count  6s
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//计时
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8400-1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM6, DISABLE);
}

void Timer7_init(void)//基本定时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//APB1:42Mhz 定时器6：84Mhz/840 = 10Khz  0.0001s/count  6s
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//计时
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8400-1;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM7, DISABLE);
}

void set_time(uint16_t time_ms)
{
	timeout = 0;
	if(time_ms > 65535){
		time_ms = 65535;
	}
	TIM_SetAutoreload(TIM6, time_ms -1);
	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断
	{
		timeout = 1;
		TIM_Cmd(TIM6, DISABLE);
		TIM_SetCounter(TIM6, 0);
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update); //清除中断标志位
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
}


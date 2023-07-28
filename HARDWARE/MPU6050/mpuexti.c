#include "mpuexti.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "oled.h"
#include "Kalman.h"
#include "sys.h"

#ifdef __MPU6050EXIT

void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	 
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置	
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=0)//一级判定
	{
		if(PEin(2)==0)//二级判定
		{
			EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志位
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//角度
			Pitch += mechanical_error_Pitch;
			Roll  += mechanical_error_Roll;
			kalmanFilter_Roll = kalmanFilter_A(Roll);
			kalmanFilter_Pitch = kalmanFilter_A(Pitch);
			kalmanFilter_Roll = Roll;
			kalmanFilter_Pitch = Pitch;
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度
		}
	}
}
#endif

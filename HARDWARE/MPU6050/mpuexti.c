#include "mpuexti.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "oled.h"
#include "Kalman.h"
#include "sys.h"



//void MPU6050_EXTI_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
//	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2

//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
//	EXTI_Init(&EXTI_InitStructure);//����
//	 
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);//����	
//}

//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2)!=0)//һ���ж�
//	{
//		if(PEin(2)==0)//�����ж�
//		{
//			EXTI_ClearITPendingBit(EXTI_Line2);//����жϱ�־λ
//			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//�Ƕ�
//			Pitch += mechanical_error_Pitch;
//			Roll  += mechanical_error_Roll;
//			kalmanFilter_Roll = kalmanFilter_A(Roll);
//			kalmanFilter_Pitch = kalmanFilter_A(Pitch);
//			kalmanFilter_Roll = Roll;
//			kalmanFilter_Pitch = Pitch;
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
//		}
//	}
//}

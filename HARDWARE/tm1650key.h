#ifndef __TM1650KEY_H
#define __TM1650KEY_H

#include "sys.h"
								 
//IO��������
#define TM1650_SDA_IN()  {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=0<<10*2;}	//PG10����ģʽ
#define TM1650_SDA_OUT() {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=1<<10*2;} 	//PG10���ģʽ
//IO��������	 
#define TM1650_IIC_SCL    PGout(9) //SCL
#define TM1650_IIC_SDA    PGout(10) //SDA	 
#define TM1650_READ_SDA   PGin(10)  //����SDA 

//IIC���в�������
void TM1650_IIC_Init(void);               		//��ʼ��IIC��IO��				 
void TM1650_IIC_Start(void);					//����IIC��ʼ�ź�
void TM1650_IIC_Stop(void);	  					//����IICֹͣ�ź�
void TM1650_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t TM1650_IIC_Read_Byte(uint8_t ack);		//IIC��ȡһ���ֽ�
uint8_t TM1650_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void TM1650_IIC_Ack(void);						//IIC����ACK�ź�
void TM1650_IIC_NAck(void);						//IIC������ACK�ź�
void TM1650_EXTI_Init(void);
void TM1650_key_Init(void);
void TM1650_Init(void);
uint8_t TM1650_Read_KEY(void);
int8_t TM1650_Gte_KEY(void);

#endif

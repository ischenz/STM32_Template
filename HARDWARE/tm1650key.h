#ifndef __TM1650KEY_H
#define __TM1650KEY_H

#include "sys.h"
								 
//IO方向设置
#define TM1650_SDA_IN()  {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=0<<10*2;}	//PG10输入模式
#define TM1650_SDA_OUT() {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=1<<10*2;} 	//PG10输出模式
//IO操作函数	 
#define TM1650_IIC_SCL    PGout(9) //SCL
#define TM1650_IIC_SDA    PGout(10) //SDA	 
#define TM1650_READ_SDA   PGin(10)  //输入SDA 

//IIC所有操作函数
void TM1650_IIC_Init(void);               		//初始化IIC的IO口				 
void TM1650_IIC_Start(void);					//发送IIC开始信号
void TM1650_IIC_Stop(void);	  					//发送IIC停止信号
void TM1650_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t TM1650_IIC_Read_Byte(uint8_t ack);		//IIC读取一个字节
uint8_t TM1650_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void TM1650_IIC_Ack(void);						//IIC发送ACK信号
void TM1650_IIC_NAck(void);						//IIC不发送ACK信号
void TM1650_EXTI_Init(void);
void TM1650_key_Init(void);
void TM1650_Init(void);
uint8_t TM1650_Read_KEY(void);
int8_t TM1650_Gte_KEY(void);

#endif

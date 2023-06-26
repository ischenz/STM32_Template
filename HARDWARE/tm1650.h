 
#ifndef __TM1650_H
#define __TM1650_H
#include "sys.h"
#include "usart.h"
 
 
/**********************************
TM1650芯片控制20路灯函数
PB7为SCL口
PB6为SDA口
**********************************/
 
#define KP_TM1650				 PBin(13)
#define SCL_TM1650               PBout(10) 
#define SDA_TM1650               PBout(11)
#define SDAM_TM1650              PBin(11)
#define SET_SCL_OUT_TM1650()    {GPIOB->MODER&=~(3<<(10*2));GPIOB->MODER|=1<<10*2;}
#define SET_SDA_OUT_TM1650()    {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;}
#define SET_SDA_IN_TM1650()     {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}
 
void IIC_Init_TM1650(void);
void I2C_Start_TM1650(void);
void I2C_Stop_TM1650(void);
void IIC_Ack_TM1650(void);
void IIC_NAck_TM1650(void);
uint8_t IIC_Wait_Ack_TM1650(void);
void IIC_WrByte_TM1650(uint8_t txd);
 
uint8_t Set_Scan_Key(void);
void TM1650_Set(uint8_t add,uint8_t dat);
void Init_Tm1650(void);
 
#endif


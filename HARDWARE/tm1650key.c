#include "tm1650key.h"
#include "delay.h"
#include "usart.h"

uint8_t tm1650_key=99;

//初始化IIC
void TM1650_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOB时钟
	//GPIOG9,G10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

	TM1650_IIC_SCL=1;
	TM1650_IIC_SDA=1;
}

void TM1650_EXTI_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOE2

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);//PG11 连接到中断线11

	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	//外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void TM1650_IIC_Start(void)
{
	TM1650_SDA_OUT();     //sda线输出
	TM1650_IIC_SDA=1;	  	  
	TM1650_IIC_SCL=1;
	delay_us(4);
 	TM1650_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	TM1650_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void TM1650_IIC_Stop(void)
{
	TM1650_SDA_OUT();//sda线输出
	TM1650_IIC_SCL=0;
	TM1650_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	TM1650_IIC_SCL=1; 
	TM1650_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);					   	
}

uint8_t TM1650_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	TM1650_SDA_IN();      //SDA设置为输入  
	TM1650_IIC_SDA=1;delay_us(1);	   
	TM1650_IIC_SCL=1;delay_us(1);	 
	while(TM1650_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			TM1650_IIC_Stop();
			return 1;
		}
	}
	TM1650_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

void TM1650_IIC_Ack(void)
{
	TM1650_IIC_SCL=0;
	TM1650_SDA_OUT();
	TM1650_IIC_SDA=0;
	delay_us(4);
	TM1650_IIC_SCL=1;
	delay_us(4);
	TM1650_IIC_SCL=0;
}
	    
void TM1650_IIC_NAck(void)
{
	TM1650_IIC_SCL=0;
	TM1650_SDA_OUT();
	TM1650_IIC_SDA=1;
	delay_us(4);
	TM1650_IIC_SCL=1;
	delay_us(4);
	TM1650_IIC_SCL=0;
}

void TM1650_IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;   
	TM1650_SDA_OUT(); 	    
	TM1650_IIC_SCL=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		TM1650_IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(4);   
		TM1650_IIC_SCL=1;
		delay_us(4); 
		TM1650_IIC_SCL=0;	
		delay_us(4);
	}	 
}

uint8_t TM1650_IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	TM1650_SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		TM1650_IIC_SCL=0; 
		delay_us(4);
		TM1650_IIC_SCL=1;
		receive<<=1;
		if(TM1650_READ_SDA)receive++;   
		delay_us(4); 
	}					 
   if(!ack)
       TM1650_IIC_NAck();//发送nACK
   else
       TM1650_IIC_Ack(); //发送ACK   
   return receive;
}

uint8_t TM1650_Read_KEY(void)
{
	uint8_t temp;
	TM1650_IIC_Start();
	TM1650_IIC_Send_Byte(0x49);//发送读按键指令
	TM1650_IIC_Wait_Ack();
	temp=TM1650_IIC_Read_Byte(0);
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Stop();
	return temp;
}

/****返回-1 错误****/
int8_t TM1650_Gte_KEY(void)
{	
	uint8_t key;
	int8_t key_name=-1;
	key=TM1650_Read_KEY();
	
	switch(key){
		case 0x44:key_name =  1;break;
		case 0x4C:key_name =  2;break;
		case 0x54:key_name =  3;break;
		case 0x5C:key_name = 10;break;
		case 0x45:key_name =  4;break;
		case 0x4D:key_name =  5;break;
		case 0x55:key_name =  6;break;
		case 0x5D:key_name = 11;break;
		case 0x46:key_name =  7;break;
		case 0x4E:key_name =  8;break;
		case 0x56:key_name =  9;break;
		case 0x5E:key_name = 12;break;
		case 0x47:key_name = 13;break;//*
		case 0x4F:key_name =  0;break;
		case 0x57:key_name = 14;break;//.
		case 0x5F:key_name = 15;break;//OK
		default  :key_name = -1;
	}
	return key_name;
}


void TM1650_Init(void)
{
	TM1650_IIC_Start();
	TM1650_IIC_Send_Byte(0x48);//发送工作模式配置指令
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Send_Byte(0x19);//发送键扫模式代码0x19
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Stop();
}

void TM1650_key_Init(void)
{
	TM1650_IIC_Init();
	TM1650_EXTI_Init();
	TM1650_Init();
}



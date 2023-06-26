#include "tm1650key.h"
#include "delay.h"
#include "usart.h"

uint8_t tm1650_key=99;

//��ʼ��IIC
void TM1650_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOBʱ��
	//GPIOG9,G10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��

	TM1650_IIC_SCL=1;
	TM1650_IIC_SDA=1;
}

void TM1650_EXTI_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOE2

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);//PG11 ���ӵ��ж���11

	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	//�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

void TM1650_IIC_Start(void)
{
	TM1650_SDA_OUT();     //sda�����
	TM1650_IIC_SDA=1;	  	  
	TM1650_IIC_SCL=1;
	delay_us(4);
 	TM1650_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	TM1650_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

void TM1650_IIC_Stop(void)
{
	TM1650_SDA_OUT();//sda�����
	TM1650_IIC_SCL=0;
	TM1650_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	TM1650_IIC_SCL=1; 
	TM1650_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);					   	
}

uint8_t TM1650_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	TM1650_SDA_IN();      //SDA����Ϊ����  
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
	TM1650_IIC_SCL=0;//ʱ�����0 	   
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
	TM1650_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
	TM1650_SDA_IN();//SDA����Ϊ����
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
       TM1650_IIC_NAck();//����nACK
   else
       TM1650_IIC_Ack(); //����ACK   
   return receive;
}

uint8_t TM1650_Read_KEY(void)
{
	uint8_t temp;
	TM1650_IIC_Start();
	TM1650_IIC_Send_Byte(0x49);//���Ͷ�����ָ��
	TM1650_IIC_Wait_Ack();
	temp=TM1650_IIC_Read_Byte(0);
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Stop();
	return temp;
}

/****����-1 ����****/
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
	TM1650_IIC_Send_Byte(0x48);//���͹���ģʽ����ָ��
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Send_Byte(0x19);//���ͼ�ɨģʽ����0x19
	TM1650_IIC_Wait_Ack();
	TM1650_IIC_Stop();
}

void TM1650_key_Init(void)
{
	TM1650_IIC_Init();
	TM1650_EXTI_Init();
	TM1650_Init();
}



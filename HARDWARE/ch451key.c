#include "ch451key.h"
/************
64 65 66 67
72 73 74 75
80 81 82 83
88 89 90 91
************/

/************
1  2  3  12
4  5  6  13
7  8  9  14
10 0  11 15
************/
uint8_t Ch451Key_Num = 99 ,Flag_Ch45KeyRead = 0;

void Ch451Key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	Ch451KEY_DIN_Clr();
	Ch451KEY_DIN_Set();
	Ch451KEY_LOAD_Set();

	ch451_write(0x0403);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOE2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource12);//PG12 ���ӵ��ж���12

	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	//�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

void ch451_write(uint16_t command)
{
  uint8_t i;
  CH451KEY_LOAD_Clr();                                  //���ʼ       
  for(i=0;i<12;i++){                             //����12λ���ݣ���λ��ǰ
		if(command&0x0001){
			Ch451KEY_DIN_Set();
		}
		else{
			Ch451KEY_DIN_Clr();
    }				
		Ch451KEY_DCLK_Clr();
		command>>=1;
		Ch451KEY_DCLK_Set();                             //��������Ч
  }
  Ch451KEY_LOAD_Set();                                 //��������
}


/************
64 65 66 67
72 73 74 75
80 81 82 83
88 89 90 91
************/
static void Get_ch451Key_Value(uint8_t * KeyNum)
{
	switch(*KeyNum)
	{
		case 64:*KeyNum = 1;break;
		case 65:*KeyNum = 2;break;
		case 66:*KeyNum = 3;break;
		case 72:*KeyNum = 4;break;
		case 73:*KeyNum = 5;break;
		case 74:*KeyNum = 6;break;
		case 80:*KeyNum = 7;break;
		case 81:*KeyNum = 8;break;
		case 82:*KeyNum = 9;break;
		case 89:*KeyNum = 0;break;
		case 88:*KeyNum = 10;break;
		case 90:*KeyNum = 11;break;
		case 67:*KeyNum = 12;break;
		case 75:*KeyNum = 13;break;
		case 83:*KeyNum = 14;break;
		case 91:*KeyNum = 15;break;
		default:*KeyNum = 99;break;
	}
}

void EXTI15_10_IRQHandler(void)
{
		if(PGin(12)==0)//�����ж�
		{
			uint8_t i,keyvalue = 0;
			uint8_t command = 0x07;
			CH451KEY_LOAD_Clr();
			for(i=0;i<4;i++){
					if(command&0x01){
							Ch451KEY_DIN_Set();
					}
					else{
							Ch451KEY_DIN_Clr();
					}
					Ch451KEY_DCLK_Clr();
					command>>=1;
					Ch451KEY_DCLK_Set();
			}
			Ch451KEY_LOAD_Set();
			for(i=0;i<7;i++){
					keyvalue<<=1;
					keyvalue |= Ch451KEY_DOUT();
					Ch451KEY_DCLK_Clr();
					Ch451KEY_DCLK_Set();
			}
			Ch451Key_Num = keyvalue;
			Flag_Ch45KeyRead = 1;
			Get_ch451Key_Value(&Ch451Key_Num);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־λ
}


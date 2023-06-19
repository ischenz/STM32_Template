#include "ch451key.h"
#include "oled.h"
#include "delay.h"
#include "math.h"
/************
64 65 66 67
72 73 74 75
80 81 82 83
88 89 90 91
************/

uint8_t Ch451Key_Num = 99 ,Flag_Ch45KeyRead = 0;

void Ch451Key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	Ch451KEY_DIN_Clr();
	Ch451KEY_DIN_Set();
	Ch451KEY_LOAD_Set();

	ch451_write(0x0403);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOE2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource12);//PG12 连接到中断线12

	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	//外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void ch451_write(uint16_t command)
{
  uint8_t i;
  CH451KEY_LOAD_Clr();                                  //命令开始       
  for(i=0;i<12;i++){                             //送入12位数据，低位在前
		if(command&0x0001){
			Ch451KEY_DIN_Set();
		}
		else{
			Ch451KEY_DIN_Clr();
    }				
		Ch451KEY_DCLK_Clr();
		command>>=1;
		Ch451KEY_DCLK_Set();                             //上升沿有效
  }
  Ch451KEY_LOAD_Set();                                 //加载数据
}


/************
64 65 66 67
72 73 74 75
80 81 82 83
88 89 90 91
************/

void Get_ch451_value(int *value)
{
	static uint8_t Flag_SetAngle = 0;
	if(Flag_SetAngle){
		uint8_t num[10] = {0},i = 0,j;
		OLED_Clear();
		OLED_ShowString(0,0,"Please input value",8,1);
		OLED_ShowString(0,10,"End with a '*' key",8,1);
		OLED_Refresh();
		while(1){
		  if(Flag_Ch45KeyRead == 1){
				Flag_Ch45KeyRead = 0;
				if(Ch451Key_Num == 13) break;//'*' == 13
			    if(Ch451Key_Num > 10){
					OLED_ShowString(0,30,"Err",8,1);
					OLED_Refresh();
					delay_ms(2000);
					break;
				}
				num[i++] = Ch451Key_Num;
				OLED_ShowNum(i*6,20,Ch451Key_Num,1,8,1);
				OLED_Refresh();
			}				
		}
		*value=0;
		for(j=0;j<i;j++){//		                    5		 4	 	 3		   2		1
			*value=*value*10+num[j];// 1 2 3 4 5  a[4]*1 + a[3]*10 + a[2]*100 + a[1]*1000 + a[0]*10000
		}//						                   10^0		 10^1	 10^2		  10^3		10^4
		//printf("num:%d\r\n",ch451_value);
	}
	/*到此处已得到数据 保存在“ch451_value”中 */
	else{
		if(Flag_Ch45KeyRead == 1){
			Flag_Ch45KeyRead = 0;
			if(Ch451Key_Num==10){
					Flag_SetAngle = 1;
			}
		}
	}
}

int8_t InputValue(float *value)
{
	static uint8_t InputFlag, frstInFlag, i;
	static uint8_t num[10] = {0};
	static uint8_t intNum;
	int8_t j;
	float input;
	if(frstInFlag == 0){
		frstInFlag = 1;
		OLED_Clear();
		OLED_ShowString(0,0,"Please input value",8,1);
		OLED_ShowString(0,10,"End with a '*' key",8,1);
		OLED_Refresh();
	}
	if(InputFlag == 0){
		if(Flag_Ch45KeyRead == 1){
			Flag_Ch45KeyRead = 0;
			
			if(Ch451Key_Num >= 10){
				if(Ch451Key_Num == 13)//*  输入结束
				{
					InputFlag = 1;
					if(intNum == 0){//没有小数
						for(j = 0;j < i; j++){
							input += pow(10, i-j-1)*num[j];
						}
					}else{//有小数
						/* 整数部分 */
						for(j = 0;j < intNum; j++){
							input += pow(10, intNum-j-1)*num[j];
						}
						/* 小数部分 */
						for(j = intNum+1; j < i; j++){
							input += pow(10, -(j-intNum))*num[j];
						}
					}
					*value = input;
					
					return 1;
				}else if(Ch451Key_Num == 14){//.
					intNum = i;
					num[i++] = '.';
					OLED_ShowChar(i*6,20,num[i-1],8,1);
					OLED_Refresh();
					return 0;
				}else{
					return 0;
				}
			}
			num[i++] = Ch451Key_Num;
			OLED_ShowNum(i*6,20,Ch451Key_Num,1,8,1);
			OLED_Refresh();
		}
	}
	if(Flag_Ch45KeyRead == 1){
		Flag_Ch45KeyRead = 0;
		if(Ch451Key_Num == 10){
			InputFlag = frstInFlag = i = 0;
		}
	}
	return 0;
}

int8_t GetValueFromKeyvalue(uint8_t keyvalue)
{
	int8_t keynum = -1;
	switch(keyvalue){
		case KeyNum0 :return 1 ;
		case KeyNum1 :return 2 ;
		case KeyNum2 :return 3 ;
		case KeyNum3 :return 10;
		case KeyNum4 :return 4 ;
		case KeyNum5 :return 5 ;
		case KeyNum6 :return 6 ;
		case KeyNum7 :return 11;
		case KeyNum8 :return 7 ;
		case KeyNum9 :return 8 ;
		case KeyNum10:return 9 ;
		case KeyNum11:return 12;
		case KeyNum12:return 13;//*
		case KeyNum13:return 0 ;
		case KeyNum14:return 14;//.
		case KeyNum15:return 15;//OK
		default      :return -1;
	}
	return keynum;
}

void EXTI15_10_IRQHandler(void)
{
	if(PGin(12)==0)//二级判定
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
		Ch451Key_Num = GetValueFromKeyvalue(keyvalue);
		Flag_Ch45KeyRead = 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line12);//清除中断标志位
}


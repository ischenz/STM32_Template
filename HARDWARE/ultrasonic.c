#include "ultrasonic.h"
#include "usart.h"

#ifdef USART_ULTRASONIC
void ultrasonic_init(void)
{
	uart3_init(9600);
    USART_SendData(ULTRASONIC_USART, 0x55);
}

uint16_t get_distance(void)
{
	uint16_t mm = 0;
	static uint16_t i;
	i++;
	if(ULTRASONIC_RINGBUFF.Lenght == 2){
		i=0;
		uint8_t data = 0;
		Read_RingBuff(&ULTRASONIC_RINGBUFF, &data);
		mm = data;
		Read_RingBuff(&ULTRASONIC_RINGBUFF, &data);
		mm =  mm << 8 | data;
		if (mm < 20 || mm > 4500)
		{
			mm = 0;
		}
		//delay_us(1000);
		USART_SendData(ULTRASONIC_USART, 0x55);
	} 
	if(i ==10000 || ULTRASONIC_RINGBUFF.Lenght > 2){ //超时重发
		RingBuff_Init(&ULTRASONIC_RINGBUFF);
		USART_SendData(ULTRASONIC_USART, 0x55);
	}
	return mm;
}

#else

float ultrasound_data[ULTRASOUND_DIS_INDEX] = {0};//数据存储
uint16_t d_count = 0;//获取数据次数计数变量
uint8_t ultrasound_flag = 0;//获取完成标志位变量

static void Ultrasound_GPIO_Conf(void)//端口配置
{
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_AHB1PeriphClockCmd(ULTRASOUND_GPIO_CLK, ENABLE);                //打开GPIO时钟

	GPIO_InitStructure.GPIO_Pin = ULTRASOUND_GPIO_ECHO_PIN;             //ECHOG端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                        //普通输入模式    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                       
	GPIO_Init(ULTRASOUND_GPIO_PORT, &GPIO_InitStructure);               
    GPIO_ResetBits(ULTRASOUND_GPIO_PORT, ULTRASOUND_GPIO_ECHO_PIN);     //拉低ECHO

    GPIO_InitStructure.GPIO_Pin = ULTRASOUND_GPIO_TRIG_PIN;             //TRIG端口配置
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	                    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;                     //下拉
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //推挽输出
    GPIO_Init(ULTRASOUND_GPIO_PORT, &GPIO_InitStructure);               
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
    SYSCFG_EXTILineConfig(ULTRASOUND_GPIOSourceGPIO, ULTRASOUND_PINSOURCE);       //连接到中断线

    EXTI_InitStructure.EXTI_Line = ULTRASOUND_EXTI_LINE;                //配置中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;      //上下边沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = ULTRASOUND_EXTI_NVIC_IRQChannel;//配置中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

static void Ultrasound_TIM_Conf(void)//定时器配置
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	ULTRASOUND_TIM_CLK_ENABLE;//打开ULTRASOUND_TIM时钟
	
	//计算分频系数（每计一次为1us）
	TIM_TimeBaseStruct.TIM_Period = 0xFDE8;//重载寄存器的值，定时周期64ms
	TIM_TimeBaseStruct.TIM_Prescaler = 168-1;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟切割
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(ULTRASOUND_TIM,&TIM_TimeBaseStruct);//初始化ULTRASOUND_TIM
	
	TIM_ClearITPendingBit(ULTRASOUND_TIM,TIM_IT_Update);//清除中断标志位，配置时必须加上
	TIM_ITConfig(ULTRASOUND_TIM, TIM_IT_Update, ENABLE);//事件更新中断配置
	
	NVIC_InitStructure.NVIC_IRQChannel = ULTRASOUND_TIM_IRQ; //配置ULTRASOUND_TIM中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	TIM_Cmd(ULTRASOUND_TIM,DISABLE);	
}

void Ultrasound_Init(void)//超声波模块初始化
{
	Ultrasound_GPIO_Conf();//超声波控制端口配置
	Ultrasound_TIM_Conf();//超声波控制计数器配置
}

void Ultrasound_start(void)//超声波模块启动
{
	GPIO_SetBits(ULTRASOUND_GPIO_PORT, ULTRASOUND_GPIO_TRIG_PIN);//拉高TRIG
	
	ULTRASOUND_TIM->CNT = 0x0000;	//清空计数器计数寄存器值
	ULTRASOUND_TIM->ARR = 0xffff; //重载值重新设定
	TIM_Cmd(ULTRASOUND_TIM,ENABLE);	//开启计数器
	while(ULTRASOUND_TIM->CNT <= 12);//大于10us,此处等待12us，
	
	GPIO_ResetBits(ULTRASOUND_GPIO_PORT, ULTRASOUND_GPIO_TRIG_PIN);//拉低TRIG
}

float Ultrasound_GetDistance(void)//获取距离数据
{
	uint8_t i,j;//循环变量
	float distance_temp;//排序的临时变量
	for(i = 0;i < ULTRASOUND_DIS_INDEX - 1;i ++ )//降序排列,找出最大值和最小值
	{
		for(j = 0;j < ULTRASOUND_DIS_INDEX - 1;j ++)
		{
			if(ultrasound_data[j] < ultrasound_data[j + 1])
			{
				distance_temp = ultrasound_data[j + 1];
				ultrasound_data[j + 1] = ultrasound_data[j];
				ultrasound_data[j] = distance_temp;
			}
		}	
	}
				
	distance_temp = 0;//清零
		
	for(i = 2; i < ULTRASOUND_DIS_INDEX - 2;i ++)//去掉两个最大值和两个最小值
	{
		distance_temp += ultrasound_data[i];//获取的距离值累加
	}
	
	distance_temp /= (ULTRASOUND_DIS_INDEX - 4);//求平均值
	return distance_temp;
}


void ULTRASOUND_EXTI_IRQHandler(void)	//超声波ECHO引脚所用中断服务函数
{
	if(EXTI_GetITStatus(ULTRASOUND_EXTI_LINE)!=0)	//判断是否真是触发中断
	{
		TIM_ITConfig(ULTRASOUND_TIM, TIM_IT_Update, DISABLE);//关闭计数器中断
		TIM_Cmd(ULTRASOUND_TIM,DISABLE);	//关闭计数器
		if(GPIO_ReadInputDataBit(ULTRASOUND_GPIO_PORT, ULTRASOUND_GPIO_ECHO_PIN) == 1)	//检测到返回信号
		{
			ULTRASOUND_TIM->CNT = 0x0000;	//清空计数器计数寄存器值
			ULTRASOUND_TIM->ARR = 0xffff;//重载值重设为计数最大值
			TIM_Cmd(ULTRASOUND_TIM,ENABLE);	//开启计数器
		}
		
		if(GPIO_ReadInputDataBit(ULTRASOUND_GPIO_PORT, ULTRASOUND_GPIO_ECHO_PIN) == 0 )	//返回信号结束
		{
			if(ULTRASOUND_TIM->ARR == 0xffff) //需要确定计数器当前是不是在计数返回信号时间
			{
				if(d_count == ULTRASOUND_DIS_INDEX) //判断获取数据的次数
				{
					d_count = 0;	//清空次数计数值
					ultrasound_flag = 1;	//置位超声波标志位
				}
				else
				{
					ultrasound_flag = 0;	//清空超声波标志位
					ultrasound_data[d_count] = (float)(ULTRASOUND_TIM->CNT) / 58;	//计算距离，单位cm。并将数据存入待处理数据数组
					d_count++;	//次数计数值自增
				}
			}
            ULTRASOUND_TIM->CNT = 0x00;
			ULTRASOUND_TIM->ARR = 0xFDE8;//重载值为计数64ms
			TIM_ITConfig(ULTRASOUND_TIM, TIM_IT_Update, ENABLE);//开启计数器中断
			TIM_Cmd(ULTRASOUND_TIM,ENABLE);	//开启计数器
		}
		EXTI_ClearITPendingBit(ULTRASOUND_EXTI_LINE);	//清除LINE上的中断标志位
	}
}

void ULTRASOUND_TIM_IRQHandler(void)//超声波所用TIM中断服务函数
{
	if(TIM_GetITStatus(ULTRASOUND_TIM,TIM_IT_Update) == SET)//判断是否有事件更新
	{
		Ultrasound_start();//启动超声波模块
	}
	TIM_ClearITPendingBit(ULTRASOUND_TIM,TIM_IT_Update);//清除中断标志位
}

#endif


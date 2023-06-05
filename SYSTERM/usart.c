#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
RingBuff_t Uart2_RingBuff,Uart1_RingBuff,Uart3_RingBuff;//创建一个ringBuff的缓冲区
uint16_t distance = 0;
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

#endif
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t rdata;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{		
		rdata = USART_ReceiveData(USART1);
		if(Write_RingBuff(&Uart1_RingBuff, rdata) == RINGBUFF_ERR){//缓冲区满灯亮
			LED1=0;
		}else{
			LED1=1;
		}
	} 
} 

void uart2_v831_init(uint32_t bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口
	
	USART_Cmd(USART2, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
}
 
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	uint8_t rdata;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		rdata = USART_ReceiveData(USART2);
		if(Write_RingBuff(&Uart2_RingBuff, rdata) == RINGBUFF_ERR){//缓冲区满灯亮
			LED1=0;
		}else{
			LED1=1;
		}
	} 
} 

void uart3_init(uint32_t bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口
	
	USART_Cmd(USART3, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
}

void USART3_IRQHandler(void)
{
	uint8_t rdata;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		rdata = USART_ReceiveData(USART3);
		if(Write_RingBuff(&Uart3_RingBuff, rdata) == RINGBUFF_ERR){//缓冲区满灯亮
			LED1=0;
		}else{
			LED1=1;
		}
	} 
} 

uint16_t get_distance(void)
{
	uint16_t mm = 0;
	uint8_t cmd = 0x55;
	static uint16_t i;
	i++;
	if(Uart3_RingBuff.Lenght == 2){
		i=0;
		uint8_t data = 0;
		Read_RingBuff(&Uart3_RingBuff, &data);
		mm = data;
		Read_RingBuff(&Uart3_RingBuff, &data);
		mm =  mm << 8 | data;
		//delay_us(1000);
		usart_send(USART3,&cmd,1);
	} 
	if(i==100 || Uart3_RingBuff.Lenght > 2){ //一秒
		RingBuff_Init(&Uart3_RingBuff);
		usart_send(USART3,&cmd,1);
	}
	return mm;
}

uint8_t DataDecode(RingBuff_t *ringbuff, uint8_t *data1, uint8_t *data2)      
{
	static uint8_t uart_dec_count;
	static uint8_t uart_rec_data[5];
	uint8_t ret = 1;
	if(Read_RingBuff(ringbuff, &uart_rec_data[uart_dec_count]) == RINGBUFF_ERR){
		return 1;
	}
	if((uart_dec_count == 0)&&(uart_rec_data[uart_dec_count] != 0x55)) {    		//检测第一个数据是否为0x55
		uart_rec_data[uart_dec_count] = 0;						
	} else if((uart_dec_count == 1)&&(uart_rec_data[uart_dec_count] != 0xaa)){      //检测第二个数据是否为0xaa
		uart_rec_data[uart_dec_count] = 0;
		uart_rec_data[uart_dec_count-1] = 0;
		uart_dec_count = 0;
	} else if((uart_dec_count == 4)&&(uart_rec_data[uart_dec_count] != 0xfa)){
		uart_rec_data[uart_dec_count] = 0;
		uart_rec_data[uart_dec_count-1] = 0;
		uart_rec_data[uart_dec_count-2] = 0;
		uart_rec_data[uart_dec_count-3] = 0;
		uart_rec_data[uart_dec_count-4] = 0;
		uart_dec_count = 0;
	} else {
		if(uart_dec_count == 4)//成功接收一帧数据
		{
			*data1 = uart_rec_data[2];
			*data2 = uart_rec_data[3];
			ret = 0;
		}
		uart_dec_count ++;
		if(uart_dec_count == 5)
		{
			uart_dec_count = 0;
		}
	}
	return ret;	
} 

void usart_send(USART_TypeDef* USARTx, uint8_t*data, uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET); 
		USART_SendData(USARTx,data[i]);
	}
}

/**
* @brief  RingBuff_Init
* @param  RingBuff_t *ringbuff
* @return void
* @note   初始化环形缓冲区
*/
void RingBuff_Init(RingBuff_t *ringbuff)
{
  //初始化相关信息
  ringbuff->Head = 0;
  ringbuff->Tail = 0;
  ringbuff->Lenght = 0;
}

/**
* @brief  Write_RingBuff
* @param  uint8_t data
* @return FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
* @note   往环形缓冲区写入uint8_t类型的数据
*/
uint8_t Write_RingBuff(RingBuff_t *ringbuff, uint8_t data)
{
  if(ringbuff->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
  {
    return RINGBUFF_ERR;
  }
  ringbuff->Ring_data[ringbuff->Tail]=data;
  ringbuff->Tail = (ringbuff->Tail+1)%RINGBUFF_LEN;//防止越界非法访问
  ringbuff->Lenght++;
  return RINGBUFF_OK;
}

/**
* @brief  Read_RingBuff
* @param  uint8_t *rData，用于保存读取的数据
* @return FLASE:环形缓冲区没有数据，读取失败;TRUE:读取成功
* @note   从环形缓冲区读取一个u8类型的数据
*/
uint8_t Read_RingBuff(RingBuff_t *ringbuff, uint8_t *rData)
{
  if(ringbuff->Lenght == 0)//判断非空
  {
    return RINGBUFF_ERR;
  }
  *rData = ringbuff->Ring_data[ringbuff->Head];//先进先出FIFO，从缓冲区头出
  ringbuff->Head = (ringbuff->Head+1)%RINGBUFF_LEN;//防止越界非法访问
  ringbuff->Lenght--;
  return RINGBUFF_OK;
}

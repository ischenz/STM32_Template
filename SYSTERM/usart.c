#include "sys.h"
#include "usart.h"	
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
RingBuff_t Uart2_RingBuff;//����һ��ringBuff�Ļ�����
RingBuff_t Uart1_RingBuff;//����һ��ringBuff�Ļ�����

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

#endif
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t rdata;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{		
		rdata = USART_ReceiveData(USART1);
		if(Write_RingBuff(&Uart1_RingBuff, rdata) == RINGBUFF_ERR){//������������
			LED1=0;
		}else{
			LED1=1;
		}
	} 
} 

void uart2_v831_init(uint32_t bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}
 
void USART2_IRQHandler(void)                	//����2�жϷ������
{
	uint8_t rdata;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		rdata = USART_ReceiveData(USART2);
		if(Write_RingBuff(&Uart2_RingBuff, rdata) == RINGBUFF_ERR){//������������
			LED1=0;
		}else{
			LED1=1;
		}
	} 
} 

uint8_t DataDecode(RingBuff_t *ringbuff, uint8_t *data1, uint8_t *data2)      
{
	static uint8_t uart_dec_count;
	static uint8_t uart_rec_data[5];
	uint8_t ret = 1;
	if(Read_RingBuff(ringbuff, &uart_rec_data[uart_dec_count]) == RINGBUFF_ERR){
		return 1;
	}
	if((uart_dec_count == 0)&&(uart_rec_data[uart_dec_count] != 0x55)) {    		//����һ�������Ƿ�Ϊ0x55
		uart_rec_data[uart_dec_count] = 0;						
	} else if((uart_dec_count == 1)&&(uart_rec_data[uart_dec_count] != 0xaa)){      //���ڶ��������Ƿ�Ϊ0xaa
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
		if(uart_dec_count == 4)//�ɹ�����һ֡����
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
* @note   ��ʼ�����λ�����
*/
void RingBuff_Init(RingBuff_t *ringbuff)
{
  //��ʼ�������Ϣ
  ringbuff->Head = 0;
  ringbuff->Tail = 0;
  ringbuff->Lenght = 0;
}

/**
* @brief  Write_RingBuff
* @param  uint8_t data
* @return FLASE:���λ�����������д��ʧ��;TRUE:д��ɹ�
* @note   �����λ�����д��uint8_t���͵�����
*/
uint8_t Write_RingBuff(RingBuff_t *ringbuff, uint8_t data)
{
  if(ringbuff->Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
  {
    return RINGBUFF_ERR;
  }
  ringbuff->Ring_data[ringbuff->Tail]=data;
  ringbuff->Tail = (ringbuff->Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringbuff->Lenght++;
  return RINGBUFF_OK;
}

/**
* @brief  Read_RingBuff
* @param  uint8_t *rData�����ڱ����ȡ������
* @return FLASE:���λ�����û�����ݣ���ȡʧ��;TRUE:��ȡ�ɹ�
* @note   �ӻ��λ�������ȡһ��u8���͵�����
*/
uint8_t Read_RingBuff(RingBuff_t *ringbuff, uint8_t *rData)
{
  if(ringbuff->Lenght == 0)//�жϷǿ�
  {
    return RINGBUFF_ERR;
  }
  *rData = ringbuff->Ring_data[ringbuff->Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
  ringbuff->Head = (ringbuff->Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringbuff->Lenght--;
  return RINGBUFF_OK;
}

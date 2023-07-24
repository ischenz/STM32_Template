#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
#define  RINGBUFF_LEN          (100)     //定义最大接收字节数
#define  RINGBUFF_OK           1     
#define  RINGBUFF_ERR          0   

typedef struct
{
    uint16_t Head;           
    uint16_t Tail;
    uint16_t Lenght;
    uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;

extern uint8_t RecCoorFlag;
extern RingBuff_t Uart2_RingBuff,Uart1_RingBuff,Uart3_RingBuff;
extern uint16_t distance;

void uart_init(u32 bound);
void uart2_v831_init(uint32_t bound);
void uart3_init(uint32_t bound);
void usart_send(USART_TypeDef* USARTx, uint8_t*data, uint8_t len);

void RingBuff_Init(RingBuff_t *ringbuff);
uint8_t Write_RingBuff(RingBuff_t *ringbuff, uint8_t data);
uint8_t Read_RingBuff(RingBuff_t *ringbuff, uint8_t *rData);
uint8_t DataDecode(RingBuff_t *ringbuff, uint8_t *data1, uint8_t *data2);
int8_t receiveJson(RingBuff_t *ringbuff, char *str);

#endif



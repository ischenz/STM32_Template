#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	


/*大越创新STM32F4XX开发板OLD-TFT液晶屏接口*/
//#define __Front	
#define __Back


/**======================================================
	*====================================================
	*             Front接法OLED端口							
	*----------------------------------------------------
	*	 OLED接口	|  单片机接口		|	TFT液晶屏接口
	*----------------------------------------------------
	*		GND		|		GND			|		30
	*----------------------------------------------------
	*		VCC		|		PB15		|		28
	*----------------------------------------------------
	*	  CLK(D0)	|		PB2			|		26
	*----------------------------------------------------
	*		SDA		|		PC13		|		24
	*----------------------------------------------------
	*		RES		|		PG12		|		22
	*----------------------------------------------------
	*		DC		|		PD5			|		20
	*----------------------------------------------------
	*		CS		|		PD14		|		18
	*----------------------------------------------------
	*====================================================
	*====================================================
	*             Back接法OLED端口							
	*----------------------------------------------------
	*	 OLED接口	|  单片机接口		|	TFT液晶屏接口
	*----------------------------------------------------
	*		GND		|		GND			|		1
	*----------------------------------------------------
	*		VCC		|		PD10		|		3
	*----------------------------------------------------
	*	  CLK(D0)	|		PD8			|		5
	*----------------------------------------------------
	*		SDA		|		PE14		|		7
	*----------------------------------------------------
	*		RES		|		PE12		|		9
	*----------------------------------------------------
	*		DC		|		PE10		|		11
	*----------------------------------------------------
	*		CS		|		PE8			|		13
	*----------------------------------------------------
	*====================================================
	*====================================================
	*****************************************************
	*/


/***********************************************************************
							Front接线法
***********************************************************************/
#ifdef  __Front 

#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)//PB15 vcc

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_2)	//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_13)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_12)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOG,GPIO_Pin_12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_5)	//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_5)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_14)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_14)
																												
#endif
/*------------------------------ endif ------------------------------------*/



/****************************************************************************
							Back接线法
*****************************************************************************/
#ifdef __Back

#define LED_ON GPIO_SetBits(GPIOD,GPIO_Pin_10)
#define LED_OFF GPIO_ResetBits(GPIOD,GPIO_Pin_10)//PD10 vcc

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_8)	//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOD,GPIO_Pin_8)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_14)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOE,GPIO_Pin_14)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_12)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOE,GPIO_Pin_12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_10)	//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_10)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_8)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_8)
																												

#endif
/*------------------------------- endif ------------------------------------*/


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,int8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowSNum(uint8_t x,uint8_t y,int num,uint8_t len,uint8_t size1,uint8_t mode);//添加显示负数功能
void OLED_ShowFNum(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size1,uint8_t mode);//显示带一位小数的数字
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(void);

#endif


/*****************************************************************************
* | File      	:   LCD_1IN47.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master 
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-16
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __LCD_1IN47_H
#define __LCD_1IN47_H	

#include "image.h"

#include <stdint.h>
#include <stdlib.h>		//itoa()
#include <stdio.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/
#define SCL_GPIO_Port GPIOB
#define SDA_GPIO_Port GPIOC
#define RST_GPIO_Port GPIOG
#define DC_GPIO_Port  GPIOD
#define CS_GPIO_Port  GPIOD

#define SCL_Pin GPIO_Pin_2
#define SDA_Pin GPIO_Pin_13
#define RST_Pin GPIO_Pin_12
#define DC_Pin  GPIO_Pin_5
#define CS_Pin  GPIO_Pin_14

#define DEV_SCL_PIN     SCL_GPIO_Port,SCL_Pin
#define DEV_SDA_PIN     SDA_GPIO_Port,SDA_Pin
#define DEV_RST_PIN     RST_GPIO_Port,RST_Pin	
#define DEV_DC_PIN      DC_GPIO_Port,DC_Pin		
#define DEV_CS_PIN		CS_GPIO_Port,CS_Pin		
#define DEV_BL_PIN		TIM3->CCR2 				

/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_pin, _value) 	GPIO_WriteBit(_pin, _value == 0? (BitAction)0:(BitAction)1)
#define DEV_Digital_Read(_pin) 				GPIO_ReadBit(_pin)

/**
 * SPI
**/
#define DEV_SPI_WRITE(_dat)  DEV_SPI_WRite(_dat);


/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms) delay_ms(__xms)

/**
 * PWM_BL
**/
#define DEV_Set_PWM(_Value)     DEV_BL_PIN= _Value

#define LCD_1IN47_HEIGHT 172
#define LCD_1IN47_WIDTH 320

#define HORIZONTAL 0
#define VERTICAL   1

#define LCD_1IN47_CS_0	DEV_Digital_Write(DEV_CS_PIN, 0)	 
#define LCD_1IN47_CS_1	DEV_Digital_Write(DEV_CS_PIN, 1)	
	                 
#define LCD_1IN47_RST_0	DEV_Digital_Write(DEV_RST_PIN, 0)
#define LCD_1IN47_RST_1	DEV_Digital_Write(DEV_RST_PIN, 1)	
	                  
#define LCD_1IN47_DC_0	DEV_Digital_Write(DEV_DC_PIN, 0)
#define LCD_1IN47_DC_1	DEV_Digital_Write(DEV_DC_PIN, 1)	
/*-----------------------------------------------------------------------------*/

typedef struct{
	UWORD WIDTH;
	UWORD HEIGHT;
	UBYTE SCAN_DIR;
}LCD_1IN47_ATTRIBUTES;
extern LCD_1IN47_ATTRIBUTES LCD_1IN47;

/********************************************************************************
********************************************************************************/
void DEV_SPI_WRite(UBYTE _dat);
int LCD_1IN47_Module_Init(void);
void LCD_1IN47_Exit(void);

void LCD_1IN47_Init(UBYTE Scan_dir);
void LCD_1IN47_Clear(UWORD Color);
void LCD_1IN47_Display(UWORD *Image);
void LCD_1IN47_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image);
void LCD_1IN47_DrawPoint(UWORD X, UWORD Y, UWORD Color);
void LCD_1IN47_SetBackLight(UWORD Value);
#endif

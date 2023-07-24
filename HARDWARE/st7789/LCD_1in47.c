/*****************************************************************************
    LCD_1IN47_Module_Init();

    Paint_DrawString_EN(30, 10, "123",        &Font24,  YELLOW, RED);  
    Paint_DrawString_EN(30, 34, "ABC",        &Font24,  BLUE,   CYAN);
    Paint_DrawFloatNum (30, 58 ,987.654321,3, &Font12,  WHITE,  BLACK);
    Paint_DrawString_CN(30,130, "微雪电子",   &Font24CN,WHITE,  BLUE);
    Paint_DrawImage(gImage_1,30,70,60,60);

    Paint_DrawRectangle(185, 10, 285, 58, RED     ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawLine  (185, 10, 285, 58,    MAGENTA ,DOT_PIXEL_2X2,LINE_STYLE_SOLID);
    Paint_DrawLine  (285, 10, 185, 58,    MAGENTA ,DOT_PIXEL_2X2,LINE_STYLE_SOLID);
    Paint_DrawCircle(210,100,  25,        BLUE    ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawCircle(240,100,  25,        BLACK   ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawCircle(280,100,  25,        RED     ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawCircle(235,125,  25,        YELLOW  ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawCircle(255,125,  25,        GREEN   ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //DEV_Delay_ms(3000);

    printf("quit...\r\n");
    DEV_Module_Exit();
 ******************************************************************************/
#include "LCD_1in47.h"
#include "GUI_Paint.h"
#include "delay.h"
#include <stdlib.h> //itoa()
#include <stdio.h>

LCD_1IN47_ATTRIBUTES LCD_1IN47;

/********************************************************************************
function:	Delay function
note:
	Driver_Delay_ms(xms) : Delay x ms
********************************************************************************/
void DEV_delay_ms(uint16_t xms )
{
	delay_ms(xms);
}

void DEV_SPI_WRite(UBYTE _dat)
{
	//HAL_SPI_Transmit(&hspi1, (uint8_t *)&_dat, 1, 500);
	//SPI1_ReadWriteByte(_dat);
    uint8_t i;			  
	//DEV_Digital_Write(DEV_CS_PIN, 0);
	for(i=0;i<8;i++)
	{
		DEV_Digital_Write(DEV_SCL_PIN, 0);
		if(_dat&0x80)
		   DEV_Digital_Write(DEV_SDA_PIN, 1);
		else 
		   DEV_Digital_Write(DEV_SDA_PIN, 0);
		DEV_Digital_Write(DEV_SCL_PIN, 1);
		_dat<<=1;   
	}				 		  
	//DEV_Digital_Write(DEV_CS_PIN, 1);
}

int LCD_1IN47_Module_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG,ENABLE);//使能时钟

    //GPIO初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);//电源

    //GPIO初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_14;
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化    
    
    DEV_Digital_Write(DEV_DC_PIN, 1);
    DEV_Digital_Write(DEV_CS_PIN, 1);
    DEV_Digital_Write(DEV_RST_PIN, 1);
    
    //printf("LCD_1IN47_ Init and Clear...\r\n");
    //LCD_1IN47_SetBackLight(1000);
    LCD_1IN47_Init(VERTICAL);
    LCD_1IN47_Clear(WHITE);

    //printf("Paint_NewImage\r\n");
    Paint_NewImage(LCD_1IN47_WIDTH,LCD_1IN47_HEIGHT, 0, WHITE);

    //printf("Set Clear and Display Funtion\r\n");
    Paint_SetClearFuntion(LCD_1IN47_Clear);
    Paint_SetDisplayFuntion(LCD_1IN47_DrawPoint);

    //printf("Paint_Clear\r\n");
    Paint_Clear(WHITE);
    //DEV_Delay_ms(100);

    Paint_SetRotate(0);
	return 0;
}

void LCD_1IN47_Exit(void)
{
    DEV_Digital_Write(DEV_DC_PIN, 0);
    DEV_Digital_Write(DEV_CS_PIN, 0);
    //close 
    DEV_Digital_Write(DEV_RST_PIN, 0);
    //HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
}


/******************************************************************************
function :	Hardware reset
parameter:
******************************************************************************/
static void LCD_1IN47_Reset(void)
{
	LCD_1IN47_RST_1;
	DEV_Delay_ms(100);
	LCD_1IN47_RST_0;
	DEV_Delay_ms(100);
	LCD_1IN47_RST_1;
	DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
	 Reg : Command register
******************************************************************************/
static void LCD_1IN47_SendCommand(UBYTE Reg)
{
	LCD_1IN47_DC_0;
	LCD_1IN47_CS_0;
	DEV_SPI_WRITE(Reg);
	//LCD_1IN47_CS_1;
}

/******************************************************************************
function :	send data
parameter:
	Data : Write data
******************************************************************************/
static void LCD_1IN47_SendData_8Bit(UBYTE Data)
{
	LCD_1IN47_DC_1;
	//LCD_1IN47_CS_0;
	DEV_SPI_WRITE(Data);
	//LCD_1IN47_CS_1;
}

/******************************************************************************
function :	send data
parameter:
	Data : Write data
******************************************************************************/
static void LCD_1IN47_SendData_16Bit(UWORD Data)
{
	LCD_1IN47_DC_1;
	LCD_1IN47_CS_0;
	DEV_SPI_WRITE((Data >> 8) & 0xFF);
    DEV_SPI_WRITE(Data & 0xFF);
	LCD_1IN47_CS_1;
}

/******************************************************************************
function :	Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_1IN47_InitReg(void)
{
	LCD_1IN47_SendCommand(0x11);
	DEV_Delay_ms(120);

	LCD_1IN47_SendCommand(0x3A);
	LCD_1IN47_SendData_8Bit(0x05);

	LCD_1IN47_SendCommand(0xB2);
	LCD_1IN47_SendData_8Bit(0x0C);
	LCD_1IN47_SendData_8Bit(0x0C);
	LCD_1IN47_SendData_8Bit(0x00);
	LCD_1IN47_SendData_8Bit(0x33);
	LCD_1IN47_SendData_8Bit(0x33);

	LCD_1IN47_SendCommand(0xB7);
	LCD_1IN47_SendData_8Bit(0x35);

	LCD_1IN47_SendCommand(0xBB);
	LCD_1IN47_SendData_8Bit(0x35);

	LCD_1IN47_SendCommand(0xC0);
	LCD_1IN47_SendData_8Bit(0x2C);

	LCD_1IN47_SendCommand(0xC2);
	LCD_1IN47_SendData_8Bit(0x01);

	LCD_1IN47_SendCommand(0xC3);
	LCD_1IN47_SendData_8Bit(0x13);

	LCD_1IN47_SendCommand(0xC4);
	LCD_1IN47_SendData_8Bit(0x20);

	LCD_1IN47_SendCommand(0xC6);
	LCD_1IN47_SendData_8Bit(0x0F);

	LCD_1IN47_SendCommand(0xD0);
	LCD_1IN47_SendData_8Bit(0xA4);
	LCD_1IN47_SendData_8Bit(0xA1);

	LCD_1IN47_SendCommand(0xD6);
	LCD_1IN47_SendData_8Bit(0xA1);

	LCD_1IN47_SendCommand(0xE0);
	LCD_1IN47_SendData_8Bit(0xF0);
	LCD_1IN47_SendData_8Bit(0x00);
	LCD_1IN47_SendData_8Bit(0x04);
	LCD_1IN47_SendData_8Bit(0x04);
	LCD_1IN47_SendData_8Bit(0x04);
	LCD_1IN47_SendData_8Bit(0x05);
	LCD_1IN47_SendData_8Bit(0x29);
	LCD_1IN47_SendData_8Bit(0x33);
	LCD_1IN47_SendData_8Bit(0x3E);
	LCD_1IN47_SendData_8Bit(0x38);
	LCD_1IN47_SendData_8Bit(0x12);
	LCD_1IN47_SendData_8Bit(0x12);
	LCD_1IN47_SendData_8Bit(0x28);
	LCD_1IN47_SendData_8Bit(0x30);

	LCD_1IN47_SendCommand(0xE1);
	LCD_1IN47_SendData_8Bit(0xF0);
	LCD_1IN47_SendData_8Bit(0x07);
	LCD_1IN47_SendData_8Bit(0x0A);
	LCD_1IN47_SendData_8Bit(0x0D);
	LCD_1IN47_SendData_8Bit(0x0B);
	LCD_1IN47_SendData_8Bit(0x07);
	LCD_1IN47_SendData_8Bit(0x28);
	LCD_1IN47_SendData_8Bit(0x33);
	LCD_1IN47_SendData_8Bit(0x3E);
	LCD_1IN47_SendData_8Bit(0x36);
	LCD_1IN47_SendData_8Bit(0x14);
	LCD_1IN47_SendData_8Bit(0x14);
	LCD_1IN47_SendData_8Bit(0x29);
	LCD_1IN47_SendData_8Bit(0x32);

	LCD_1IN47_SendCommand(0x21);

	LCD_1IN47_SendCommand(0x11);
	DEV_Delay_ms(120);
	LCD_1IN47_SendCommand(0x29);
}

/********************************************************************************
function:	Set the resolution and scanning method of the screen
parameter:
		Scan_dir:   Scan direction
********************************************************************************/
static void LCD_1IN47_SetAttributes(UBYTE Scan_dir)
{
	// Get the screen scan direction
	LCD_1IN47.SCAN_DIR = Scan_dir;
	UBYTE MemoryAccessReg = 0x00;

	// Get GRAM and LCD width and height
	if (Scan_dir == HORIZONTAL)
	{
		LCD_1IN47.HEIGHT = LCD_1IN47_WIDTH;
		LCD_1IN47.WIDTH = LCD_1IN47_HEIGHT;
		MemoryAccessReg = 0X00;
	}
	else
	{
		LCD_1IN47.HEIGHT = LCD_1IN47_HEIGHT;
		LCD_1IN47.WIDTH = LCD_1IN47_WIDTH;		
		MemoryAccessReg = 0X70;
	}

	// Set the read / write scan direction of the frame memory
	LCD_1IN47_SendCommand(0x36); // MX, MY, RGB mode
	// LCD_1IN47_SendData_8Bit(MemoryAccessReg);	//0x08 set RGB
	LCD_1IN47_SendData_8Bit(MemoryAccessReg); // 0x08 set RGB
}

/********************************************************************************
function :	Initialize the lcd
parameter:
********************************************************************************/
void LCD_1IN47_Init(UBYTE Scan_dir)
{
	// Hardware reset
	LCD_1IN47_Reset();

	// Set the resolution and scanning method of the screen
	LCD_1IN47_SetAttributes(Scan_dir);

	// Set the initialization register
	LCD_1IN47_InitReg();
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
		Xstart 	:   X direction Start coordinates
		Ystart  :   Y direction Start coordinates
		Xend    :   X direction end coordinates
		Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_1IN47_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
	if (LCD_1IN47.SCAN_DIR == HORIZONTAL)
	{ 
		
		// set the X coordinates
		LCD_1IN47_SendCommand(0x2A);
		LCD_1IN47_SendData_8Bit(0x00);
		LCD_1IN47_SendData_8Bit(Xstart + 0x22);
		LCD_1IN47_SendData_8Bit(((Xend + 0x22) - 1) >> 8);
		LCD_1IN47_SendData_8Bit((Xend + 0x22) - 1);

		// set the Y coordinates
		LCD_1IN47_SendCommand(0x2B);
		LCD_1IN47_SendData_8Bit(0x00);
		LCD_1IN47_SendData_8Bit(Ystart);
		LCD_1IN47_SendData_8Bit((Yend - 1) >> 8);
		LCD_1IN47_SendData_8Bit(Yend - 1);
	}
	else
	{ 
		// set the X coordinates
		LCD_1IN47_SendCommand(0x2A);
		LCD_1IN47_SendData_8Bit(Xstart >> 8);
		LCD_1IN47_SendData_8Bit(Xstart);
		LCD_1IN47_SendData_8Bit((Xend - 1) >> 8);
		LCD_1IN47_SendData_8Bit(Xend - 1);

		// set the Y coordinates
		LCD_1IN47_SendCommand(0x2B);
		LCD_1IN47_SendData_8Bit(Ystart >> 8);
		LCD_1IN47_SendData_8Bit(Ystart + 0x22);
		LCD_1IN47_SendData_8Bit((Yend - 1 + 0x22) >> 8);
		LCD_1IN47_SendData_8Bit(Yend - 1 + 0x22);
	}

	LCD_1IN47_SendCommand(0X2C);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void LCD_1IN47_Clear(UWORD Color)
{
	UWORD i,j;
  LCD_1IN47_SetWindows(0, 0, LCD_1IN47.WIDTH, LCD_1IN47.HEIGHT);

	DEV_Digital_Write(DEV_DC_PIN, 1);
	for(i = 0; i < LCD_1IN47_WIDTH; i++){
		for(j = 0; j < LCD_1IN47_HEIGHT; j++){
			DEV_SPI_WRITE((Color>>8)&0xff);
			DEV_SPI_WRITE(Color);
		}
	 }
}

/******************************************************************************
function :	Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void LCD_1IN47_Display(UWORD *Image)
{
		UWORD i,j;
    LCD_1IN47_SetWindows(0, 0, LCD_1IN47.WIDTH, LCD_1IN47.HEIGHT);
    DEV_Digital_Write(DEV_DC_PIN, 1);
    for(i = 0; i < LCD_1IN47_WIDTH; i++){
      for(j = 0; j < LCD_1IN47_HEIGHT; j++){
        DEV_SPI_WRITE((*(Image+i*LCD_1IN47_HEIGHT+j)>>8)&0xff);
        DEV_SPI_WRITE(*(Image+i*LCD_1IN47_WIDTH+j));
      }
    }

}

void LCD_1IN47_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image)
{
	// display
    UDOUBLE Addr = 0;

    UWORD i,j;
    LCD_1IN47_SetWindows(Xstart, Ystart, Xend , Yend);
    LCD_1IN47_DC_1;
    for (i = Ystart; i < Yend - 1; i++) {
        Addr = Xstart + i * LCD_1IN47_WIDTH ;
        for(j=Xstart;j<Xend-1;j++){
          DEV_SPI_WRITE((*(Image+Addr+j)>>8)&0xff);
          DEV_SPI_WRITE(*(Image+Addr+j));
				}  
		}
}

void LCD_1IN47_DrawPoint(UWORD X, UWORD Y, UWORD Color)
{
	LCD_1IN47_SetWindows(X, Y, X, Y);
	LCD_1IN47_SendData_16Bit(Color);
}

void LCD_1IN47_SetBackLight(UWORD Value)
{
	DEV_Set_PWM(Value);
}

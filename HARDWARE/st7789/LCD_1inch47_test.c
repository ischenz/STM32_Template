#include "image.h"
#include "LCD_Test.h"
#include "LCD_1in47.h"
#include "delay.h"


void LCD_1in47_test()
{
	printf("LCD_1IN47_test Demo\r\n");
	LCD_1IN47_Module_Init();
  
  printf("LCD_1IN47_ Init and Clear...\r\n");
	//LCD_1IN47_SetBackLight(1000);
	LCD_1IN47_Init(VERTICAL);
	LCD_1IN47_Clear(WHITE);
  
  printf("Paint_NewImage\r\n");
	Paint_NewImage(LCD_1IN47_WIDTH,LCD_1IN47_HEIGHT, 0, WHITE);
  
  printf("Set Clear and Display Funtion\r\n");
	Paint_SetClearFuntion(LCD_1IN47_Clear);
	Paint_SetDisplayFuntion(LCD_1IN47_DrawPoint);
  
  printf("Paint_Clear\r\n");
	Paint_Clear(WHITE);
  //DEV_Delay_ms(100);
  
  printf("drawing...\r\n");
	Paint_SetRotate(180);
	Paint_DrawString_EN(30, 10, "123",        &Font24,  YELLOW, RED);  
	Paint_DrawString_EN(30, 34, "ABC",        &Font24,  BLUE,   CYAN);
  Paint_DrawFloatNum (30, 58 ,987.654321,3, &Font12,  WHITE,  BLACK);
	Paint_DrawString_CN(30,130, "Î¢Ñ©µç×Ó",   &Font24CN,WHITE,  BLUE);
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
	LCD_1IN47_Exit();
}


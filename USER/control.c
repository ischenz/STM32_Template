#include "control.h"
#include "oled.h"
#include "pid.h"
#include "pidtool.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "Kalman.h"
#include "servo.h"
#include "delay.h"
#include "led.h"

uint8_t mode = NO_SELECT_MODE;

void (*mode_task)(void) = mode_0;

int8_t target_speed = 30;

void mode_0(void)
{
	
}

void mode_1(void)
{

}

void mode_2(void)
{

}

void mode_3(void)
{

}

void mode_4(void)
{

}

void mode_pro_1(void)
{

}

void mode_pro_2(void)
{

}

void mode_pro_3(void)
{

}

void analyse(uint8_t *lable, uint8_t *load)
{
	switch(*lable){
		case CMD:{
			switch(*load){
				case START:
					break;
				case STOP:
					break;
				case REBOOT:
					NVIC_SystemReset();
					break;
				case BSP_LED:
					LED2 = !LED2;
					break;
				}
			break;
		}
		case CHANGEMODE:{
			mode = *load;
			switch(mode){
				case MODE_1:{
					mode_task = mode_1;
					break;
				}
				case MODE_2:{
					mode_task = mode_2;
					break;
				}
				case MODE_3:{
					mode_task = mode_3;
					break;
				}
				case MODE_4:{
					mode_task = mode_4;
					break;
				}
				case MODE_5:{
					//mode_task = mode_5;
					break;
				}
				case MODE_PRO_1:
					mode_task = mode_pro_1;
					break;
				case MODE_PRO_2:
					mode_task = mode_pro_2;
					break;
				case MODE_PRO_3:
					mode_task = mode_pro_3;
					break;
			}
			break;
		}           
	}                
}                    

//void TIM1_UP_TIM10_IRQHandler(void)//5ms一次pid运算
//{
//	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //溢出中断
//	{
//		if(RecCoorFlag){
//			RecCoorFlag = 0;
//			mode_task();
//			//PID_Calculate(&x_pid, kfp_x.after_kalman - 120);
//			//PID_Calculate(&y_pid, kfp_y.after_kalman - 120);
//		}
//	}
//	TIM_ClearITPendingBit(TIM10,TIM_IT_Update); //清除中断标志位	
//}


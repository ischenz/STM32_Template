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

PID_TypeDef x_pid,y_pid;
uint8_t RecCoorFlag = 0; //接收成功标志
uint8_t mode = NO_SELECT_MODE;
//static uint8_t SetFlag;

void (*mode_task)(void) = mode_0;

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

void set_target(int8_t target_x, int8_t target_y)
{
	set_pid_target(&x_pid, target_x);
	set_pid_target(&y_pid, target_y);
}

void stop(void)
{
	TIM_Cmd(TIM10, DISABLE);//关闭PID计算
	servo_ctr(&Xserv, Xserv.MIDPWM);//0-2000
	servo_ctr(&Yserv, Yserv.MIDPWM);
}

void start(void)
{
	TIM_Cmd(TIM10, ENABLE);
}

void analyse(uint8_t *lable, uint8_t *load)
{
	switch(*lable){
		case CMD:{
			switch(*load){
				case START:
					start();
					break;
				case STOP:
					stop();
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
			//SetFlag = 0;
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
			start();
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


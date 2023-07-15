#include "motor.h"
#include "timer.h"
#include "led.h"

motor motor_l = {0,0,2000,MOTOR_STATUS_FREE};
motor motor_r = {0,0,2000,MOTOR_STATUS_FREE};

/*电机初始化函数*/
void Motor_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //正反转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	Timer1_PWM_GPIO_Init(2-1, 2000);//24Khz   2-1, 2000-1
}

/*绝对值函数*/
int My_abs(int x)
{
	int y;
	if(y < 0) y = -y; 
	return y;
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void PWM_Load(motor* moto, int16_t pwmin)
{
	switch(moto->status)
	{
		case MOTOR_STATUS_STOP:
			if(moto == &motor_l){
				Xin2=1,Xin1=1;
				TIM_SetCompare1(TIM1,moto->PWM_MAX);
			}else{
				Yin2=1,Yin1=1;
				TIM_SetCompare4(TIM1,moto->PWM_MAX);
			}
			break;
		case MOTOR_STATUS_ACTIVE:
			TIM_Cmd(TIM1, ENABLE);
			if(pwmin > moto->PWM_MAX){
				pwmin = moto->PWM_MAX;
			} else if(pwmin < -moto->PWM_MAX){
				pwmin = -moto->PWM_MAX;
			}
			moto->pwm = pwmin;	
			if(moto == &motor_l){
				if(moto->pwm > 0)	Xin1=1,Xin2=0;//x+
				else 				Xin2=1,Xin1=0;//x-
				TIM_SetCompare1(TIM1,My_abs(moto->pwm));
			}else{
				if(moto->pwm > 0)	Yin1=1,Yin2=0;//x+
				else 				Yin2=1,Yin1=0;//x-
				TIM_SetCompare4(TIM1,My_abs(moto->pwm));
			}
			break;
		case MOTOR_STATUS_FREE:
			if(moto == &motor_l){
				TIM_SetCompare1(TIM1, 0);
			}else{
				TIM_SetCompare4(TIM1, 0);
			}
			TIM_Cmd(TIM1, DISABLE);//关闭pwm
			break;
	}
}

void motor_set_status(motor* moto, MOTOR_STATUS_TypeDef status)
{
	moto->status = status;
}

void motor_stop(void)
{
	Xin1=0,Xin2=0;//x+
	Yin2=0,Yin1=0;
	TIM_Cmd(TIM1, DISABLE);
	motor_l.pwm = 0;
	motor_r.pwm = 0;
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare4(TIM1,0);
}

void motor_start(void)
{
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
}



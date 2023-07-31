#include "motor.h"
#include "timer.h"
#include "led.h"

motor motor_l = {
    .pwm = 0,
    .coder_v = 0,
    .PWM_MAX = 2000,
    .status = MOTOR_STATUS_ACTIVE
};
motor motor_r = {    
    .pwm = 0,
    .coder_v = 0,
    .PWM_MAX = 2000,
    .status = MOTOR_STATUS_ACTIVE
};

/*电机初始化函数*/
void motor_gpio_and_timer_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Timer1_PWM_GPIO_Init(168-1, 2000);     /* 0.5kHz   2-1, 2000 */
    Timer5_Init(84-1, 2000);                 /* 0.5kHz   2-1, 2000 */
    Init_Timer3();                           /* 编码器 */
    Init_Timer4();                           /* 编码器 */
    Timer7_init();                           /* 读取编码器速度 */
}

/*绝对值函数*/
int My_abs(int x)
{
	int y = x;
	if(y < 0) y = -y; 
	return y;
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void pwm_load(motor* moto, int16_t pwmin)
{
	switch(moto->status)
	{
		case MOTOR_STATUS_ACTIVE:
			TIM_Cmd(TIM5, ENABLE);
			if(pwmin > moto->PWM_MAX){
				pwmin = moto->PWM_MAX;
			} else if(pwmin < -moto->PWM_MAX){
				pwmin = -moto->PWM_MAX;
			}
			moto->pwm = pwmin;	
			if(moto == &motor_l){
				if(moto->pwm > 0)	Lin1=1,Lin2=0;//x+
				else 				Lin2=1,Lin1=0;//x-
				TIM_SetCompare1(TIM5,My_abs(moto->pwm));
			}else{
				if(moto->pwm > 0)	Rin1=1,Rin2=0;//x+
				else 				Rin2=1,Rin1=0;//x-
				TIM_SetCompare4(TIM5,My_abs(moto->pwm));
			}
			break;
		case MOTOR_STATUS_FREE:
			if(moto == &motor_l){
				TIM_SetCompare1(TIM5, 0);
			}else{
				TIM_SetCompare4(TIM5, 0);
			}
			TIM_Cmd(TIM5, DISABLE);//关闭pwm
			break;
	}
}

void motor_set_status(motor* moto, MOTOR_STATUS_TypeDef status)
{
	moto->status = status;
}

void motor_stop(void)
{
	Lin1=0,Lin2=0;//x+
	Rin2=0,Rin1=0;
	TIM_Cmd(TIM5, DISABLE);
	motor_l.pwm = 0;
	motor_r.pwm = 0;
	TIM_SetCompare1(TIM5,0);
	TIM_SetCompare4(TIM5,0);
}

void motor_start(void)
{
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
}



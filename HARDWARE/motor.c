#include "motor.h"

int PWM_MAX = 1000,PWM_MIN = -1000;

/*电机初始化函数*/
void Motor_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //正反转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*限幅函数*/
void PWM_Limit(int16_t *moto)
{
	if(*moto>PWM_MAX) *moto=PWM_MAX;
	if(*moto<PWM_MIN) *moto=PWM_MIN;
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
void PWM_Load(int moto_x,int moto_y)
{
	if(moto_x>0)	Xin1=1,Xin2=0;//x+
	else 			Xin2=1,Xin1=0;//x-
	TIM_SetCompare4(TIM1,My_abs(moto_x));
	
	if(moto_y>0)	Yin1=1,Yin2=0;
	else 			Yin2=1,Yin1=0;	
	TIM_SetCompare1(TIM1,My_abs(moto_y));
}

void motor_stop(void)
{
	Xin1=0,Xin2=0;//x+
	Yin2=0,Yin1=0;
	TIM_Cmd(TIM1, DISABLE);
	TIM_Cmd(TIM10, DISABLE);
	TIM_SetCompare4(TIM1,0);
	TIM_SetCompare1(TIM1,0);
}

void motor_start(void)
{
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
}



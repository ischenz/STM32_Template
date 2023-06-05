#include "motor.h"
#include "timer.h"

motor motor_l = {0,0,2000};
motor motor_r = {0,0,2000};

/*�����ʼ������*/
void Motor_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //����ת
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	Timer1_PWM_GPIO_Init(2-1, 2000-1);//24Khz   2-1, 2000-1
}

/*����ֵ����*/
int My_abs(int x)
{
	int y;
	if(y < 0) y = -y; 
	return y;
}

/*��ֵ����*/
/*��ڲ�����PID������ɺ������PWMֵ*/
void PWM_Load(motor* moto, int16_t pwmin)
{
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



#include "Kalman.h"

Kalman kfp_x,kfp_y;

void Kalman_Init(Kalman *kalman)
{
	kalman->Last_P = 1;			
	kalman->Now_P = 0;		
	kalman->out = 0;			
	kalman->Kg = 0;		
	kalman->Q = 0.009;//Խ��Խ��˳������ӦԽ��
	kalman->R = 0.10;
	kalman->source = 0;
	kalman->after_kalman = 0;
}

/**
 *�������˲���
 *@param 	Kalman *kfp �������ṹ�����
 *   			float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float KalmanFilter(Kalman *kfp,float input)
{
   //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
   //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out;
}

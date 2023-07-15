#include "track.h"
#include "oled.h"
#include "mpu6050.h"
#include "motor.h"
#include "pid.h"
#include "stdlib.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "gw_grayscale_sensor.h"
#include "sw_i2c.h"

SENSOR_TypeDef track = {
	.cross_num = 0,
	.offset = 0,
	.cross_sensitivity = 4,
	.data = 0,
	.status = 0,
	.mode = 0
};

#ifdef HJduino
void track_io_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��PORTDʱ��
	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12| \
								  GPIO_Pin_13|GPIO_Pin_14| \
								  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#endif

static uint8_t line_sum(void)
{
	uint8_t sum = 0;
	uint8_t i;
	for(i = 0; i < 8; i++){
		sum += !gray_sensor[i];
	}
	return sum;
}

static int8_t get_offset(void)
{
	uint8_t n = ~(track.data);
    uint8_t count = 0;
	int8_t offset = 7;
	uint8_t i;
	
	while (n) {
		n = n & (n << 1);
		count++; //���ٸ�������bit1
	}
	if (line_sum() == count)
	{
		if(count == 1 || count == 2){//1/2��������1
			count = 0;
			n = ~(track.data);
			while (n != 0) {
				n = n & (n - 1);
				count++;
			}

			if(count == 1 || count == 2 ){//�������1
				for(i = 0; i < 8; i++){
					if(GET_NTH_BIT(~(track.data), i+1) == 1){
						offset = i;
						if(GET_NTH_BIT(~(track.data), i+2) == 1){
							offset = offset * 2 + 1;
							break;
						}else{
							offset *= 2;
							break;
						}
					}
				}
			}
		}
	}
	return offset - 7;
}

/*	int8_t get_line(uint8_t mode)
 *  mode 0:  �����ֹͣ��
 *  mode 1:  ���ֹͣ��
 * 
 */
int8_t get_line(uint8_t mode)
{
#ifdef HJduino
	uint8_t linebit;
	int8_t ret = 0;
	track.mode = mode;
	static uint8_t stop_flag = 0;
	uint8_t r1, r2, m, l1, l2;
	
	
	if(track.status == 1)
	{
		r2 = R2; r1 = R1; m = M; l1 = L1; l2 = L2;
		linebit = GPIOD->IDR;
		linebit >>= 11; 
		
		if(l1 == 1 && l2 == 0){
			ret = 1;
		}else if(r1 == 1 && r2 == 0){
			ret = -1;
		}else if(l2 == 1 && m == 0){
			ret = 3;
		}else if(r2 == 1 && m == 0){
			ret = -3;
		}
		track.offset = ret;
		
		if(track.mode == 1){
			if(linebit == 0x1F){
				stop_flag ++;
				if(stop_flag == track.cross_sensitivity){//���ֹͣ��������
					stop_flag = 0;
					track.mode = 0;//  <----------------------------------/ֻ��ʶ��һ��·��
					ret = 100;
					track.cross_num++;
				}
			}				
		}
		track.data = linebit;		
	}		
	return ret;
#endif

#ifdef GW_GRAYCALE
	static uint8_t stop_count = 0, stop_flag;
	uint8_t linebit;
	int8_t offset = 0;
	
	track.mode = mode;
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &linebit); // digital_data ��1~8��̽ͷ��������
	/* ���ֽ����8���������浽�˸����������Ϊgray_sensor[0] ~ gray_sensor[7], 
	 * Ҳ�����Ǳ���val1 ~ val8, ��Ϊ�Ǻ궨�� */
	SEP_ALL_BIT8(linebit, 
		gray_sensor[0], //̽ͷ1
		gray_sensor[1], //̽ͷ2
		gray_sensor[2], //̽ͷ3
		gray_sensor[3], //̽ͷ4
		gray_sensor[4], //̽ͷ5
		gray_sensor[5], //̽ͷ6
		gray_sensor[6], //̽ͷ7
		gray_sensor[7]  //̽ͷ8
	);
	track.data = linebit;
	
	if(track.status == ENABLE){		
		offset = get_offset();
		track.offset = offset;
		
		if(track.mode == 1){//ֹͣ��ģʽ
			if(stop_flag == 0){
				if(linebit == 0x00 || linebit == 0x0F || linebit == 0xF0 || linebit == 0x07 || linebit == 0xE0){
					stop_count ++;
					if(stop_count >= track.cross_sensitivity){//���ֹͣ��������
						stop_count = 0;
						track.cross_num++;
						stop_flag = 1;
					}
				}if(linebit != 0xff){
					track.mode = 1;
				}					
			}else if(stop_flag == 1){//��ȡ��ֹͣ��֮��
				if(linebit != 0x00 && linebit != 0x0F && linebit != 0xF0 && linebit != 0x07 && linebit != 0xE0){
					stop_flag = 0;
				}
			}
		
		}
	}
		
	return offset;
#endif
	
}

void track_cmd(int8_t newstatus)
{
	track.status = newstatus;
}



//void turn(int16_t __dir, int8_t __v)
//{
//	int16_t target_yaw = yaw - __dir; 
//	int16_t err;
//	if(target_yaw > 180){
//		target_yaw -= 360;
//	} else if(target_yaw < -180){  
//		target_yaw += 360;
//	}
//	motor_set_status(&motor_l, MOTOR_STATUS_ACTIVE);
//	motor_set_status(&motor_r, MOTOR_STATUS_ACTIVE);
//	while(1){
//		//printf("yaw=%f, targ=%d \r\n", yaw, target_yaw);
//		if(__dir == RIGHT){
//			set_pid_target(&l_pid, __v);
//			set_pid_target(&r_pid, 0);
//		}else if(__dir == LEFT ){
//			set_pid_target(&l_pid, 0);
//			set_pid_target(&r_pid, __v);
//		}else if(__dir == BACK){
//			set_pid_target(&l_pid, -__v);
//			set_pid_target(&r_pid, __v);
//		}
//		err = abs(target_yaw - (int)yaw);
//		if(err > 180){ 
//			err = 360 - err;
//		} 
//		if(err < 3){
//			break;
//		}
//	}	
//	set_pid_target(&l_pid, 0);
//	set_pid_target(&r_pid, 0);
//	delay_ms(500);
//}

static int calc_min_angle_direction(int now, int targ) {
    // ����˳ʱ����ת�ĽǶȺ���ʱ����ת�ĽǶ�
    int cw_angle = (targ - now +360) % 360;
    int ccw_angle = (now - targ + 360) % 360;
    // �ж���С�ǶȺ���ת����
    if (cw_angle <= ccw_angle) {
        return -cw_angle;
    } else {
        return ccw_angle;
    }
}

static int get_targ(int now, int angle)
{
    //# ������ת�Ƕȼ���Ŀ��Ƕ�
    int targ = (now + angle + 360) % 360;
    return targ;
}

void turn_pid(int16_t __dir, int8_t __v)
{
	
	int16_t target_yaw = get_targ(my_yaw, __dir);
	float veer_p = 0.6, veer_d = 6;
	int16_t err;
	if(__dir == BACK){
		veer_p = 0.2;
		veer_d = 7;
	}
	
	PID_TypeDef veerpid;
	PID_param_init(&veerpid);
	set_pid_target(&veerpid, target_yaw);
	set_p_i_d(&veerpid, veer_p, 0, veer_d);
	
	
	motor_set_status(&motor_l, MOTOR_STATUS_ACTIVE);
	motor_set_status(&motor_r, MOTOR_STATUS_ACTIVE);
	while(1){
		err = calc_min_angle_direction(my_yaw, target_yaw);
		PID_Calculate(&veerpid, target_yaw-err);
		//printf("yaw=%f, targ=%d \r\n", yaw, target_yaw);
		if(__dir == RIGHT){
			set_pid_target(&l_pid, veerpid.PID_out);
			set_pid_target(&r_pid, 0);
		}else if(__dir == LEFT ){
			set_pid_target(&l_pid, 0);
			set_pid_target(&r_pid, -veerpid.PID_out);
		}else {
			set_pid_target(&l_pid, veerpid.PID_out);
			set_pid_target(&r_pid, -veerpid.PID_out);
		}
		
		
		if(abs(err) < 10 && motor_l.coder_v == 0 && motor_r.coder_v == 0){
			break;
		}
	}
	set_pid_target(&l_pid, 0);
	set_pid_target(&r_pid, 0);
	delay_ms(500);
}



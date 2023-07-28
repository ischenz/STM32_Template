#include "oled.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "motor.h"
#include "fatfs.h"
#include "datasave.h"
#include "ch451key.h"
#include "stmflash.h"
#include "tm1650key.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "LCD_1in47.h"
#include "GUI_Paint.h"
#include "ultrasonic.h"
#include "gw_grayscale_sensor.h"
#include "servo.h"

int main(void)
{
	uint32_t heartbeat = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(115200);
    servo_init();    
	//LED_Init();
	//KEY_Init();//weak 与定时器通道冲突
	//OLED_Init();
    LCD_1IN47_Module_Init();
    
    sw_i2c_init();
    grayscale_init();

    servo_ctr(&Xserv, 1500);
    Paint_DrawString_EN(0, 10, "Hello World", &Font24, WHITE, RED);
    OLED_ShowString(0, 10, "Hello World", 12, 1);
    OLED_Refresh();
	printf("Hello Chen!!!\r\n");
	while(1)
	{
        sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); 
        //printf("%#X\r\n", digital_data);
        Paint_DrawNum(10, 30, digital_data, &Font24, WHITE, BLUE);
//        delay_ms(20);
#ifdef DEBUG		
		printf("data=%d\r\n", track.offset);
		printf("%3d %3d %3d %3d %3d %3d %3d %3d\r\n",num_dir[0],num_dir[1],num_dir[2],num_dir[3],num_dir[4],num_dir[5],num_dir[6],num_dir[7]);
		printf("%3d %3d %3d %3d %3d %3d %3d %3d\r\n",num_value[0],num_value[1],num_value[2],num_value[3],num_value[4],num_value[5],num_value[6],num_value[7]);
		count = get_num(num, 0);
		printf("count=%d, %3d, %3d, %3d, %3d \r\n", count, num[0], num[1],num[2],num[3]);
		printf("%3d %3d %3d %3d %3d %3d %3d %3d\r\n",num_value[0],num_value[1],num_value[2],num_value[3],num_value[4],num_value[5],num_value[6],num_value[7]);
		printf("%f,%d\r\n",r_pid.Target,motor_r.coder_v);
#endif		
		
#ifdef DBUG_PID		
		if(receiveJson(&Uart1_RingBuff, str)){
			printf("Rec:\r\n%s \r\n", str);
			OLED_Clear();
			if(read_json_pid(str, "L_PID", &l_p, &l_i, &l_d) == 0 || read_json_pid(str, "R_PID", &r_p, &r_i, &r_d) == 0){
				pid_flash_root_init(ADDR_FLASH_SECTOR_11);//使用扇区11,先擦除flash，后一次性写入全部pid参数
				write_pid_to_flash(L_PID_FLASH_ADDR, "L_PID", l_p, l_i, l_d);
				write_pid_to_flash(R_PID_FLASH_ADDR, "R_PID", r_p, r_i, r_d);
				set_p_i_d(&l_pid, l_p, l_i, l_d);
				set_p_i_d(&r_pid, r_p, r_i, r_d);
			}else{
				OLED_ShowString(0, 0, "Not this PID", 12, 1);
				OLED_Refresh();
			}
			sprintf(showstr, "L_P:%5.2f  R_P:%5.2f",l_p, r_p);
			OLED_ShowString(0, 0, (int8_t *)showstr, 12, 1);
			sprintf(showstr, "L_I:%5.2f  R_I:%5.2f",l_i, r_i);
			OLED_ShowString(0, 20, (int8_t *)showstr, 12, 1);
			sprintf(showstr, "L_D:%5.2f  R_D:%5.2f",l_d, r_d);
			OLED_ShowString(0, 30, (int8_t *)showstr, 12, 1);
			OLED_Refresh();
		}	
#endif
		heartbeat++;
		if(heartbeat%1000000 == 0){
			LED1 = !LED1;
            //OLED_ShowChar(0, 0, 'c', 12, 1);
			//OLED_Refresh();
		}
	}
}

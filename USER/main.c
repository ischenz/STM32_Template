#include "oled.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "motor.h"
#include "control.h"
#include "fatfs.h"
#include "datasave.h"
#include "stmflash.h"
#include "pid.h"
#include "track.h"

#define DEBUG
#define DBUG_PID	

int main(void)
{
    double v_p = 20, v_i = 2, v_d = 0, veer_p = 0, veer_i = 0, veer_d = 0;
#ifdef DBUG_PID	    
    char json_str[80] = {0};
#endif    
	uint32_t heartbeat = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(115200);
    uart2_init(115200);
    printf("Hello Chen!!!\r\n\r\n");
#ifdef DBUG_PID		
    read_pid_from_flash(PID_FLASH_ADDR_0, "V_PID", &v_p, &v_i, &v_d);
    read_pid_from_flash(PID_FLASH_ADDR_1, "VEER_PID", &veer_p, &veer_i, &veer_d);
#endif    
    motor_gpio_and_timer_init();
    pid_timer_init();
    pid_param_init(&l_pid);
    pid_param_init(&r_pid);
    pid_param_init(&veer_pid);
    set_pid_param(&l_pid, v_p, v_i, v_d);
    set_pid_param(&r_pid, v_p, v_i, v_d);
    set_pid_param(&veer_pid, veer_p, veer_i, veer_d);
    
	LED_Init();
	KEY_Init();//weak 与定时器通道冲突
	OLED_Init();
    
    OLED_ShowString(0,  0, "T_PID:", 12, 1);
    OLED_ShowString(0, 13, "V_PID:", 12, 1);
    OLED_ShowString(0, 26, "coder:", 12, 1);
    OLED_ShowString(0, 39, "offst:", 12, 1);
    //OLED_ShowString(0, 52, "offst:", 12, 1);
    OLED_Refresh();

	while(1)
	{

		heartbeat++;
		if(heartbeat%100 == 0){
			LED1 = !LED1;
            //OLED_ShowChar(0, 0, 'c', 12, 1);
			//OLED_Refresh();
		       
#ifdef DEBUG		
            OLED_ShowFNum(36, 0, veer_pid.Kp, 3, 12, 1);
            OLED_ShowFNum(60, 0, veer_pid.Ki, 3, 12, 1);
            OLED_ShowFNum(84, 0, veer_pid.Kd, 3, 12, 1);
            OLED_ShowSNum(36, 26, motor_l.coder_v, 2, 12, 1);
            OLED_ShowSNum(54, 26, motor_r.coder_v, 2, 12, 1);
            OLED_ShowSNum(36, 39, track.offset, 2, 12, 1);
            //OLED_ShowSNum(36, 52, track.offset, 3, 12, 1);
            OLED_Refresh();
        }
#else
        }
#endif	
		
#ifdef DBUG_PID	
        //printf("%d\n", motor_l.coder_v);
        delay_us(30);
		if(receiveJson(&Uart1_RingBuff, json_str)){
			printf("Rec:\r\n%s \r\n", json_str);
			if(read_json_pid(json_str, "V_PID", &v_p, &v_i, &v_d) == 0 || read_json_pid(json_str, "VEER_PID", &veer_p, &veer_i, &veer_d) == 0){
				pid_flash_root_init(ADDR_FLASH_SECTOR_11);/* 使用扇区11,先擦除flash，后一次性写入全部pid参数, 避免下次写入是擦除整个扇区丢失数据。*/
				write_pid_to_flash(PID_FLASH_ADDR_0, "V_PID", v_p, v_i, v_d);
				write_pid_to_flash(PID_FLASH_ADDR_1, "VEER_PID", veer_p, veer_i, veer_d);
				set_pid_param(&l_pid, v_p, v_i, v_d);
				set_pid_param(&r_pid, v_p, v_i, v_d);
                set_pid_param(&veer_pid, veer_p, veer_i, veer_d);
			}else{
                printf("Error: Not this pid\r\n");
			}
            /* 
			sprintf(showstr, "L_P:%5.2f  R_P:%5.2f",l_p, r_p);
			OLED_ShowString(0, 0, (int8_t *)showstr, 12, 1);
			sprintf(showstr, "L_I:%5.2f  R_I:%5.2f",l_i, r_i);
			OLED_ShowString(0, 20, (int8_t *)showstr, 12, 1);
			sprintf(showstr, "L_D:%5.2f  R_D:%5.2f",l_d, r_d);
			OLED_ShowString(0, 30, (int8_t *)showstr, 12, 1);
			OLED_Refresh(); */
		}	
#endif
	}
}

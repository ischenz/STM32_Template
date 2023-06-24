/*
 ******************************************************
 STM32F407ZG KEIL例程 大越创新开发板

 202207081915	创建例程
 202305281734	改进
 202305302021	添加版本管理
 202306051313	移植u8g2,cJSON
 202306061142	在spiflash中存取pid参数

 作者：			ischenz						V2.0
 ******************************************************
 */
#include "oled.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "motor.h"
#include "u8g2.h"
#include "u8g2_init.h"
#include "fatfs.h"
#include "datasave.h"
#include "ch451key.h"
#include "stmflash.h"

char str[100] = {0};

int main(void)
{
	double l_p,l_i,l_d,r_p,r_i,r_d;
	char showstr[50];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init(168);
	uart_init(9600);
	Ch451Key_init();
	LED_Init();
	KEY_Init();//weak不能使用 与定时器通道冲突
	RingBuff_Init(&Uart1_RingBuff);
	fatfs_init();
//	read_pid_from_spiflash("L_PID", &l_p, &l_i, &l_d);
//	read_pid_from_spiflash("R_PID", &r_p, &r_i, &r_d);
	
	read_pid_from_flash(L_PID_FLASH_ADDR, "L_PID", &l_p, &l_i, &l_d);
	read_pid_from_flash(R_PID_FLASH_ADDR, "R_PID", &r_p, &r_i, &r_d);
	
	
	u8g2_init();
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_template);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFont(&u8g2,u8g2_font_12x6LED_tf);

	u8g2_ClearBuffer(&u8g2);
	sprintf(showstr, "L_P:%5.2f  R_P:%5.2f",l_p, r_p);
	u8g2_DrawStr(&u8g2, 0, 25, showstr);
	sprintf(showstr, "L_I:%5.2f  R_I:%5.2f",l_i, r_i);
	u8g2_DrawStr(&u8g2, 0, 40, showstr);
	sprintf(showstr, "L_D:%5.2f  R_D:%5.2f",l_d, r_d);
	u8g2_DrawStr(&u8g2, 0, 55, showstr);
	u8g2_SendBuffer(&u8g2);
	
	printf("hello chen\r\n");
	
	while(1)
	{
		if(receiveJson(&Uart1_RingBuff, str)){
			printf("Rec:\r\n%s \r\n", str);
			u8g2_ClearBuffer(&u8g2);
			
			if(read_json_pid(str, "L_PID", &l_p, &l_i, &l_d) == 0 || read_json_pid(str, "R_PID", &r_p, &r_i, &r_d) == 0){
				pid_flash_root_init(ADDR_FLASH_SECTOR_11);//使用扇区11,先擦除flash，后一次性写入全部pid参数
				write_pid_to_flash(L_PID_FLASH_ADDR, "L_PID", l_p, l_i, l_d);
				write_pid_to_flash(R_PID_FLASH_ADDR, "R_PID", r_p, r_i, r_d);
			}else{
				u8g2_DrawStr(&u8g2, 0, 25, "Not this PID");
				u8g2_SendBuffer(&u8g2);
			}
			sprintf(showstr, "L_P:%5.2f  R_P:%5.2f",l_p, r_p);
			u8g2_DrawStr(&u8g2, 0, 25, showstr);
			sprintf(showstr, "L_I:%5.2f  R_I:%5.2f",l_i, r_i);
			u8g2_DrawStr(&u8g2, 0, 40, showstr);
			sprintf(showstr, "L_D:%5.2f  R_D:%5.2f",l_d, r_d);
			u8g2_DrawStr(&u8g2, 0, 55, showstr);
			u8g2_SendBuffer(&u8g2);
		}	
	}
}

/*
 * Copyright (c) 2022 感为智能科技(济南)
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#include "sys.h"  
#include "sw_i2c.h"
#include "gw_grayscale_sensor.h"
#include "delay.h"
#include "usart.h"

uint8_t gray_sensor[8];
uint8_t digital_data;

void grayscale_init(void)
{
	sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据

	/* 读取开关量数据 */
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据
	/* 把字节里的8个开关量存到八个变量里，这里为gray_sensor[0] ~ gray_sensor[7], 
	 * 也可以是变量val1 ~ val8, 因为是宏定义 */
	SEP_ALL_BIT8(digital_data, 
		gray_sensor[0], //探头1
		gray_sensor[1], //探头2
		gray_sensor[2], //探头3
		gray_sensor[3], //探头4
		gray_sensor[4], //探头5
		gray_sensor[5], //探头6
		gray_sensor[6], //探头7
		gray_sensor[7]  //探头8
	);
}




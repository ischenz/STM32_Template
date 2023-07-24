/*
 * Copyright (c) 2022 ��Ϊ���ܿƼ�(����)
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
	/* ���ɨ�赽�ĵ�ַ */
	volatile uint8_t count;

	sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data ��1~8��̽ͷ��������

	/* ��ȡ���������� */
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data ��1~8��̽ͷ��������
	/* ���ֽ����8���������浽�˸����������Ϊgray_sensor[0] ~ gray_sensor[7], 
	 * Ҳ�����Ǳ���val1 ~ val8, ��Ϊ�Ǻ궨�� */
	SEP_ALL_BIT8(digital_data, 
		gray_sensor[0], //̽ͷ1
		gray_sensor[1], //̽ͷ2
		gray_sensor[2], //̽ͷ3
		gray_sensor[3], //̽ͷ4
		gray_sensor[4], //̽ͷ5
		gray_sensor[5], //̽ͷ6
		gray_sensor[6], //̽ͷ7
		gray_sensor[7]  //̽ͷ8
	);
}




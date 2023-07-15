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
/* 设置软件IIC驱动 */
sw_i2c_interface_t i2c_interface = 
{
	.sda_in = sda_in,
	.scl_out = scl_out,
	.sda_out = sda_out,
	.user_data = 0, //用户数据，可在输入输出函数里得到
};
void grayscale_init(void)
{
	/* 存放扫描到的地址 */
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	uint8_t ping_response;
	/* 初始化IIC */
	sw_i2c_init();
	/* ps: 软件IIC初始化的时候会出发一次IIC start，会导致第一次IIC通讯会失败 */
	
	/* 第一次IIC通讯会失败（因为软件IIC触发了start），手动发个stop也能消除 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	/* 后面IIC通讯是正常的 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);

	count = i2c_scan(&i2c_interface, scan_addr);
	printf("count=%d, addr=%#X\r\n", count, scan_addr[0]);
	/* 读取开关量数据 */	
	/* 打开开关量数据模式 */
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

/**
 * 初始化i2c
 */
void sw_i2c_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA);
}

/* 定义sda输出函数 bit=0为低电平 bit=1为高电平 */
void sda_out(uint8_t bit, void *user_data)
{
	SDA_OUT();
	GPIO_WriteBit(GPIOC, SW_I2C1_PIN_SDA, (BitAction)bit);
	
	/* IIC软件延迟 */
	delay_us(10);
}

/* 定义sda读取函数 bit 为返回的电平值 */
uint8_t sda_in(void *user_data)
{
	uint8_t bit;
	SDA_IN();
	bit = (uint8_t)GPIO_ReadInputDataBit(GPIOC, SW_I2C1_PIN_SDA);
	
	/* IIC软件延迟 */
	delay_us(10);
	return bit;
}

/* 定义scl时钟输出函数 bit=0为低电平 bit=1为高电平 */
void scl_out(uint8_t bit, void *user_data)
{
	GPIO_WriteBit(GPIOC, SW_I2C1_PIN_SCL, (BitAction)bit);
	
	/* IIC软件延迟 */
	delay_us(10);
}

/**
 * i2c地址扫描
 * @param scan_addr 扫描出来的地址存放,数值不为0的为扫描到的地址，扫到的地址会挨个放在数组的最前面
 * @return 返回扫描到的设备数量, 0为无设备发现
 */
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr)
{
	int i;
	uint8_t count = 0;
	uint8_t data;
	int8_t ret;
	
	for (i = 1; i < 127; ++i) {
		ret = sw_i2c_read(i2c_interface, i << 1, &data, 1);
		if (ret == 0) {
			scan_addr[count] = i;
			++count;
		}
	}
	
	return count;
}


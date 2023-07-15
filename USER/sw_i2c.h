/*
 * Copyright (c) 2023 感为智能科技(济南)
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#ifndef SW_I2C_H_GUARD
#define SW_I2C_H_GUARD

#include <stdint.h>

typedef struct {
	void (*sda_out)(uint8_t bit, void *user_data);
	uint8_t (*sda_in)(void *user_data);
	void (*scl_out)(uint8_t bit, void *user_data);
	void *user_data;
} sw_i2c_interface_t;

/**
 * @brief 从IIC总线上的设备读取多个字节
 * @param i2c_interface
 * @param dev_addr 从设备地址
 * @param[out] data 读取到的字节数组
 * @param data_length 读取大小(字节)
 * @return 0:成功, 1:错误
 */
int8_t sw_i2c_read(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data, uint8_t data_length);
int8_t sw_i2c_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t *data, uint8_t data_length);

/**
 * @brief 从IIC总线上的设备读取一个字节
 * @param i2c_interface
 * @param dev_addr 从设备地址
 * @param[out] data 读取到的字节
 * @return 0:成功, 1:错误
 */
int8_t sw_i2c_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data);
int8_t sw_i2c_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t data);

/**
 * @brief 读取IIC总线从设备的寄存器数据. 即先写入寄存器地址,无终止位,再连续读取所需数据
 * @param i2c_interface
 * @param dev_addr 从设备地址
 * @param mem_addr 寄存器地址
 * @param[out] data 读取到的字节数组
 * @param data_length 读取大小(字节),不包括寄存器地址本身
 * @return 0:成功, 1:错误
 */
int8_t sw_i2c_mem_read(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint8_t data_length);

/**
 * @brief 写入IIC总线从设备的寄存器. 即先写入寄存器地址,再连续写入数组中的数据
 * @param i2c_interface
 * @param dev_addr 从设备地址
 * @param mem_addr 寄存器地址
 * @param[out] data 连续写入的数据
 * @param data_length 所写入的字节大小,不包括寄存器地址本身
 * @return 0:成功, 1:错误
 */
int8_t sw_i2c_mem_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t mem_addr, const uint8_t *data, uint8_t data_length);

#endif //SW_I2C_H_GUARD

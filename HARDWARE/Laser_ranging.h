#ifndef __LASTER_RANGING_H
#define __LASTER_RANGING_H

#include "sys.h"
#include <stdint.h>
#include "sw_i2c.h"

#define SW_I2C1_PIN_SCL GPIO_Pin_8
#define SW_I2C1_PIN_SDA GPIO_Pin_7

#define SDA_IN()  {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;}	//PC7输入模式
#define SDA_OUT() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;} //PC7输出模式

/* 默认地址 */
#define GW_GRAY_ADDR_DEF 0x4C

/**
 * @brief 从I2C得到的8位的数字信号的数据 读取第n位的数据
 * @param sensor_value_8 数字IO的数据
 * @param n 第1位从1开始, n=1 是传感器的第一个探头数据, n=8是最后一个
 * @return
 */
#define GET_NTH_BIT(sensor_value, nth_bit) (((sensor_value) >> ((nth_bit)-1)) & 0x01)


/**
 * @brief 从一个变量分离出所有的bit
 */
#define SEP_ALL_BIT8(sensor_value, val1, val2, val3, val4, val5, val6, val7, val8) \
do {                                                                              \
val1 = GET_NTH_BIT(sensor_value, 1);                                              \
val2 = GET_NTH_BIT(sensor_value, 2);                                              \
val3 = GET_NTH_BIT(sensor_value, 3);                                              \
val4 = GET_NTH_BIT(sensor_value, 4);                                              \
val5 = GET_NTH_BIT(sensor_value, 5);                                              \
val6 = GET_NTH_BIT(sensor_value, 6);                                              \
val7 = GET_NTH_BIT(sensor_value, 7);                                              \
val8 = GET_NTH_BIT(sensor_value, 8);                                              \
} while(0)

void Laser_ranging_init(void);
void sw_i2c_init(void);
void sda_out(uint8_t bit, void *user_data);
uint8_t sda_in(void *user_data);
void scl_out(uint8_t bit, void *user_data);
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr);

void Laser_ranging_getdistance(uint16_t *__dis);

extern sw_i2c_interface_t i2c_interface;


#endif


#ifndef __U8G2_INIT_H
#define __U8G2_INIT_H

#include "sys.h"
#include "u8g2.h"

extern u8g2_t u8g2;

void u8g2_init(void);
uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* __U8G2_INIT_H */


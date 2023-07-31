#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define NO_SELECT_MODE	0x00;
#define MODE_1			0x01
#define MODE_2			0x02
#define MODE_3			0x03
#define MODE_4			0x04
#define MODE_5			0x05
#define MODE_PRO_1 		0x06
#define MODE_PRO_2 		0x07
#define MODE_PRO_3 		0x08
#define MODE_PRO_4 		0x09

#define CMD				0xff
#define DATA			0x80
#define SETPID			0x81

#define STOP			0xbb
#define START			0xaa
#define REBOOT  		0x00
#define CHANGEMODE 		0xab
#define BSP_LED			0xff


void mode_test(void);
void mode_0(void);
void mode_1(void);
void mode_2(void);
void mode_3(void);
void mode_4(void);
void mode_pro_1(void);
void mode_pro_2(void);
void mode_pro_3(void);
void mode_select(void);
void stop(void);
void start(void);
void analyse(uint8_t *lable, uint8_t *load);
extern uint8_t mode, SetFlag;
extern int8_t target_speed;

#endif /* __CONTROL_H */

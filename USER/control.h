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
#define DOM1			1
#define DOM2			2
#define DOM3			3
#define DOM4			4
#define DOM5			5
#define DOM6			6
#define DOM7			7
#define DOM8			8
#define DOM9			9

#define CMD				0xff
#define DATA			0x80

#define STOP			0xbb
#define START			0xaa
#define REBOOT  		0x00
#define CHANGEMODE 		0xab
#define BSP_LED			0xff
#define P_A 			0x80
#define P_B 			0x81
#define P_C 			0x82
#define P_D 			0x83

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
void set_dom(uint8_t dom);
void get_dom(uint8_t dom, int8_t *target_x, int8_t *target_y);
void set_target(int8_t target_x, int8_t target_y);
uint8_t getball_p(uint8_t *x, uint8_t *y);
int8_t GoTo(int8_t current_x, int8_t current_y,
		    int8_t target_x,  int8_t target_y,
			int8_t step_arr[3][2]);

extern uint8_t mode, SetFlag;
extern uint8_t Pro2_A_Place,Pro2_B_Place,Pro2_C_Place,Pro2_D_Place;

#endif /* __CONTROL_H */

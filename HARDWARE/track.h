#ifndef __TRACK_H
#define __TRACK_H

#include "sys.h"

#define GW_GRAYCALE 

#ifdef HJduino

#define R2 PDin(15)
#define R1 PDin(14)
#define M  PDin(13)
#define L1 PDin(12)
#define L2 PDin(11)

#endif

#define RIGHT	 (-90)
#define LEFT	(90)
#define BACK	 (180)

typedef struct{
	uint8_t cross_num;          /* 路口数量     */
	int8_t offset;              /* 偏移量       */
	int8_t cross_sensitivity;   /* 路口灵敏度   */
	uint8_t data;               /* 传感器原始值 */
	int8_t status;              /* 传感器状态   */
	int8_t mode;                /* 传感器模式   */
}Sensor_TypeDef;

extern Sensor_TypeDef track;

void track_io_init(void);
int8_t get_line(uint8_t mode);
void track_cmd(int8_t newstatus);

void turn(int16_t __dir, int8_t __v);
void turn_pid(int16_t __dir, int8_t __v);
	
#endif /* __TRACK_H*/


#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "sys.h"

#define PID_FLASH_ADDR_0	(ADDR_FLASH_SECTOR_11)
#define PID_FLASH_ADDR_1	(ADDR_FLASH_SECTOR_11 + 1024)

void read_pid_from_spiflash(const char *pid, double *p, double *i, double *d);
void write_pid_to_spiflash(const char *pid, double p, double i, double d);
int8_t read_json_pid(const char *str, const char *pid, double *p, double *i, double *d);
void pid_flash_root_init(uint32_t addr);
void write_pid_to_flash(uint32_t addr, const char *pid, double p, double i, double d);
void read_pid_from_flash(uint32_t addr, const char *pid, double *p, double *i, double *d);

#endif


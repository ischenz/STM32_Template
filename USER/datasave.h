#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "sys.h"

void read_pid_from_spiflash(const char *pid, double *p, double *i, double *d);
void write_pid_to_spiflash(const char *pid, double p, double i, double d);
int8_t read_json_pid(const char *str, const char *pid, double *p, double *i, double *d);

#endif


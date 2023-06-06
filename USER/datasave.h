#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "sys.h"

void read_pid(const char *pid, double *p, double *i, double *d);
void write_pid(const char *pid, double p, double i, double d);

#endif


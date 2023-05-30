#ifndef __FATFS_H
#define __FATFS_H

#include "sys.h"
#include "ff.h"
#include "string.h"

uint8_t mf_scan_files(uint8_t * path);
void fatfs_init(void);
UINT* Mylog(const char* log);
int copytosd(void);
#endif


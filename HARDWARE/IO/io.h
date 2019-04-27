#ifndef _IO_H
#define _IO_H

#include "sys.h"

#define IO_MICROSWITCH PBin(0)
#define IO_ENSURANCE1 PBin(4)
#define IO_ENSURANCE2 PBin(5)
#define IO_SOFT_EMERGE PBin(13)

void IO_Init(void);

#endif

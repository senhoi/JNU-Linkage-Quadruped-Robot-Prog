#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#include "delay.h"

#define BEEP PFout(8)

void BEEP_Init(void);

void BEEP_Normal(uint8_t num);
void BEEP_Alert(uint8_t num);
void BEEP_Error(uint8_t num);

#endif

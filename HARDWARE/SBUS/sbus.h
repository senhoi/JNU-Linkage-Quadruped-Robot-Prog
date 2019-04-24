#ifndef _SBUS_H
#define _SBUS_H

#include "stm32f4xx.h"

extern uint8_t SBUS_MsgPack[];
extern uint16_t SBUS_ChanelVal[];

void SBUS_Encode(void);
void SBUS_Decode(void);

#endif

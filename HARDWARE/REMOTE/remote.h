#ifndef _REMOTE_H
#define _REMOTE_H

#include "stm32f4xx.h"

#define SBUS_IN	1
#define UPWM_IN	2

#define RCV_SIG	SBUS_IN

#include "led.h"
#include "sbus.h"

typedef enum{
	MoveMode_Stand	= 0x00,
	MoveMode_Walk 	= 0x01,
	MoveMode_Trot	= 0x02,
	MoveMode_Error	= 0xFF,
}MoveMode_TypeDef;

typedef enum{
	CtrlMode_Error	= 0xFF,
	CtrlMode_XYW	= 0x00,
	CtrlMode_NOA	= 0x01,
	CtrlMode_RPY	= 0x02,
}CtrlMode_TypeDef;

extern volatile uint8_t RemoteUpdated;

typedef struct Remote_t
{
	uint16_t LX;
	uint16_t LY;
	uint16_t RX;
	uint16_t RY;
	uint16_t LS;
	uint16_t RS;
	uint16_t VA;
	uint16_t VB;
	uint16_t VC;
}Remote_t;

void UpdateRemoteInfo(void* PWM_Array);
int8_t Remote_GetLX(void);
int8_t Remote_GetLY(void);
int8_t Remote_GetRX(void);
int8_t Remote_GetRY(void);
uint8_t Remote_GetLS(void);
uint8_t Remote_GetRS(void);
uint8_t Remote_GetVA(void);
int8_t *Remote_Get(void);

uint16_t Remote_GetLX_Pulse(void);
uint16_t Remote_GetLY_Pulse(void);
uint16_t Remote_GetRX_Pulse(void);
uint16_t Remote_GetRY_Pulse(void);
uint16_t Remote_GetLS_Pulse(void);
uint16_t Remote_GetRS_Pulse(void);
uint16_t Remote_GetVA_Pulse(void);

void RemoteAnalyzeTask(void);
int8_t Remote_PulseToVal(uint16_t PulseWidth);

#endif

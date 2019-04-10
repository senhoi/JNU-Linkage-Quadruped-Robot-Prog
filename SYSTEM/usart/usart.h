#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "tasks.h"
#include "remote.h"

#define USART_REC_LEN 200
#define EN_USART1_RX 1 //使能（1）/禁止（0）串口1接收

extern volatile uint8_t FrameState;
extern uint8_t RxStream[];

void USART1_Init(u32 bound);
void USART3_Init(u32 bound);
void UART4_Init(u32 bound);

#endif

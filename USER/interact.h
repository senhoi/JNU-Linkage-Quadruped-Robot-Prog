#ifndef INTERACT_H
#define INTERACT_H

#include "sys.h"
#include "usart.h"
#include "led.h"

#define FRAMETYPE_REMOTE		0x11
#define FRAMETYPE_CAMERA		0x21
#define FRAMETYPE_GYROSCOPE 0x22

#define SYNSIG PBout(2)

typedef union {
	float fl;
	uint8_t u8[4];
} float_uint8_t;

typedef union {
	uint16_t u16;
	uint8_t u8[2];
} uint16_uint8_t;

typedef union {
	uint32_t u32;
	uint8_t u8[4];
} uint32_uint8_t;

void SynSig_Init(void);
int InvertActrRawData(void);
void UploadRemoteData(int8_t *RemoteData,char len);

#endif

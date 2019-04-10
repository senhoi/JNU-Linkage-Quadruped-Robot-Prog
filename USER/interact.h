#ifndef INTERACT_H
#define INTERACT_H

#include "sys.h"
#include "usart.h"

#define FRAMETYPE_REMOTE		0x11
#define FRAMETYPE_CAMERA		0x21
#define FRAMETYPE_GYROSCOPE 0x22

#define SYNSIG PBout(2)

typedef union {
	float fl;
	unsigned char u8[4];
} fl2u8;

void SynSig_Init(void);
int InvertActrRawData(void);
void UploadRemoteData(int8_t *RemoteData,char len);

#endif

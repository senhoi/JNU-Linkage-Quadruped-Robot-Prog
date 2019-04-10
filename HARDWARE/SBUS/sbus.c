#include "sbus.h"

/*这个文件是解析SBUS协议用的*/

uint8_t SBUS_MsgPack[25];
uint16_t SBUS_ChanelVal[16];

void SBUS_Encode(void)
{
	SBUS_MsgPack[0]  = 0x0F; 
	SBUS_MsgPack[1]  = (uint8_t) ((SBUS_ChanelVal[0]	& 0x07FF));
	SBUS_MsgPack[2]  = (uint8_t) ((SBUS_ChanelVal[0]	& 0x07FF) >> 8	| (SBUS_ChanelVal[1]	& 0x07FF) << 3);
	SBUS_MsgPack[3]  = (uint8_t) ((SBUS_ChanelVal[1]	& 0x07FF) >> 5	| (SBUS_ChanelVal[2]	& 0x07FF) << 6);
	SBUS_MsgPack[4]  = (uint8_t) ((SBUS_ChanelVal[2]	& 0x07FF) >> 2);
	SBUS_MsgPack[5]  = (uint8_t) ((SBUS_ChanelVal[2]	& 0x07FF) >> 10	| (SBUS_ChanelVal[3]	& 0x07FF) << 1);
	SBUS_MsgPack[6]  = (uint8_t) ((SBUS_ChanelVal[3]	& 0x07FF) >> 7	| (SBUS_ChanelVal[4]	& 0x07FF) << 4);
	SBUS_MsgPack[7]  = (uint8_t) ((SBUS_ChanelVal[4]	& 0x07FF) >> 4	| (SBUS_ChanelVal[5]	& 0x07FF) << 7);
	SBUS_MsgPack[8]  = (uint8_t) ((SBUS_ChanelVal[5]	& 0x07FF) >> 1);
	SBUS_MsgPack[9]  = (uint8_t) ((SBUS_ChanelVal[5]	& 0x07FF) >> 9	| (SBUS_ChanelVal[6]	& 0x07FF) << 2);
	SBUS_MsgPack[10] = (uint8_t) ((SBUS_ChanelVal[6]	& 0x07FF) >> 6	| (SBUS_ChanelVal[7]	& 0x07FF) << 5);
	SBUS_MsgPack[11] = (uint8_t) ((SBUS_ChanelVal[7]	& 0x07FF) >> 3);
	SBUS_MsgPack[12] = (uint8_t) ((SBUS_ChanelVal[8]	& 0x07FF));
	SBUS_MsgPack[13] = (uint8_t) ((SBUS_ChanelVal[8]	& 0x07FF) >> 8	| (SBUS_ChanelVal[9]	& 0x07FF) << 3);
	SBUS_MsgPack[14] = (uint8_t) ((SBUS_ChanelVal[9]	& 0x07FF) >> 5	| (SBUS_ChanelVal[10]	& 0x07FF) << 6);  
	SBUS_MsgPack[15] = (uint8_t) ((SBUS_ChanelVal[10]	& 0x07FF) >> 2);
	SBUS_MsgPack[16] = (uint8_t) ((SBUS_ChanelVal[10]	& 0x07FF) >> 10	| (SBUS_ChanelVal[11]	& 0x07FF) << 1);
	SBUS_MsgPack[17] = (uint8_t) ((SBUS_ChanelVal[11]	& 0x07FF) >> 7	| (SBUS_ChanelVal[12]	& 0x07FF) << 4);
	SBUS_MsgPack[18] = (uint8_t) ((SBUS_ChanelVal[12]	& 0x07FF) >> 4	| (SBUS_ChanelVal[13]	& 0x07FF) << 7);
	SBUS_MsgPack[19] = (uint8_t) ((SBUS_ChanelVal[13]	& 0x07FF) >> 1);
	SBUS_MsgPack[20] = (uint8_t) ((SBUS_ChanelVal[13]	& 0x07FF) >> 9	| (SBUS_ChanelVal[14]	& 0x07FF) << 2);
	SBUS_MsgPack[21] = (uint8_t) ((SBUS_ChanelVal[14]	& 0x07FF) >> 6	| (SBUS_ChanelVal[15]	& 0x07FF) << 5);
	SBUS_MsgPack[22] = (uint8_t) ((SBUS_ChanelVal[15]	& 0x07FF) >> 3);
	SBUS_MsgPack[23] = 0x00;
	SBUS_MsgPack[24] = 0X00;
}

void SBUS_Decode(void)
{
	SBUS_ChanelVal[0]  = ((SBUS_MsgPack[1]		| SBUS_MsgPack[2] << 8)	& 0x07FF);
	SBUS_ChanelVal[1]  = ((SBUS_MsgPack[2] >> 3	| SBUS_MsgPack[3] << 5)	& 0x07FF);
	SBUS_ChanelVal[2]  = ((SBUS_MsgPack[3] >> 6	| SBUS_MsgPack[4] << 2 | SBUS_MsgPack[5] << 10)	& 0x07FF);
	SBUS_ChanelVal[3]  = ((SBUS_MsgPack[5] >> 1	| SBUS_MsgPack[6] << 7)	& 0x07FF);
	SBUS_ChanelVal[4]  = ((SBUS_MsgPack[6] >> 4	| SBUS_MsgPack[7] <<4)	& 0x07FF);
	SBUS_ChanelVal[5]  = ((SBUS_MsgPack[7] >> 7	| SBUS_MsgPack[8] << 1 | SBUS_MsgPack[9] << 9)	& 0x07FF);
	SBUS_ChanelVal[6]  = ((SBUS_MsgPack[9] >> 2	| SBUS_MsgPack[10] << 6)	& 0x07FF);
	SBUS_ChanelVal[7]  = ((SBUS_MsgPack[10] >> 5	| SBUS_MsgPack[11] << 3)	& 0x07FF);
	SBUS_ChanelVal[8]  = ((SBUS_MsgPack[12]		| SBUS_MsgPack[13] << 8)	& 0x07FF);
	SBUS_ChanelVal[9]  = ((SBUS_MsgPack[13] >> 3	| SBUS_MsgPack[14]<<5)	& 0x07FF);
	SBUS_ChanelVal[10] = ((SBUS_MsgPack[14] >> 6	| SBUS_MsgPack[15]<<2 | SBUS_MsgPack[16] << 10)	& 0x07FF);
	SBUS_ChanelVal[11] = ((SBUS_MsgPack[16] >> 1	| SBUS_MsgPack[17]<<7)	& 0x07FF);
	SBUS_ChanelVal[12] = ((SBUS_MsgPack[17] >> 4	| SBUS_MsgPack[18]<<4)	& 0x07FF);
	SBUS_ChanelVal[13] = ((SBUS_MsgPack[18] >> 7	| SBUS_MsgPack[19]<<1 | SBUS_MsgPack[20]<<9)	& 0x07FF);
	SBUS_ChanelVal[14] = ((SBUS_MsgPack[20] >> 2	| SBUS_MsgPack[21]<<6)	& 0x07FF);
	SBUS_ChanelVal[15] = ((SBUS_MsgPack[21] >> 5	| SBUS_MsgPack[22]<<3)	& 0x07FF);
}

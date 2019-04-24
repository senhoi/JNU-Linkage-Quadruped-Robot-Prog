#include "remote.h"

/*这个文件是获取遥控器数据的接口*/

const int16_t Pulse_Max = 1478, Pulse_Mid = 1024, Pulse_Min = 568; 

Remote_t Remote;

int8_t RemoteData[7];

void RemoteInit(void)
{
	Remote.LX = 0;
	Remote.LY = 0;
	Remote.RX = 0;
	Remote.RY = 0;
	Remote.LS = 0;
	Remote.RS = 0;
	Remote.VA = 0;
}

void UpdateRemoteInfo(void* PWM_Array)
{
#if RCV_SIG == UPWM_IN
	Remote.RX = ((int16_t*)PWM_Array)[0];
	Remote.RY = ((int16_t*)PWM_Array)[2];
	Remote.LY = ((int16_t*)PWM_Array)[1];
	Remote.LX = ((int16_t*)PWM_Array)[3];
	Remote.LS = ((int16_t*)PWM_Array)[7];
	Remote.RS = ((int16_t*)PWM_Array)[8];
	Remote.VA = ((int16_t*)PWM_Array)[4];
	Remote.VB = ((int16_t*)PWM_Array)[5];
	Remote.VC = ((int16_t*)PWM_Array)[6];
#else
	#if RCV_SIG == SBUS_IN
	/*Prevent Warning*/
	*((uint8_t*)PWM_Array) = *((uint8_t*)PWM_Array);
	
	SBUS_Decode();
	Remote.RX = SBUS_ChanelVal[0];
	Remote.RY = SBUS_ChanelVal[1];
	Remote.LY = SBUS_ChanelVal[2];
	Remote.LX = SBUS_ChanelVal[3];
	Remote.LS = SBUS_ChanelVal[4];
	Remote.VA = SBUS_ChanelVal[5];
	Remote.RS = SBUS_ChanelVal[6];
	#endif
#endif
}

/*以下七个函数是获取转换后的通道数据的函数*/
int8_t Remote_GetLX(void)
{
	return Remote_PulseToVal(Remote_GetLX_Pulse());
}

int8_t Remote_GetLY(void)
{
	return Remote_PulseToVal(Remote_GetLY_Pulse());
}

int8_t Remote_GetRX(void)
{
	return Remote_PulseToVal(Remote_GetRX_Pulse());
}

int8_t Remote_GetRY(void)
{
	return Remote_PulseToVal(Remote_GetRY_Pulse());
}

uint8_t Remote_GetLS(void)
{
	uint16_t PulseWidth = Remote_GetLS_Pulse();
	if(PulseWidth == Pulse_Min)
		return CtrlMode_XYW;
	if(PulseWidth == Pulse_Mid)
		return CtrlMode_NOA;
	if(PulseWidth == Pulse_Max)
		return CtrlMode_RPY;
	return CtrlMode_Error;
}

uint8_t Remote_GetRS(void)
{
	uint16_t PulseWidth = Remote_GetRS_Pulse();
	if(PulseWidth == Pulse_Min)
		return MoveMode_Trot;
	if(PulseWidth == Pulse_Max)
		return MoveMode_Walk;
	if(PulseWidth == Pulse_Mid)
		return MoveMode_Stand;
	return MoveMode_Error;
}

uint8_t Remote_GetVA(void)
{
	return (Remote_GetVA_Pulse() - 340) / 6;
}

int8_t *Remote_Get(void)
{
	RemoteData[0] = Remote_GetLX();
	RemoteData[1] = Remote_GetLY();
	RemoteData[2] = Remote_GetRX();
	RemoteData[3] = Remote_GetRY();
	RemoteData[4] = Remote_GetLS();
	RemoteData[5] = Remote_GetRS();
	RemoteData[6] = Remote_GetVA();
	return RemoteData;
}

/*以下七个函数为获取接收机的原始输出脉宽*/

uint16_t Remote_GetLX_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.LX);
}

uint16_t Remote_GetLY_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.LY);
}

uint16_t Remote_GetRX_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.RX);
}

uint16_t Remote_GetRY_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.RY);
}

uint16_t Remote_GetLS_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.LS);
}

uint16_t Remote_GetRS_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.RS);
}

uint16_t Remote_GetVA_Pulse(void)
{
	return *((volatile uint16_t*)&Remote.VA);
}

void RemoteAnalyzeTask(void)
{
#if RCV_SIG == UPWM_IN
	static uint8_t ch;
	static uint8_t RawData[22] = {0};
	static uint8_t RxState = 0, RxDataIndex = 0;
	if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		ch = USART_ReceiveData(USART2);
		switch(RxState)
		{
			case 0x00:				//还未检测到起始标志，开始匹配起始标志第一字节
				if(ch == 0x55)
					RxState++;
				else
					RxState = 0;
				break;
			case 0x01:				//起始标志第一字节已匹配，开始匹配起始标志第二字节
				if(ch == 0xAA)
				{
					RxDataIndex = 0;
					RxState++;
				}
				else
					if(ch == 0x55);
					else
						RxState = 0;
				break;
			case 2:					//起始标志已经匹配，开始接收原始数据
				RawData[RxDataIndex] = ch;
				RxDataIndex++;
				if(RxDataIndex >= 18)
				{
					RxDataIndex = 0;
					RxState++;
				}
				break;
			case 3:					//数据已经接收完毕，开始匹配结束标志第一字节
				if(ch == 0xAA)
					RxState++;
				else
					RxState = 0;
				break;
			case 4:					//结束标志第一字节已匹配，开始匹配结束标志第二字节
				if(ch == 0x55)
				{
					UpdateRemoteInfo((void*)&RawData[0]);
					RemoteUpdated = 30;
				}
				RxState = 0;
				break;
			default:
				RxState = 0;
		}
	}
#else
	#if RCV_SIG == SBUS_IN
	static uint8_t ch;
	static uint8_t RxState = 0, RxDataIndex = 0;
	if(USART_GetFlagStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		ch = USART_ReceiveData(UART4);
		switch(RxState)
		{
			case 0:				//还未检测到起始标志，开始匹配起始标志
				if(ch == 0x0F)
				{
					RxState++;
					SBUS_MsgPack[0] = ch;
					RxDataIndex = 1;
				}
				else
					RxState = 0;
				break;
			case 1:					//起始标志匹配，开始接收原始数据
				SBUS_MsgPack[RxDataIndex] = ch;
				RxDataIndex++;
				if(RxDataIndex >= 23)
				{
					RxDataIndex = 0;
					RxState++;
				}
				break;
			case 2:					//数据已经接收完毕，开始匹配结束标志第一字节
					SBUS_MsgPack[23] = ch;
					RxState++;
				break;
			case 3:					//结束标志第一字节已匹配，开始匹配结束标志第二字节
				if(ch == 0x00)
				{
					UpdateRemoteInfo((void*)&SBUS_MsgPack[0]);
					SBUS_MsgPack[24] = 0x00;
				}
				RxState = 0;
				break;
			default:
				RxState = 0;
		}
	}
	#endif
#endif
}

#if RCV_SIG == SBUS_IN
#else
	#if RCV_SIG == UPWM_IN
const uint16_t MidPulse = 1520;
const uint16_t MidError = 20;
	#endif
#endif

/*
 *	春哥要的原始数据转成int8_t
 *	这里还可以优化，因为对于SBUS，原始数据未经零点纠正的话，
 *	数据一定是在 568 - 1478 内，除以（456/12t）可以用满int8_t
*	不过考虑到一般不至于跑到最大速度，也可以不改以避免误触油门校准导致数据溢出
 */
int8_t Remote_PulseToVal(uint16_t PulseWidth)
{
	return (PulseWidth - Pulse_Mid) / 6;
}

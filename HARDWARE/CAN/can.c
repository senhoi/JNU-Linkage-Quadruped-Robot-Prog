#include "can.h"

extern CanTxMsg g_tCanTxMsg; /* 用于发送 */
extern CanRxMsg g_tCanRxMsg; /* 用于接收 */

void CAN1_Init(void)
{
	CAN_InitTypeDef CAN_InitSructrue;
	CAN_FilterInitTypeDef CAN_FilterInitSructrue;
	GPIO_InitTypeDef GPIO_InitSructrue;
	NVIC_InitTypeDef NVIC_InitTypeSructrue;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

	GPIO_InitSructrue.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitSructrue.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitSructrue);

	NVIC_InitTypeSructrue.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitTypeSructrue.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitTypeSructrue.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitTypeSructrue.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeSructrue);

	NVIC_InitTypeSructrue.NVIC_IRQChannel = CAN1_TX_IRQn;
	NVIC_InitTypeSructrue.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitTypeSructrue.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitTypeSructrue.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeSructrue);

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitSructrue);

	CAN_InitSructrue.CAN_TTCM = DISABLE;
	CAN_InitSructrue.CAN_ABOM = DISABLE;
	CAN_InitSructrue.CAN_AWUM = DISABLE;
	CAN_InitSructrue.CAN_NART = DISABLE;
	CAN_InitSructrue.CAN_RFLM = DISABLE;
	CAN_InitSructrue.CAN_TXFP = ENABLE;
	CAN_InitSructrue.CAN_Mode = CAN_Mode_Normal;
	CAN_InitSructrue.CAN_SJW = CAN_SJW_1tq;
	CAN_InitSructrue.CAN_BS1 = CAN_BS1_9tq;
	CAN_InitSructrue.CAN_BS2 = CAN_BS2_4tq;
	CAN_InitSructrue.CAN_Prescaler = 3; //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN1, &CAN_InitSructrue);

	CAN_FilterInitSructrue.CAN_FilterNumber = 0;
	CAN_FilterInitSructrue.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitSructrue.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitSructrue.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitSructrue.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitSructrue.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitSructrue.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitSructrue.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitSructrue.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitSructrue);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	//CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
}

void CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}
}

extern void Can1InterruptHandler(void);

void CAN1_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
		CAN_ClearFlag(CAN1, CAN_FLAG_FF0);
		Can1InterruptHandler();
	}
}

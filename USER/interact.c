#include "interact.h"

extern float actrAngle[12];

void SynSig_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int InvertActrRawData(void)
{
	int i, j;
	int actrRevFlag = 0;
	fl2u8 temp[12];

	if (USART1_RX_STA & 0x8000)
	{
		if ((USART1_RX_STA & 0x3fff) == 48)
		{
			for (i = 0; i < 12; i++)
			{
				for (j = 0; j < 4; j++)
				{
					temp[i].u8[3 - j] = USART1_RX_BUF[4 * i + j];
				}
				actrAngle[i] = temp[i].fl / 3.1416f * 32.0f;
			}
			actrRevFlag = 1;
		}
		else
			actrRevFlag = 0;

		USART1_RX_STA = 0;
	}
	return actrRevFlag;
}

void UploadData(unsigned char c)
{
    USART_SendData(USART1, c);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
}

void UploadRemoteData(int8_t *RemoteData,char len)
{
	char i;
	UploadData(0xAA);
	UploadData(0x55);
	UploadData(0xAA);
	UploadData(0x55);
	UploadData(FRAMETYPE_REMOTE);
	UploadData(0x07);
	for(i=0;i<7;i++)
	{
		UploadData(RemoteData[i]);
	}
}

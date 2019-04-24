#include "exti.h"

void EXTI0_IRQHandler(void)
{
	static uint8_t rising_flag;

	if (EXTI_GetITStatus(EXTI_Line0))
	{
		for (int i = 0; i < 80; i++)
			for (int j = 0; j < 80; j++)
				;
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) == Bit_SET)
		{
			if (!rising_flag)
			{
				BEEP_Error(1);
			}
			rising_flag = 1;
		}
		else
		{
			BEEP_Error(1);
			rising_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	static uint8_t rising_flag;

	if (EXTI_GetITStatus(EXTI_Line1))
	{
		for (int i = 0; i < 80; i++)
			for (int j = 0; j < 80; j++)
				;
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == Bit_SET)
		{
			if (!rising_flag)
			{
				BEEP_Error(1);
			}
			rising_flag = 1;
		}
		else
		{
			rising_flag = 0;
			BEEP_Error(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI2_IRQHandler(void)
{
	static uint8_t rising_flag;

	if (EXTI_GetITStatus(EXTI_Line2))
	{
		for (int i = 0; i < 80; i++)
			for (int j = 0; j < 80; j++)
				;
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == Bit_SET)
		{
			if (!rising_flag)
				BEEP_Error(1);
			rising_flag = 1;
		}
		else
		{
			BEEP_Error(1);
			rising_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler(void)
{
	static uint8_t rising_flag;

	if (EXTI_GetITStatus(EXTI_Line3))
	{
		for (int i = 0; i < 80; i++)
			for (int j = 0; j < 80; j++)
				;
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) == Bit_SET)
		{
			if (!rising_flag)
				BEEP_Error(1);
			rising_flag = 1;
		}
		else
		{
			BEEP_Error(1);
			rising_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Ê¹ÄÜSYSCFGÊ±ÖÓ

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

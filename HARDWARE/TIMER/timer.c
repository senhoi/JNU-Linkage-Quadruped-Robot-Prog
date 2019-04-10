#include "timer.h"

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period = 1000;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)
{
	static uint16_t t = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		t++;
		if (t % 10 == 0)
		{
			SYNSIG = 1;
			for (int i = 0; i < 100; i++)
				;
			SYNSIG = 0;
			TASK_SET_FLAG(TASK_FLAG_UPLOAD);
			TASK_SET_FLAG(TASK_FLAG_CONTROL);
		}
		if (t % 50 == 0)
		{
			//TASK_SET_FLAG(TASK_FLAG_REPORT_POS);
		}
		if (t % 500 == 0)
		{
			//LED0 = !LED0;
		}
		if (t == 1000)
		{
			t = 0;
			if (TASK_CHECK_REGULAR_REPORT_FLAG)
				TASK_SET_FLAG(TASK_FLAG_REPORT_ALL);
		}
	}
}

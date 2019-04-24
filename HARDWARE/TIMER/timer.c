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

int AutoTime_Flag = 0;
int AutoTime_S = 0;
int AutoTime_MS = 0;

void TIM3_IRQHandler(void)
{
	static uint16_t t = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		t++;
		if (t % 10 == 0)
		{
			TASK_SET_FLAG(TASK_FLAG_CONTROL);
			TASK_SET_FLAG(TASK_FLAG_REPORT_POS);
			if (AutoTime_Flag)
			{
				AutoTime_MS += 10;
				if (AutoTime_MS == 1000)
				{
					AutoTime_MS = 0;
					AutoTime_S++;
				}
			}
		}
		if (t % 50 == 0)
		{
			//TASK_SET_FLAG(TASK_FLAG_REPORT_POS);
			TASK_SET_FLAG(TASK_FLAG_REPORT_CUR);
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

void TIM14_PWM_Init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM14, ENABLE);

	TIM_Cmd(TIM14, ENABLE);

	TIM_SetCompare1(TIM14, 2500);
}

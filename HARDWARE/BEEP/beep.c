#include "beep.h"

void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器对应引脚GPIOF8拉低，
}

void BEEP_Normal(uint8_t num)
{
    for (int i = 0; i < num; i++)
    {
        delay_ms(300);
        BEEP = 1;
        delay_ms(300);
        BEEP = 0;
    }
}

void BEEP_Alert(uint8_t num)
{
    for (int i = 0; i < num; i++)
    {
        delay_ms(150);
        BEEP = 1;
        delay_ms(150);
        BEEP = 0;
    }
}

void BEEP_Error(uint8_t num)
{
    for (int i = 0; i < num; i++)
    {
        delay_ms(100);
        BEEP = 1;
        delay_ms(50);
        BEEP = 0;
    }
}

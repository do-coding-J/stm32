#include "tim_defs.h"

TIM_HandleTypeDef timer6 = {0};

// system clock configured as 180 000 000

void TIM_Init(void)
{
    // target : TIM6
    TIM_MasterConfigTypeDef mastercfg = {0};

    __HAL_RCC_TIM6_CLK_ENABLE();
    timer6.Instance = TIM6;

    timer6.Init.Prescaler = (HAL_RCC_GetSysClockFreq()) / (TIM6_Target_Freq * ARR_FIXED) - 1;
    timer6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    timer6.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer6.Init.Period = 999;
    timer6.Init.RepetitionCounter = 0x00;

    HAL_TIM_Base_Init(&timer6);

    mastercfg.MasterOutputTrigger = TIM_TRGO_ENABLE;
    mastercfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timer6, &mastercfg);

    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // HAL_TIM_Base_Start(&timer6);
    HAL_TIM_Base_Start_IT(&timer6);
}
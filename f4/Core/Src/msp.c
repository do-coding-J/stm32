#include "main.h"

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    
    // enable PendSV_IRQn interrupt cfg
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}
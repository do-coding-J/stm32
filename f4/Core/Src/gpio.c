#include "gpio_defs.h"

void GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    // GPIOB
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio_init.Pin = GREEN_PIN | BLUE_PIN | RED_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_WritePin(GPIOB, gpio_init.Pin, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOB, &gpio_init);

    // btn poll
    // GPIOC
    // __HAL_RCC_GPIOC_CLK_ENABLE();
    // gpio_init.Pin = B1_PIN;
    // gpio_init.Mode = GPIO_MODE_INPUT;
    // gpio_init.Pull = GPIO_NOPULL;
    // gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    // HAL_GPIO_WritePin(GPIOC, gpio_init.Pin, GPIO_PIN_RESET);
    // HAL_GPIO_Init(GPIOC, &gpio_init);

    // btn it
    // GPIOC
    __HAL_RCC_GPIOC_CLK_ENABLE();
    gpio_init.Pin = B1_PIN;
    gpio_init.Mode = GPIO_MODE_IT_RISING;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
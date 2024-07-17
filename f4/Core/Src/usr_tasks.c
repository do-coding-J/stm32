#include "usr_tasks.h"

void TASK_Init(void)
{
    xTaskCreate(&prvLedRedTask, "red led blink", 100, NULL, 2, NULL);
}

void prvLedRedTask(void *parameters)
{
    for (;;)
    {
        HAL_GPIO_TogglePin(RED_PORT, RED_PIN);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
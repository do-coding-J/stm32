#ifndef __MAIN_H__
#define __MAIN_H__

// std include
#include <stdbool.h>

// lib include
#include "stm32f4xx_hal.h" 
#include "FreeRTOS.h"

// freertos include
#include "task.h"
#include "timers.h"
#include "semphr.h"

// hardware include
#include "gpio_pin_defs.h"

// functions
static void Error_Handler(void);
static void SystemClock_Config(void);


#endif /*__MAIN_H_*/
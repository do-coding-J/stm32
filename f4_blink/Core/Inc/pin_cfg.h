#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"

// LED Green
#define LED_GREEN_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_0

// LED blue
#define LED_BLUE_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_7

// LED red
#define LED_RED_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14

// USR Btn
#define USR_BTN_Port GPIOC
#define USR_BTN_Pin GPIO_PIN_13

#endif
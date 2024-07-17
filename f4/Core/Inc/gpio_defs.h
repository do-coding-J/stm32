#ifndef __GPIO_PIN_DEF_H__
#define __GPIO_PIN_DEF_H__

#include "main.h"
#include "stm32f446xx.h"

/*
GPIOB
Pin = GREEN_PIN | BLUE_PIN | RED_PIN 
Mode = GPIO_MODE_OUTPUT_PP
Pull = GPIO_NOPULL
Speed = GPIO_SPEED_FREQ_MEDIUM
Alternate = None(no settings for led)
*/

/*
GPIOC
Pin = B1_PIN
Mode = GPIO_MODE_INPUT
Pull = GPIO_NOPULL
Speed = GPIO_SPEED_FREQ_HIGH
Alternate = None(no setting for btn)
*/

#define GREEN_PORT      GPIOB
#define GREEN_PIN       GPIO_PIN_0
#define BLUE_PORT       GPIOB
#define BLUE_PIN        GPIO_PIN_7
#define RED_PORT        GPIOB
#define RED_PIN         GPIO_PIN_14

#define B1_PORT         GPIOC
#define B1_PIN          GPIO_PIN_13

void GPIO_Init(void);

#endif
#ifndef __TIM_DEF_H__
#define __TIM_DEF_H__

#include "stm32f446xx.h"
#include "main.h"

#define ARR_FIXED (100 - 1)   // 100
#define TIM6_Target_Freq 10 // Hz // 0.1 sec

void TIM_Init(void);

#endif
#ifndef TIME_HAL_H
#define TIME_HAL_H

#include "BAT32A237.h"
#include "common_type.h"
#include "tima.h"
#include "isr.h"

void TIMER_HAL_Init(void);
void TIMER_HAL_1msPeriod(void);
uint8_t  TIMER_HAL_Is1msTickTimeout(void);
uint8_t TIMER_HAL_Is100msTickTimeout(void);
uint32_t TIMER_HAL_GetTimerTickCnt(void);

#endif

#ifndef __KEY_INTP_DRV_H__
#define __KEY_INTP_DRV_H__
#include "gpio.h"
#include "intp.h"
#include "isr.h"

extern uint8_t flag;

void INTP_Start(uint16_t pinMsk);
void INTP_Stop(uint16_t pinMsk);
void Intp_Init(void);

void Key_Init(void);
void key_interrupt(void *msg);
void intp0_interrupt(void *msg);

#endif

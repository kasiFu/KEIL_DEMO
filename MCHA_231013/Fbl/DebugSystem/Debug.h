#ifndef DEBUG_H
#define DEBUG_H
#include "BAT32A237.h"
#include "userdefine.h"
#include "sci_common.h"
#include "uart.h"
#include "uart_drv.h"

void DEBUG_Init(void);
void DEBUG_IO_Deinit(void);
void DEBUG_IO_SetDebugIOLow(void);
void DEBUG_IO_SetDebugIOHigh(void);
void DEBUG_IO_ToggleDebugIO(void);

#endif

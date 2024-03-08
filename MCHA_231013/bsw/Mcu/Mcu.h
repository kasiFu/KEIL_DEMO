/***********************************************************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Mcu.h
*
*   Description: Source Code File for Real Time Environment
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2023-09-14      Armand.zhao     Initial created
*
***********************************************************************************************************************/
#ifndef MCU_H
#define MCU_H 1 

#include "Std_Types.h"
#include "BAT32A237.h"
#include "gpio.h"

#define MCU_Toggle_TEST_TP11() GPIO_ToggleBits(GPIO_PORT6, GPIO_Pin_1) 
#define MCU_Toggle_TEST_TP12() GPIO_ToggleBits(GPIO_PORT6, GPIO_Pin_0)


extern void MCU_Config(void);
extern void MCU_Interrupt_Config(void);
#endif

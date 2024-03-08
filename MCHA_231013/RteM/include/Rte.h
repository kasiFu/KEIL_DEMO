/*****************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Rte.h
*
*   Description: Header file for Real Time Environment
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2021-10-04      XueFeng.Fan     Initial created
*
*****************************************************************************/
#ifndef RTE_H
#define RTE_H 1
/*****************************************************************************
*   Include Files
*****************************************************************************/
#include "Std_Types.h"
/*****************************************************************************
*   Global Define Definitions
*****************************************************************************/

/*****************************************************************************
*   Global Type Definitions
*****************************************************************************/
typedef struct
{
	uint16 TickCycle;
	uint16 CounterCycle;
    /*Max Task ID, Used for Task Loop Back*/
    uint32 TaskSize;
    /*function call*/
    const uint32 *PollingTaskPtr;
}Rte_ConfigType;
/*****************************************************************************
*   Global Macros Definitions
*****************************************************************************/
#define TASK(TaskName) void Rte_##TaskName(void)
/*****************************************************************************
*   Global Data Declarations
*****************************************************************************/

/*****************************************************************************
*   Global Function Declarations
*****************************************************************************/
extern TASK(1ms_Task);
extern TASK(2ms_Task);
extern TASK(10ms_Task_A);
extern TASK(10ms_Task_B);
extern TASK(10ms_Task_C);
extern TASK(10ms_Task_D);
extern TASK(10ms_Task_E);
extern void Rte_Init(void);
extern void Rte_TaskPolling(void);
extern void Rte_Start(void);
extern void Rte_Stop(void);
extern void Rte_IncrementCounter(void);
#endif
/*End of Rte.h file*/

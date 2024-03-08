
/***********************************************************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Rte.c
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
/*****************************************************************************
*   Include Files
*****************************************************************************/
#include "Rte.h"
#include "EcuM.h"
#include "gpio.h"
#include "Massage.h"
#include "Analog.h"
#include "keyScan.h"
#include "Mcu.h"
#include "lin_protocol.h"
#include "uds_app.h"
#include "wdt.h"
#include "BootCfg.h"


/*****************************************************************************
*   Local Define Definitions
*****************************************************************************/
/*****************************************************************************
*   Local Type Definitions
*****************************************************************************/
typedef struct
{
	boolean CounterStartEn;
    uint16  Tick;
    uint16  Counter;
    uint16  TaskId; 
}Rte_VariableType;
/*****************************************************************************
*   Local Macro Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Function Declarations
*****************************************************************************/

/*****************************************************************************
*   Local Data Definitions
*****************************************************************************/
volatile Rte_VariableType Rte_B;
/*****************************************************************************
*   Global Data Definitions
*****************************************************************************/
extern const Rte_ConfigType Rte_Config;
/*****************************************************************************
*   Static/Inline Function Definitions
*****************************************************************************/

/*****************************************************************************
*   Global/Extrnal Function Definitions
*****************************************************************************/


/*****************************************************************************
*
*   Function          :   TASK(1ms_Task)
*
*   Description       :   1ms Task:
*
*****************************************************************************/

TASK(1ms_Task)
{
	WDT_Restart();
	UDS_MainFun();
	
	ResetMonitor(); // 检测是需要跳转到APP
	BootMonitor();
	Uds_FlashControlMain();

	Analog_Task1ms();
}
/*****************************************************************************
*
*   Function          :   TASK(2ms_Task)
*
*   Description       :   2ms Task: 33.9us
*
*****************************************************************************/
TASK(2ms_Task)
{	
}
/*****************************************************************************
*
*   Function          :   TASK(10ms_Task_A)
*
*   Description       :   10ms Task A
*
*****************************************************************************/
TASK(10ms_Task_A)
{
	EcuM_MainFunction_10ms();
	MCU_Toggle_TEST_TP12();
}
/*****************************************************************************
*
*   Function          :   TASK(10ms_Task_B)
*
*   Description       :   10ms Task B
*
*****************************************************************************/
TASK(10ms_Task_B)
{
	keyScan_Task10ms();
}
/*****************************************************************************
*
*   Function          :   TASK(10ms_Task_C)
*
*   Description       :   10ms Task C
*
*****************************************************************************/
TASK(10ms_Task_C)
{
	Massage_task10ms();
	//MCU_Toggle_TEST_TP11();
}
/*****************************************************************************
*
*   Function          :   TASK(10ms_Task_D)
*
*   Description       :   10ms Task D
*
*****************************************************************************/
TASK(10ms_Task_D)
{
	
}
/*****************************************************************************
*
*   Function          :   TASK(10ms_Task_E)
*
*   Description       :   10ms Task E
*
*****************************************************************************/
TASK(10ms_Task_E)
{
	
}
/*****************************************************************************
*
*   Function          :   Rte_Init
*
*   Description       :   Os Task Intial
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :
*
*****************************************************************************/
void Rte_Init(void)
{
	Rte_B.TaskId = 0;
	Rte_B.CounterStartEn = FALSE;
    Rte_B.Counter = 0;
    Rte_B.Tick = 0;
}
/*****************************************************************************
*
*   Function          :   Rte_Start
*
*   Description       :   Os Counter Start
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :
*
*****************************************************************************/
void Rte_Start(void)
{
    Rte_B.CounterStartEn = TRUE;
}
/*****************************************************************************
*
*   Function          :   Rte_Stop
*
*   Description       :   Os Counter Stop
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :
*
*****************************************************************************/
void Rte_Stop(void)
{
    Rte_B.CounterStartEn = FALSE;
}
/*****************************************************************************
*
*   Function          :   Rte_IncrementCounter
*
*   Description       :   Os Counter Increment
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :
*
*****************************************************************************/
void Rte_IncrementCounter(void)
{
	if(TRUE ==  Rte_B.CounterStartEn)
	{
		Rte_B.Tick++;
		if(Rte_B.Tick >= Rte_Config.TickCycle)
		{
			Rte_B.Tick = 0;
			Rte_B.Counter++;
			if(Rte_B.Counter >= Rte_Config.CounterCycle)
			{
				Rte_B.Counter = 0;
			}
		}
	}
}
/*****************************************************************************
*
*   Function          :   Rte_TaskPolling
*
*   Description       :   Os Background Task Polling
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :   Endless loop For Os Task Polling
*
*****************************************************************************/
void Rte_TaskPolling(void)
{
    /*Main loop for system task execution*/
    while(1)
    {
        /*Check for Task slice is up*/
        if(Rte_B.TaskId != Rte_B.Counter)
        {
            /*Save Task ID*/
            Rte_B.TaskId = Rte_B.Counter;
            /*Execute Task*/
            ((void (*)())Rte_Config.PollingTaskPtr[Rte_B.TaskId])();
        }
    }
}
/*End of Rte.c file*/

/*****************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Rte_Cfg.c
*
*   Description: Configuration Data File for Rte
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2021-10-04      XueFeng.Fan     Initial created
*
*****************************************************************************/
/*****************************************************************************
*   Include Files
*****************************************************************************/
#include "Rte.h"
/*****************************************************************************
*   Local Define Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Type Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Macro Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Function Declarations
*****************************************************************************/
/*****************************************************************************
*   Local Data Definitions
*****************************************************************************/
/*Task Table Definition*/
const uint32 Rte_lPollingTaskConfig[] =
{
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º1 */
		(uint32)(&Rte_10ms_Task_A),    	/* Task Number£º2 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º3 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number£º4 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º5 */
		(uint32)(&Rte_10ms_Task_B),    	/* Task Number£º6 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º7 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number£º8 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º9 */
		(uint32)(&Rte_10ms_Task_C),    	/* Task Number£º10 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º11 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number£º12 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º13 */
		(uint32)(&Rte_10ms_Task_D),    	/* Task Number£º14 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º15 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number£º16 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º17 */
		(uint32)(&Rte_10ms_Task_E),    	/* Task Number£º18 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number£º19 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number£º20 */
};
/*****************************************************************************
*   Global Data Definitions
*****************************************************************************/
const Rte_ConfigType Rte_Config =
{
		/*Define Ticks number for Counter Increment one:*/
		.TickCycle = 1,/*500us*/
		/*Define Counter Increment Maximum Value:*/
		.CounterCycle = 20,/*10ms*/
		/*Task Size*/
        .TaskSize = sizeof(Rte_lPollingTaskConfig)/sizeof(uint32),
		/*Task Point*/
        .PollingTaskPtr = (uint32 *)Rte_lPollingTaskConfig,
};
/*End of Rte_Cfg.c file*/

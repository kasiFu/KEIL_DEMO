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
		(uint32)(&Rte_1ms_Task),    		/* Task Number��1 */
		(uint32)(&Rte_10ms_Task_A),    	/* Task Number��2 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��3 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number��4 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��5 */
		(uint32)(&Rte_10ms_Task_B),    	/* Task Number��6 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��7 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number��8 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��9 */
		(uint32)(&Rte_10ms_Task_C),    	/* Task Number��10 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��11 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number��12 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��13 */
		(uint32)(&Rte_10ms_Task_D),    	/* Task Number��14 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��15 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number��16 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��17 */
		(uint32)(&Rte_10ms_Task_E),    	/* Task Number��18 */
		(uint32)(&Rte_1ms_Task),    		/* Task Number��19 */
		(uint32)(&Rte_2ms_Task),    		/* Task Number��20 */
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

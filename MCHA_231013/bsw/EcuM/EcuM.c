/*****************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,LTD.
*
*   All rights reserved.
*
*   Filename:    EcuM.c
*
*   Description: Source Code File for Ecu State Management
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2021-10-04      Armand.zhao     Initial created
*
*****************************************************************************/
/*****************************************************************************
*   Include Files
*****************************************************************************/

#include "EcuM.h"
#include "Rte.h"
#include "Mcu.h"
#include "lin_protocol.h"
#include "dma.h"
#include "timm.h"
#include "sci_common.h"
#include "wdt.h"
#include "debug.h"
#include "timer_hal.h"
#include "common_type.h"
#include "uds_app.h"
#include "BootCfg.h"
/*****************************************************************************
*   Local Define Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Type Definitions
*****************************************************************************/
typedef struct
{ 
	boolean        ShutDownRequest;
	EcuM_StateType State;
}EcuM_VariableType;
/*****************************************************************************
*   Local Macro Definitions
*****************************************************************************/

/*****************************************************************************
*   Local Function Declarations
*****************************************************************************/

/*****************************************************************************
*   Local Data Definitions
*****************************************************************************/
EcuM_VariableType EcuM_B;
/*****************************************************************************
*   Global Data Definitions
*****************************************************************************/
extern void EcuM_AL_DriverInit(void);
extern void EcuM_AL_SwitchOff(void);
/*****************************************************************************
*   Static/Inline Function Definitions
*****************************************************************************/

/*****************************************************************************
*   Global/Extrnal Function Definitions
*****************************************************************************/
/*****************************************************************************
*
*   Function          :   EcuM_Init
*
*   Description       :   EcuM Module Initial
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
void EcuM_Init(void)
{
    /* Initial Variable */
    EcuM_B.State = ECU_STATE_M_INIT;
    EcuM_B.ShutDownRequest = FALSE;
	/* Rte Init*/
	Rte_Init();
	/* Driver Init */
	EcuM_AL_DriverInit();
	MCU_Interrupt_Config();

	LIN_SleepIO_Init();
	LIN_SLEEP_LOW;
	LIN_SetConfig();
	LIN_SLEEP_HIGH; //拉高休眠脚电平	
	lin_state = LIN_IDLE;

	TIMER_HAL_Init();
	BootInit();

	/* Start System Counter */
	Rte_Start();
    /* Rte Start */
	Rte_TaskPolling();
}
/*****************************************************************************
*
*   Function          :   EcuM_ShutDownRequest
*
*   Description       :   Ecu Shut Down Request
*
*   Parameters(in)    :   EcuM ShutDown Type
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
void EcuM_ShutDownRequest(EcuM_ShutDownType ShutDownType)
{
	if(ECU_STATE_M_NORMAL_SHUT_DOWN == ShutDownType)
	{
		EcuM_B.ShutDownRequest = TRUE;
	}
	else
	{
		while(1)
		{
			/*Trigger Wdg TimeOut*/
		}
	}
}
/*****************************************************************************
*
*   Function          :   EcuM_MainFunction_10ms
*
*   Description       :   EcuM Module Main function  
*
*   Parameters(in)    :   None
*
*   Parameters(in-out):   None
*
*   Parameters(out)   :   None
*
*   Return Value      :   None
*
*   Notes             :   None
*
*****************************************************************************/
void EcuM_MainFunction_10ms(void)
{
	switch(EcuM_B.State)
	{
	case ECU_STATE_M_INIT:
		EcuM_B.State = ECU_STATE_M_NORMAL;
		break;
	case ECU_STATE_M_NORMAL:
		if(TRUE == EcuM_B.ShutDownRequest)
		{
			EcuM_B.State = ECU_STATE_M_SHUT_DOWN;
		}
		break;
	case ECU_STATE_M_SHUT_DOWN:
		/* EcuM Switch Off*/
		EcuM_AL_SwitchOff();

	    /*Mcu Reset*/

		break;
	}
}
/*End of EcuM.c file*/

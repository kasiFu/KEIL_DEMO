#include "timer_hal.h"

/*******************************************************************************
 * User Include
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint16_t gs_1msCnt = 0u;
static uint16_t gs_100msCnt = 0u;

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMER_HAL_Init
 * Description   : This function initial this module.
 *
 * Implements : TIMER_HAL_Init_Activity
 *END**************************************************************************/
void TIMER_HAL_Init(void)
{
    TMA_InitTypeDef TIMA_InitStructure={};
	
	TIMA_InitStructure.TMA_Clk = TMA_FCLK_Div1;   //specify the operation clk of tim
	TIMA_InitStructure.TMA_Period = 64000;                 //specify the number of count clock 
	TIMA_InitStructure.TMA_Mode = TMA_Mode_Interval;      // timer mode
    TMA_Init(&TIMA_InitStructure);
    ISR_Register(TMA_IRQn, TIMER_HAL_1msPeriod);  //中断服务路径注册

    TMA_Start();
}

/*Timer 1ms period called*/
void TIMER_HAL_1msPeriod(void)
{
    uint16_t cntTmp = 0u;

    INTC_ClearPendingIRQ(TMA_IRQn);

    UDS_SystemTickCtl();    /* Add UDS timer in 1ms interrupt. */
    /*Just for check time overflow or not?*/
    cntTmp = gs_1msCnt + 1u;
    if(0u != cntTmp)
    {
        gs_1msCnt++;
    }

    cntTmp = gs_100msCnt + 1u;
    if(0u != cntTmp)
    {
        gs_100msCnt++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMER_HAL_Is1msTickTimeout
 * Description   : This function is check timeout. If timeout return TRUE, else return FALSE.
 *
 * Implements : Is1msTickTimeout_Activity
 *END**************************************************************************/
uint8_t  TIMER_HAL_Is1msTickTimeout(void)
{
    uint8_t result = FALSE;

    if(gs_1msCnt)
    {
        result = TRUE;

        gs_1msCnt--;
    }

    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMER_HAL_Is10msTickTimeout
 * Description   : This function is check timeout or not. If timeout return TRUE, else return FALSE.
 *
 * Implements : Is10msTickTimeout_Activity
 *END**************************************************************************/

uint8_t TIMER_HAL_Is100msTickTimeout(void)
{
    uint8_t result = FALSE;

    if(gs_100msCnt >= 100u)
    {
        result = TRUE;

        gs_100msCnt -= 100u;
    }

    return result;
}

/*get timer tick cnt for random seed.*/
uint32_t TIMER_HAL_GetTimerTickCnt(void)
{
	/*This two variables not init before used, because it used for generate random*/
	uint32_t hardwareTimerTickCnt;
	uint32_t timerTickCnt;
	
	timerTickCnt = ((hardwareTimerTickCnt & 0xFFFFu)) | (timerTickCnt << 16u);

	return timerTickCnt;
}

/******************************************************************************
 * EOF
 *****************************************************************************/




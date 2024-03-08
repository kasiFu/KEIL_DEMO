/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : tim_drv.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "tim_drv.h"
#include "uart_drv.h"
extern  unsigned char ledon;
uint32_t g_Int_Taken;


/***********************************************************************************************************************
* Function Name: Tim40_IntervalTimer
* @brief This function interval timer and generate interval interrupt.
* @param
* @return none
***********************************************************************************************************************/
void Tim40_IntervalTimer()
{
    TIM_InitTypeDef TIM_InitStructure = {0};

    TIM_InitStructure.TIM = TIM40;                   //specify the 8 channels TIM
    TIM_InitStructure.TIM_Channel = TTM_Channel_0;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div32768;// specify the operation clk=1khz of tim for 32Mhz fclk
    TIM_InitStructure.TIM_Period[0] = 200;               //specify the  time =20*(1/1000) =20ms
    TIM_InitStructure.TIM_Mode = TIM_Mode_Interval; // external event
    TIM_InitStructure.TIM_StartInt = TIM_StartInt_Disable;
    TIM_Init(&TIM_InitStructure);

    ISR_Register(TM00_IRQn, tim40_channel0_interrupt);
}

/***********************************************************************************************************************
* Function Name: tim41_channel0_interrupt
* @brief  TIM41 Channel interrupt service routine
* @param  msg
* @return None
***********************************************************************************************************************/

void tim40_channel0_interrupt(void *msg)
{
    INTC_ClearPendingIRQ(TM00_IRQn);    // clear INTTM00 interrupt flag

    UART2_RX_STA |= 1 << 15;                //强制标记接收完成

    TIM_Cmd(TIM40, TTM_Channel_0, Disable);
}


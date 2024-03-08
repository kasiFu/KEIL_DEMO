#include "intp_drv.h"
uint8_t flag = 0;


/***********************************************************************************************************************
* Function Name: Intp_Init
* @brief config gpio exit trigger-interrupt
* @param   :
* @return
***********************************************************************************************************************/
void Intp_Init(void)
{

    INTP_InitTypeDef INTP_InitStructure;

    INTP_InitStructure.INTP_Select  = INTP0 ;           //选择外部中断INTP0
    INTP_InitStructure.EXTI_Trigger = Trigger_Falling;  //设置外部中断，下降沿触发
    INTP_Init(&INTP_InitStructure);

    ISR_Register(INTP0_IRQn, intp0_interrupt);  //中断服务路径注册

    INTP_Start(INTP0);                  // Enable INTP0 Interrupt
}



void intp0_interrupt(void *msg)
{
    INTC_ClearPendingIRQ(INTP0_IRQn);     /* clear KEY interrupt flag */
    flag = 1;
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

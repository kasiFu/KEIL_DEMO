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

    INTP_InitStructure.INTP_Select  = INTP0 ;           //ѡ���ⲿ�ж�INTP0
    INTP_InitStructure.EXTI_Trigger = Trigger_Falling;  //�����ⲿ�жϣ��½��ش���
    INTP_Init(&INTP_InitStructure);

    ISR_Register(INTP0_IRQn, intp0_interrupt);  //�жϷ���·��ע��

    INTP_Start(INTP0);                  // Enable INTP0 Interrupt
}



void intp0_interrupt(void *msg)
{
    INTC_ClearPendingIRQ(INTP0_IRQn);     /* clear KEY interrupt flag */
    flag = 1;
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

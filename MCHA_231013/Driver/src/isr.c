#include "isr.h"
#include "Rte.h"
void IntDefaultHandler(void *pmsg);
volatile uint32_t g_ticks = 0xFFFFFFFFU;

ATE_FRAME_t pData; /*pData used for userdata as an example*/


device_isr_handler_t isrObj[TOTAL_IRQ_NUM]={
{0,NULL,IntDefaultHandler},/*  LVI_IRQn         !< 0  Low Voltage detection interrupt                                        */
{0,NULL,IntDefaultHandler},/*  INTP0_IRQn       !< 1  INTP0 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP1_IRQn       !< 2  INTP1 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP2_IRQn       !< 3  INTP2 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler}, /*  INTP3_IRQn       !< 4  INTP3 External interrupt request input is valid                       */
{0,NULL,IntDefaultHandler},/*  INTP4_IRQn       !< 5  INTP4 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP5_IRQn       !< 6  INTP5 External interrupt request input is valid                        */
{0,&pData,IntDefaultHandler},/*  ST2_IRQn /SPI20_IRQn/IIC20_IRQn !< 7  UART2 transmission transfer end or buffer empty       */
{0,&pData,IntDefaultHandler},/*  SR2_IRQn /SPI21_IRQn/IIC21_IRQn !< 8  UART2 rerception transfer                               */
{0,NULL,IntDefaultHandler},/*  SRE2_IRQn        !< 9  UART2 rerception communication error occurrence                        */
{0,&pData,IntDefaultHandler},/*  ST0_IRQn /SPI00_IRQn/IIC00_IRQn !< 10 UART0 transmission transfer end or buffer empty         */
{0,&pData,IntDefaultHandler},/*  SR0_IRQn  /SPI01_IRQn/IIC01_IRQn !< 11 UART0 rerception transfer                              */
{0,NULL,IntDefaultHandler},/*  SRE0_IRQn        !< 12 UART0 rerception communication error occurrence                        */
{0,&pData,IntDefaultHandler},/*  ST1_IRQn /SPI10_IRQn/IIC10_IRQn  !< 13 UART1 transmission transfer end or buffer empty        */
{0,&pData,IntDefaultHandler},/*  SR1_IRQn /SPI11_IRQn/IIC11_IRQn  !< 14 UART1 rerception transfer                              */
{0,NULL,IntDefaultHandler},/*  SRE1_IRQn        !< 15 UART1 rerception communication error occurrence                        */
{0,&pData,IntDefaultHandler},/*  IICA_IRQn        !< 16 IICA interrupt request                                                 */
{0,NULL,IntDefaultHandler},/*  TM00_IRQn        !< 17 TM4 channel 0 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  TM01_IRQn        !< 18 TM4 channel 1 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  TM02_IRQn        !< 19 TM4 channel 2 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  TM03_IRQn        !< 20 TM4 channel 3 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  ADC_IRQn         !< 21 ADC interrupt request                                                  */
{0,NULL,IntDefaultHandler},/*  RTC_IRQn         !< 22 Real-Time Clock interrupt request                                      */
{0,NULL,IntDefaultHandler},/*  KEY_IRQn         !< 23 KEY return interrupt request                                           */
{0,NULL,IntDefaultHandler},/*  CMP0_IRQn        !< 24 CMP0 interrupt request                                                 */
{0,NULL,IntDefaultHandler},/*  CMP1_IRQn        !< 25 CMP1 interrupt request                                                 */
{0,NULL,IntDefaultHandler},/*  TMA_IRQn         !< 26 TMA interrupt request                                                  */
{0,NULL,IntDefaultHandler},/*  TMM0_IRQn        !< 27 TMM channel 0 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  TMM1_IRQn        !< 28 TMM channel 1 interrupt request                                        */
{0,NULL,IntDefaultHandler},/*  TMB_IRQn         !< 29 TMB interrupt request                                                  */
{0,NULL,IntDefaultHandler},/*  TMC_IRQn         !< 30 TMC interrupt request                                                  */
{0,NULL,IntDefaultHandler},/*  FMC_IRQn         !< 31 Flash erase or write finish                                            */
{0,NULL,0},
{0,NULL,IntDefaultHandler},/*  INTP6_IRQn       !< 33 INTP6 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP7_IRQn       !< 34 INTP7 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP8_IRQn       !< 35 INTP8 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP9_IRQn       !< 36 INTP9 External interrupt request input is valid                        */
{0,NULL,IntDefaultHandler},/*  INTP10_IRQn      !< 37 INTP10 External interrupt request input is valid                       */
{0,NULL,IntDefaultHandler},/*  INTP11_IRQn      !< 38 INTP11 External interrupt request input is valid                       */
{0,NULL,0},
{0,NULL,0},
{0,NULL,IntDefaultHandler},/*  CAN0ERR_IRQn     !< 41 CAN0 error interrupt                                                   */
{0,NULL,0},
{0,NULL,0},
{0,NULL,IntDefaultHandler},/*  TM01H_IRQn       !< 44 TM4 channel 0 (8bit) interrupt request                                 */
{0,NULL,0},
{0,NULL,0},
{0,NULL,IntDefaultHandler},/*  TM03H_IRQn       !< 47 TM4 channel 3 (8bit) interrupt request                                 */
{0,NULL,IntDefaultHandler},/*  DIV_IRQn         !< 48 Hardware divider interrupt                                             */
{0,NULL,0},
{0,NULL,0},
{0,NULL,0},
{0,NULL,0},
{0,NULL,0},
{0,NULL,IntDefaultHandler},/*  IT_IRQn          !< 54 15-bit interval timer interrupt request                                */
{0,NULL,IntDefaultHandler},/*  CAN0REC_IRQn     !< 55 CAN0 reception completion interrupt                                    */
{0,NULL,IntDefaultHandler},/*  CAN0WUP_IRQn     !< 56 CAN0 wakeup interrupt                                                  */
{0,NULL,IntDefaultHandler},/*  CAN0TRX_IRQn     !< 57 CAN0 transmission completion interrupt                                 */
};

void IntDefaultHandler(void *pmsg)
{ 
	while(1) {}
}
/**
  * @brief  SysTick Handler Decreament the g_ticks value
  * @param  TM13 IRQ
  * @retval None
  */

void SysTick_Handler(void)
{
	g_ticks--;
	WDT->WDTE = 0xAC;
	Rte_IncrementCounter();
}



/**
  * @brief  IRQ00_Handler
  * @param  LVI IRQ
  * @retval None
  */
void IRQ00_Handler(void)
{
	isrObj[LVI_IRQn].isrHandler(isrObj[LVI_IRQn].pUserData);
}

/**
  * @brief  IRQ01_Handler
  * @param  INTP0 IRQ /INTP6_IRQn
  * @retval None
  */
void IRQ01_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP0_IRQn))
	{
		isrObj[INTP0_IRQn].isrHandler(isrObj[INTP0_IRQn].pUserData);
	}

	if(INTC_GetPendingIRQ(INTP6_IRQn))
	{
		isrObj[INTP6_IRQn].isrHandler(isrObj[INTP6_IRQn].pUserData);
	}
}

/**
  * @brief  IRQ02_Handler
  * @param  INTP1 IRQ /INTP7_IRQn
  * @retval None
  */
void IRQ02_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP1_IRQn))
	{
		isrObj[INTP1_IRQn].isrHandler(isrObj[INTP1_IRQn].pUserData);
	}

	if(INTC_GetPendingIRQ(INTP7_IRQn))
	{
		isrObj[INTP7_IRQn].isrHandler(isrObj[INTP7_IRQn].pUserData);
	}
}

/**
  * @brief  IRQ03_Handler
  * @param  INTP2 IRQ /INTP8_IRQn
  * @retval None
  */
void IRQ03_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP2_IRQn))
	{
		isrObj[INTP2_IRQn].isrHandler(isrObj[INTP2_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(INTP8_IRQn))
	{
		isrObj[INTP8_IRQn].isrHandler(isrObj[INTP8_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ04_Handler
  * @param  INTP3 IRQ /INTP9_IRQn
  * @retval None
  */
void IRQ04_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP3_IRQn))
	{
		isrObj[INTP3_IRQn].isrHandler(isrObj[INTP3_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(INTP9_IRQn))
	{
		isrObj[INTP9_IRQn].isrHandler(isrObj[INTP9_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ05_Handler
  * @param  INTP4_IRQn /INTP10_IRQn
  * @retval None
  */
void IRQ05_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP4_IRQn))
	{
		isrObj[INTP4_IRQn].isrHandler(isrObj[INTP4_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(INTP10_IRQn))
	{
		isrObj[INTP10_IRQn].isrHandler(isrObj[INTP10_IRQn].pUserData);
	}	

}

/**
  * @brief  IRQ06_Handler
  * @param  INTP5_IRQn /INTP11_IRQn
  * @retval None
  */
void IRQ06_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP5_IRQn))
	{
		isrObj[INTP5_IRQn].isrHandler(isrObj[INTP5_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(INTP11_IRQn))
	{
		isrObj[INTP11_IRQn].isrHandler(isrObj[INTP11_IRQn].pUserData);
	}
}

/**
  * @brief  IRQ07_Handler
  * @param  ST2/SPI20/IIC20 IRQ 
  * @retval None
  */
void IRQ07_Handler(void)
{
	isrObj[7].isrHandler(isrObj[7].pUserData);
}

/**
  * @brief  IRQ08_Handler
  * @param  SR2/SPI21/IIC21 IRQ
  * @retval None
  */
void IRQ08_Handler(void)
{	
	isrObj[8].isrHandler(isrObj[8].pUserData);
}

/**
  * @brief  IRQ09_Handler
  * @param  SRE2 IRQ /CAN0ERR_IRQn
  * @retval None
  */
void IRQ09_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE2_IRQn))
	{
		isrObj[SRE2_IRQn].isrHandler(isrObj[SRE2_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(CAN0ERR_IRQn))
	{
		isrObj[CAN0ERR_IRQn].isrHandler(isrObj[CAN0ERR_IRQn].pUserData);
	}
}

/**
  * @brief  IRQ10_Handler
  * @param  ST0/SPI00/IIC00 IRQ  
  * @note   the num of ST0_IRQn is same as SPI00_IRQn and IIC00_IRQ, so SPI00_IRQn or IIC00_IRQ
  *			interrupt quote this function handler IRQ10_Handler
  * @retval None
  */
void IRQ10_Handler(void)
{
	isrObj[10].isrHandler(isrObj[10].pUserData);
}

/**
  * @brief  IRQ11_Handler
  * @param  SR0/SPI01/IIC01 IRQ 
  * @note   the num of SR0_IRQn is same as SPI01_IRQn and IIC01_IRQ, so SPI01_IRQn or IIC01_IRQ
  *			interrupt quote this function handler IRQ11_Handler
  * @retval None
  */
void IRQ11_Handler(void)
{
	isrObj[11].isrHandler(isrObj[11].pUserData);
}

/**
  * @brief  IRQ12_Handler
  * @param   SRE0 or TM01H IRQ 
  * @note   the num of SRE0_IRQn is same as TM01H_IRQn, so TM01H_IRQn 
  *			interrupt quotes this function handler IRQ12_Handler
  * @retval None
  */
void IRQ12_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE0_IRQn))
	{
		isrObj[SRE0_IRQn].isrHandler(isrObj[SRE0_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(TM01H_IRQn))
	{
		isrObj[TM01H_IRQn].isrHandler(isrObj[TM01H_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ13_Handler
  * @param  ST1/SPI10/IIC10 IRQ  
  * @note   the num of ST1_IRQn is same as SPI10_IRQn and IIC10_IRQ, so SPI10_IRQn or IIC10_IRQ
  *			interrupt quote this function handler IRQ13_Handler
  * @retval None
  */
void IRQ13_Handler(void)
{
	isrObj[13].isrHandler(isrObj[13].pUserData);
}

/**
  * @brief  IRQ14_Handler
  * @param  SR1/SPI11/IIC11 IRQ
  * @note   the num of SR1_IRQn is same as SPI11_IRQn and IIC11_IRQ, so SPI11_IRQn or IIC11_IRQ
  *			interrupt quote this function handler IRQ14_Handler
  * @retval None
  */
void IRQ14_Handler(void)
{
	isrObj[14].isrHandler(isrObj[14].pUserData);
}

/**
  * @brief  IRQ15_Handler
  * @param  SRE1 IRQ /TM03H_IRQn
  * @note   the num of SRE1_IRQn is same as TM03H_IRQn, so TM03H_IRQn 
  *			interrupt quotes this function handler IRQ15_Handler
  * @retval None
  */
void IRQ15_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE1_IRQn))
	{
		isrObj[SRE1_IRQn].isrHandler(isrObj[SRE1_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(TM03H_IRQn))
	{
		isrObj[TM03H_IRQn].isrHandler(isrObj[TM03H_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ16_Handler
  * @param  IICA IRQ
  * @note   the num of IICA_IRQn is same as DIV_IRQn, so DIV_IRQn 
  *			interrupt quotes this function handler IRQ16_Handler
  * @retval None
  */
void IRQ16_Handler(void)
{
	if(INTC_GetPendingIRQ(IICA_IRQn))
	{
		isrObj[IICA_IRQn].isrHandler(isrObj[IICA_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(DIV_IRQn))
	{
		isrObj[DIV_IRQn].isrHandler(isrObj[DIV_IRQn].pUserData);
	}
}

/**
  * @brief  IRQ17_Handler
  * @param  TM00 IRQ
  * @retval None
  */
void IRQ17_Handler(void)
{
	isrObj[17].isrHandler(isrObj[17].pUserData);
}

/**
  * @brief  IRQ18_Handler
  * @param  TM01 IRQ
  * @retval None
  */
void IRQ18_Handler(void)
{
	isrObj[18].isrHandler(isrObj[18].pUserData);
}

/**
  * @brief  IRQ19_Handler
  * @param  TM02 IRQ
  * @retval None
  */
void IRQ19_Handler(void)
{
	isrObj[19].isrHandler(isrObj[19].pUserData);
}

/**
  * @brief  IRQ20_Handler
  * @param  TM03 IRQ
  * @retval None
  */
void IRQ20_Handler(void)
{
	isrObj[20].isrHandler(isrObj[20].pUserData);
}

/**
  * @brief  IRQ21_Handler
  * @param  ADC IRQ
  * @retval None
  */
void IRQ21_Handler(void)
{
	isrObj[21].isrHandler(isrObj[21].pUserData);
}

/**
  * @brief  IRQ22_Handler
  * @param  RTC IRQ /IT_IRQn
  * @retval None
  */
void IRQ22_Handler(void)
{
	if(INTC_GetPendingIRQ(RTC_IRQn))
	{
		isrObj[RTC_IRQn].isrHandler(isrObj[RTC_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(IT_IRQn))
	{
		isrObj[IT_IRQn].isrHandler(isrObj[IT_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ23_Handler
  * @param  KEY_IRQn /CAN0REC_IRQn
  * @retval None
  */
void IRQ23_Handler(void)
{
	if(INTC_GetPendingIRQ(KEY_IRQn))
	{
		isrObj[KEY_IRQn].isrHandler(isrObj[KEY_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(CAN0REC_IRQn))
	{
		isrObj[CAN0REC_IRQn].isrHandler(isrObj[CAN0REC_IRQn].pUserData);
	}	
}

/**
  * @brief  IRQ24_Handler
  * @param  CMP0_IRQn /CAN0WUP_IRQn
  * @retval None
  */
void IRQ24_Handler(void)
{
	if(INTC_GetPendingIRQ(CMP0_IRQn))
	{
		isrObj[CMP0_IRQn].isrHandler(isrObj[CMP0_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(CAN0WUP_IRQn))
	{
		isrObj[CAN0WUP_IRQn].isrHandler(isrObj[CAN0WUP_IRQn].pUserData);
	}
}

void IRQ25_Handler(void)
{
	if(INTC_GetPendingIRQ(CMP1_IRQn))
	{
		isrObj[CMP1_IRQn].isrHandler(isrObj[CMP1_IRQn].pUserData);
	}
	
	if(INTC_GetPendingIRQ(CAN0TRX_IRQn))
	{
		isrObj[CAN0TRX_IRQn].isrHandler(isrObj[CAN0TRX_IRQn].pUserData);
	}
}

void IRQ26_Handler(void)
{
	isrObj[26].isrHandler(isrObj[26].pUserData);
}
/**
  * @brief  IRQ27_Handler
  * @param  TMM0 IRQ
  * @retval None
  */
void IRQ27_Handler(void)
{
	isrObj[27].isrHandler(isrObj[27].pUserData);
}

/**
  * @brief  IRQ28_Handler
  * @param  TM13 IRQ
  * @retval None
  */
void IRQ28_Handler(void)
{
	isrObj[28].isrHandler(isrObj[28].pUserData);
}

/**
  * @brief  IRQ29_Handler
  * @param  TM12 IRQ
  * @retval None
  */
void IRQ29_Handler(void)
{
	isrObj[29].isrHandler(isrObj[29].pUserData);
}

/**
  * @brief  IRQ30_Handler
  * @param  TM13 IRQ
  * @retval None
  */
void IRQ30_Handler(void)
{
	isrObj[30].isrHandler(isrObj[30].pUserData);
}

/**
  * @brief  IRQ31_Handler
  * @param  FMC IRQ 
  * @retval None
  */
void IRQ31_Handler(void)
{
	isrObj[31].isrHandler(isrObj[31].pUserData);
}

/**
  * @brief  register interrupt function according to  IRQn num.
  * @param  irq_num: it relates to irq handler 
  * @param	interrupt:interrupt server function
  * @retval regiseter status 
  */
int ISR_Register(IRQn_Type irq_num, void *interrupt)
{
	int res = ISR_SUCC;

	if(isrObj[irq_num].IRQ_Flag)
	{
		return ISR_ERR;
	}
	else
	{		
		isrObj[irq_num].isrHandler = (isr_handler_t)interrupt;
		if(isrObj[irq_num].isrHandler == NULL)
		{
			return ISR_ERR;
		}
		isrObj[irq_num].IRQ_Flag = 1;
		INTC_ClearPendingIRQ(irq_num); // clear  interrupt flag 
		NVIC_ClearPendingIRQ(irq_num); // clear  interrupt flag 
		INTC_EnableIRQ(irq_num);
	}

	return res;
}

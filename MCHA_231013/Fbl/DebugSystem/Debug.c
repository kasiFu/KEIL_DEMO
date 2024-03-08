#include "debug.h"

void DEBUG_Init(void)
{
    PORT->P7    = 0x02U;
	PORT->PM7  &= ~(3<<1); // P71/LED, P72/LED
    
    PORT->PM2  &= ~(0xff<<0);  // P20/P21/P22/P23/P24/P25/P26/P27
	PORT->PMC2  &= ~(0xff<<0); // P20/P21/P22/P23/P24/P25/P26/P27

    SystemCoreClockUpdate();
    #ifdef DEBUG_PRINTF
	if(Uart1_Init(115200) == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
    #endif	
}

void DEBUG_IO_Deinit(void)
{
    
}

void DEBUG_IO_SetDebugIOLow(void)
{
         

}

void DEBUG_IO_SetDebugIOHigh(void)
{
	
}

void DEBUG_IO_ToggleDebugIO(void)
{
	PORT->P7 ^= (1<<1);     // Toggle P71
    PORT->P7 ^= (1<<2); 	// Toggle P72
}

/***********************************************************************************************************************
* Function Name: HardFault_Handler
* Description  : Assembly wrapper using Embedded Assembler in Keil MDK
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// Hard Fault handler wrapper in assembly
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also extract the LR value as second
// parameter.
void HardFault_Handler(void)
{
    while(1);
}

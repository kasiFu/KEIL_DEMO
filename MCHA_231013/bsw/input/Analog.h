#ifndef __ANALOG_H__
#define __ANALOG_H__ 1

#include "adc.h"
#include "Std_Types.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//void IRQ21_Handler(void) __attribute__((alias("adc_interrupt")));

/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

#define ANALOG_BUFFER_SIZE (uint16)6u /**/
#define ANALOG_CHANNEL_MAX (uint16)3U

#define ANALOG_KEY_IO_AD 12    /*12*/
#define ANALOG_MOTOR_CIR_AD 14 /*14*/
#define ANALOG_BAT_DEC_AD 8    /*8*/

typedef struct
{
	uint8 count;
	uint16 avgVal;
    uint16 buffer[ANALOG_BUFFER_SIZE];
}Analog_channelData_type;

typedef struct 
{
    uint8 number; /*ad channel number*/
    ADC_Channel_t channel;
    Analog_channelData_type  channelArr[ANALOG_CHANNEL_MAX];
}Analog_handle_type;



extern void Analog_Task1ms(void);
extern uint16 Analog_getValue(ADC_Channel_t Channel);

#endif

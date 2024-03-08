

#include "adc.h"
#include "Analog.h"

Analog_handle_type Analog_handle = {
    .number = 0,
    .channel = (ADC_Channel_t)ANALOG_KEY_IO_AD,
    .channelArr[0].count = 0,
    .channelArr[0].avgVal = 0,
    .channelArr[0].buffer = {0,0,0,0,0,0},

    .channelArr[1].count = 0,
    .channelArr[1].avgVal = 0,
    .channelArr[1].buffer = {0,0,0,0,0,0},

    .channelArr[2].count = 0,
    .channelArr[2].avgVal = 0,
    .channelArr[2].buffer = {0,0,0,0,0,0}
};

const ADC_Channel_t channelNumArr[] =
    {
        ADC_Channel_12, /*ANALOG_KEY_IO_AD*/
        ADC_Channel_14, /*ANALOG_MOTOR_CIR_AD*/
        ADC_Channel_8}; /*ANALOG_BAT_DEC_AD*/

void Analog_Task1ms(void) /*2ms*/
{
    uint8 i = 0;
    uint8 counterTmp = 0;
    uint16 buffer[2] = {0};
    uint16 result = 0;
    uint32 sumTmp = 0;
    uint16 maxValue = 0;
    uint16 minValue = 4095;
    /*channel 12*/
    Analog_handle.channel = channelNumArr[Analog_handle.number]; /*obtain channel number*/
    result = ADC_Converse(Analog_handle.channel, 2, buffer);
    ADC_Stop();
    counterTmp = Analog_handle.channelArr[Analog_handle.number].count;
    Analog_handle.channelArr[Analog_handle.number].buffer[counterTmp] = result;
    Analog_handle.channelArr[Analog_handle.number].count++;

    if (Analog_handle.channelArr[Analog_handle.number].count >= ANALOG_BUFFER_SIZE)
    {
        Analog_handle.channelArr[Analog_handle.number].count = 0;

        for (i = 0; i < ANALOG_BUFFER_SIZE; i++) /*obtain 6 data, delete max,min,and average*/
        {
            if (maxValue < Analog_handle.channelArr[Analog_handle.number].buffer[i])
            {
                maxValue = Analog_handle.channelArr[Analog_handle.number].buffer[i];
            }
            if (minValue > Analog_handle.channelArr[Analog_handle.number].buffer[i])
            {
                minValue = Analog_handle.channelArr[Analog_handle.number].buffer[i];
            }
            sumTmp += Analog_handle.channelArr[Analog_handle.number].buffer[i];
        }
        sumTmp = sumTmp - maxValue - minValue;
        Analog_handle.channelArr[Analog_handle.number].avgVal = 0x0FFF & (sumTmp >> 2);

        Analog_handle.number++;
        if (Analog_handle.number >= ANALOG_CHANNEL_MAX)
        {
            Analog_handle.number = 0;
        }
    }
}

/*****************************************************************************
 *
 *   Function          :   Analog_getValue
 *
 *   Description       :  get Ad value
 *
 *   Parameters(in)    :   AD channel value, only 12,14,8
 *
 *   Parameters(in-out):   None
 *
 *   Parameters(out)   :   None
 *
 *   Return Value      :   AD value
 *
 *   Notes             :
 *
 *****************************************************************************/
uint16 Analog_getValue(ADC_Channel_t Channel)
{
    uint16 Advalue =  0;

    if(Channel == ADC_Channel_12)
    {
        Advalue = Analog_handle.channelArr[0].avgVal;
    }
    else if(Channel == ADC_Channel_14)
    {
        Advalue = Analog_handle.channelArr[1].avgVal;
    }
    else if(Channel == ADC_Channel_8)
    {
        Advalue = Analog_handle.channelArr[2].avgVal;
    }
    else
    {
        Advalue =  0;
    }

    return Advalue;
}

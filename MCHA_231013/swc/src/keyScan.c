
#include "keyScan.h"


#define KEYSCAN_BUTTON_ANALOG_LOW 	    0X300
#define KEYSCAN_BUTTON_ANALOG_HIGH 		0XF00 	/*0*/

#define KEYSCAN_UP_JITTER_TIME      2 /*10ms/cycle*2*/
#define KEYSCAN_DOWN_JITTER_TIME      2 /*10ms/cycle*2*/

keyScan_cluster_t keyScan_signal_table[KEYSCAN_INDEX_MAX] =
{
     {KEYSCAN_KEY,STD_LOW,STD_LOW,0,0,0,FALSE,FALSE},
     {KEYSCAN_KEY,STD_LOW,STD_LOW,0,0,0,FALSE,FALSE},
     {KEYSCAN_KEY,STD_LOW,STD_LOW,0,0,0,FALSE,FALSE},
     {KEYSCAN_KEY,STD_LOW,STD_LOW,0,0,0,FALSE,FALSE},
     {KEYSCAN_KEY,STD_LOW,STD_LOW,0,0,0,FALSE,FALSE}
};

static void keyScan_InputProcess(void) /*4个key input process*/
{
     uint8 val = 0;
     uint16 advalue = 0;
     uint16 index = 0;
     uint16 gpioPinTmp = 0;

     for (index = 0; index < KEYSCAN_INDEX_MAX; index++)
     {
          gpioPinTmp = 1u << index;

          if (index == 0)
          {
               advalue = Analog_getValue((ADC_Channel_t)ANALOG_KEY_IO_AD);
               if (advalue < KEYSCAN_BUTTON_ANALOG_LOW)
               {
                    val = STD_LOW;
               }
               else if (advalue > KEYSCAN_BUTTON_ANALOG_HIGH)
               {
                    val = STD_HIGH;
               }
          }
          else
          {
               val = GPIO_ReadInputDataBit(GPIO_PORT12, gpioPinTmp);
          }

          if (val == STD_LOW) /*按下有效*/
          {
               keyScan_signal_table[index].upCount++;
               if (keyScan_signal_table[index].upCount > KEYSCAN_UP_JITTER_TIME)
               {
                    keyScan_signal_table[index].upCount = 0;
                    keyScan_signal_table[index].valCurrent = STD_HIGH;

                    if(keyScan_signal_table[index].val_shadow != keyScan_signal_table[index].valCurrent)
                    {
                         keyScan_signal_table[index].val_shadow = keyScan_signal_table[index].valCurrent;
                         keyScan_signal_table[index].upEdge = TRUE;
                    }
                    else
                    {
                         keyScan_signal_table[index].val = keyScan_signal_table[index].val_shadow;

                    }
               }
               keyScan_signal_table[index].downCount = 0;

          }
          else
          {
               keyScan_signal_table[index].downCount++;
               if (keyScan_signal_table[index].downCount > KEYSCAN_DOWN_JITTER_TIME)
               {
                    keyScan_signal_table[index].downCount = 0;
                    keyScan_signal_table[index].valCurrent = STD_LOW;

                  if(keyScan_signal_table[index].val_shadow != keyScan_signal_table[index].valCurrent)
                    {
                         keyScan_signal_table[index].val_shadow = keyScan_signal_table[index].valCurrent;
                         keyScan_signal_table[index].downEdge = TRUE;
                    }
                    else
                    {
                         keyScan_signal_table[index].val = keyScan_signal_table[index].val_shadow;
                    }
               }
               keyScan_signal_table[index].upCount = 0;
          }
     }
}


/*********************************************************************************/

void keyScan_Task10ms(void)
{
     keyScan_InputProcess();
}

uint8 keyScan_get_value(keyScan_index_e_type index)
{
	return keyScan_signal_table[index].val;
}

boolean keyScan_get_up_edge(keyScan_index_e_type index)
{
	boolean ret = FALSE;

	ret = keyScan_signal_table[index].upEdge;

	if (ret)
	{
		keyScan_signal_table[index].upEdge = FALSE;
	}

	return ret;
}

boolean keyScan_get_down_edge(keyScan_index_e_type index)
{
	boolean ret = FALSE;

	ret = keyScan_signal_table[index].downEdge;

	if (ret)
	{
		keyScan_signal_table[index].downEdge = FALSE;
	}

	return ret;
}



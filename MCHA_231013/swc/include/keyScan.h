#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__  1

#include "adc.h"
#include "Std_Types.h"
#include "Analog.h"
#include "gpio.h"

typedef enum
{
	KEYSCAN_KEY = 0,
	KEYSCAN_UP_IO,
	KEYSCAN_DOWN_IO,
	KEYSCAN_FORNT_IO,
	KEYSCAN_REAR_IO,

	KEYSCAN_INDEX_MAX
} keyScan_index_e_type;

typedef struct
{
	keyScan_index_e_type index;
	uint8 valCurrent;
	uint8 val;
    uint8 upCount;
    uint8 downCount;
	uint8 val_shadow;
	boolean upEdge;
	boolean downEdge;
}keyScan_cluster_t;

typedef uint16 (*func_keyscan_t)(GPIO_Port_t, uint16_t);

extern keyScan_cluster_t keyScan_signal_table[KEYSCAN_INDEX_MAX];


extern void keyScan_Task10ms(void);
extern uint8 keyScan_get_value(keyScan_index_e_type index);
extern boolean keyScan_get_up_edge(keyScan_index_e_type index);
extern boolean keyScan_get_down_edge(keyScan_index_e_type index);

#endif







#ifndef _ISR_H__
#define _ISR_H__
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"


#define	ISR_SUCC   0
#define	ISR_ERR    1
#define TOTAL_IRQ_NUM	  58

typedef void(*isr_handler_t)(void *apUserData);




typedef enum
{
	INT_IDLE = 0,
	INT_RX = 1,
	INT_TX = 2,
	INT_DMA =3,
}DATA_DIR_t;

typedef struct 
{
	DATA_DIR_t flag;
	uint32_t len;
	volatile uint8_t *data;
}ATE_FRAME_t;



/** @defgroup device_isr_handler_t 
  * @{
  */ 
typedef struct
{	
	bool IRQ_Flag;
	void *pUserData;  
	isr_handler_t isrHandler;
}device_isr_handler_t;


extern ATE_FRAME_t pData;
int ISR_Register(IRQn_Type irq_num, void *interrupt);


#endif

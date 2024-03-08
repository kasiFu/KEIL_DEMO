#ifndef __DMA_DRV_H__
#define __DMA_DRV_H__
#include <stdint.h>
#include "dma.h"
#define DMA_TRANS_TIMES     DMAVEC->CTRL[CTRL_DATA_UART0].DMACT

extern unsigned char CTRL_DATA_UART0;
void DMA_Uart0_Tx(DMA_VECTOR_t dma_vector, DMA_Mode_t mode, void *src_adr, void *dst_adr, uint16_t count);
void DMA_Uart0_Rx(DMA_VECTOR_t dma_vector, DMA_Mode_t mode, void *src_adr, void *dst_adr, uint16_t count);


#endif

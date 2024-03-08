#ifndef __UART_DRV_H__
#define __UART_DRV_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "gpio.h"
#include "isr.h"
#define UART0_DMA_RCV
#define UART_MAX_RECV_LEN       64

extern unsigned char UART1_RX_BUF[UART_MAX_RECV_LEN];
extern unsigned short UART1_RX_STA ;
extern unsigned char UART0_RX_BUF[UART_MAX_RECV_LEN];
extern unsigned short UART0_RX_STA ;
extern unsigned char UART2_RX_BUF[UART_MAX_RECV_LEN];
extern unsigned short UART2_RX_STA ;
extern unsigned char UART3_RX_BUF[UART_MAX_RECV_LEN];
extern unsigned short UART3_RX_STA ;

int8_t Uart0_Init(uint32_t bound);
int8_t Uart1_Init(uint32_t bound);
int8_t Uart2_Init(uint32_t bound);


void Uart0_Send(uint8_t ch);
char Uart0_Receive(void);

void Uart0_IntSend(uint8_t *tx_buf, uint16_t tx_num);
void uart0_interrupt_send(void *msg);
void uart0_interrupt_receive(void *msg);
uint8_t Uart0_Dma_Rcv(uint8_t *buf);

void Uart1_IntSend(uint8_t *tx_buf, uint16_t tx_num);
void uart1_interrupt_send(void *msg);
int  Uart1_IntRcv(uint8_t *rx_buf);
void uart1_interrupt_receive(void *msg);

void Uart2_IntSend(uint8_t *tx_buf, uint16_t tx_num);
void uart2_interrupt_send(void *msg);
void uart2_interrupt_receive(void *msg);

void Uart3_IntSend(uint8_t *tx_buf, uint16_t tx_num);
void uart3_interrupt_send(void *msg);

void Uart0_Dma_Send(uint8_t *tx_buf, uint16_t tx_num);

#if defined( BAT32G1XX_100PIN) || defined( BAT32G1XX_80PIN)
int8_t Uart3_Init(uint32_t bound);
void uart3_interrupt_receive(void *msg);
#endif
#endif

#ifndef __LIN_PROTOCOL_H__
#define __LIN_PROTOCOL_H__
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
#include "cgc.h"
#include "tim.h"
#include "isr.h"
#include "uart.h"
#include "uds_lin_tp.h"
#include "BootCfg.h"
#include "intp.h"

#define UART_PRS         (SCI0->SCI.SPS0 & 0x0FU)
#define LIN_SLEEP_HIGH  GPIO_SetBits(GPIO_PORT3,GPIO_Pin_0)
#define LIN_SLEEP_LOW   GPIO_ResetBits(GPIO_PORT3,GPIO_Pin_0)


/**************************************************************************
* Typedef defination
**************************************************************************/
typedef struct
{
	uint8_t sync;		//laidi 把同步段0x55也通过UART接收
    union {
        uint8_t PID;
        struct {
            uint8_t ID    : 6;
            uint8_t P0    : 1;  /*  odd parity: P0 =  (ID0 ^ ID1 ^ ID2 ^ ID4) */
            uint8_t P1    : 1;  /* even parity: P1 = ~(ID1 ^ ID3 ^ ID4 ^ ID5) */
        } PID_b;
    };
    uint8_t Data[8];            /* data field: 1 ~ 8 bytes */
    uint8_t ChkSum;             /* check sum field */
} lin_frame_t;

typedef enum
{
	LIN_IDLE,
	WAKE,
	BREAK,
	LIN_RX,
	LIN_TX
} lin_state_t;

#define LIN_MASTER_REQUEST_ID   0X3C
#define LIN_SLAVER_RESPOND_ID   0X3D

extern lin_frame_t txFrame, rxFrame;
extern lin_state_t lin_state;
extern uint8_t lin_check_timeout;
extern uint32_t break_timeout, rx_timeout;
extern volatile uint8_t g_uart_tx_end, g_uart_rx_end;
extern bool ReallyTxBuffFlag;//数据链路层通讯使用的发送标志，表示数据传输完毕从机节点可以发送应答了

void LIN_SetConfig(void);
void LIN_SleepIO_Init(void);

void tim40_channel3_interrupt(void *msg);
void uart0_lin_receive(void *msg);
void uart0_lin_send(void *msg);
void LIN_ReceiveBreak(void);
uint8_t Lin_Checksum(uint8_t id , uint8_t data[]);
void LinMainFun(void);
#endif

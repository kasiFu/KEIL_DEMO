/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : uart_drv.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "uart_drv.h"
#include "tim_drv.h"
#include "dma_drv.h"

unsigned char UART0_RX_BUF[UART_MAX_RECV_LEN];
unsigned short UART0_RX_STA = 0;
unsigned char UART1_RX_BUF[UART_MAX_RECV_LEN];
unsigned short UART1_RX_STA = 0;
unsigned char UART2_RX_BUF[UART_MAX_RECV_LEN];
unsigned short UART2_RX_STA = 0;
unsigned char UART3_RX_BUF[UART_MAX_RECV_LEN];
unsigned short UART3_RX_STA = 0;



/******************************************************************************
* Function Name: Uart0_Init
* @brief  UART0 init drv
* @param  bound
* @return init status
******************************************************************************/
int8_t Uart0_Init(uint32_t bound)
{
    int8_t ret;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_1, GPIO_P51, GROUP_AF_ODEFAULT);
    GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_0, GPIO_P50, GROUP_AF_ODEFAULT);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;	
    GPIO_InitStruct.GPIO_Ctrl  = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0 ;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);

    /*USART CONFIG*/
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
    UART_InitStructure.phase = UART_Phase_Normal;
    UART_InitStructure.bitorder = UART_Bit_LSB;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //收发模式

    ret = UART_Init(UART0, &UART_InitStructure); //初始化串口

    if (ret)
    {
        SCI_ERROR_LOG(ret);
        return ret;
    }

    return SCI_SUCCESS;
}
/******************************************************************************
* Function Name: uart1_init
* @brief  UART1 init
* @param  bound
* @return error or success
*******************************************************************************/
int8_t Uart1_Init(uint32_t bound)
{
    int8_t res ;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    UART_InitTypeDef UART_InitStructure = {0};

#ifndef  BAT32G1XX_48PIN	
    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_2, GPIO_P02, GROUP_AF_ODEFAULT);
    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_3, GPIO_P03, GROUP_AF_ODEFAULT);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;	
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);
#else 
    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_0, GPIO_P00, GROUP_AF_ODEFAULT);
    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_1, GPIO_P01, GROUP_AF_ODEFAULT);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;	
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);	
#endif
    /*USART CONFIG*/
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
    UART_InitStructure.phase = UART_Phase_Normal;
    UART_InitStructure.bitorder = UART_Bit_LSB;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //收发模式

    res = UART_Init(UART1, &UART_InitStructure); //初始化串口

    if (res)
    {
        SCI_ERROR_LOG(res);
        return res;
    }

    ISR_Register(ST1_IRQn, uart1_interrupt_send);     //串口1发送中断服务路径注册
    ISR_Register(SR1_IRQn, uart1_interrupt_receive);  //串口1接收中断服务路径注册

    return SCI_SUCCESS;
}

/******************************************************************************
* Function Name: uart2_init
* @brief  UART2 init
* @param  bound
* @return error or success
******************************************************************************/
int8_t Uart2_Init(uint32_t bound)
{
    int8_t res ;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_3, GPIO_P13, GROUP_AF_ODEFAULT);
    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_4, GPIO_P14, GROUP_AF_ODEFAULT);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;		
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    /*USART CONFIG*/
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
    UART_InitStructure.phase = UART_Phase_Normal;
    UART_InitStructure.bitorder = UART_Bit_LSB;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //收发模式

    res = UART_Init(UART2, &UART_InitStructure); //初始化串口

    if (res)
    {
        SCI_ERROR_LOG(res);
        return res;
    }

    ISR_Register(ST2_IRQn, uart2_interrupt_send);     //串口2发送中断服务路径注册
    ISR_Register(SR2_IRQn, uart2_interrupt_receive);  //串口2接收中断服务路径注册


    Tim40_IntervalTimer();               //定时器20ms
    TIM_Cmd(TIM40, TTM_Channel_0, Disable); //关闭定时器
    return SCI_SUCCESS;
}


/******************************************************************************
* Function Name: Uart0_Send
* @brief  UART0 Send data
* @param  None
* @return None
*******************************************************************************/
void Uart0_Send(uint8_t ch)
{
    UART_SendByte(UART0, ch);
}
/*****************************************************************************
* Function Name: Uart0_Receive
* @brief  UART0 receive data
* @param  None
* @return rx data
*****************************************************************************/
char Uart0_Receive(void)
{
    return UART_ReceiveByte(UART0);
}

/*****************************************************************************
* Function Name: Uart0_IntSend
* @brief  UART0 Send data by interrupt
* @param  None
* @return None
*****************************************************************************/
void Uart0_IntSend(uint8_t *tx_buf, uint16_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST0_IRQn);
}

/****************************************************************************
* Function Name: uart0_dma_send
* @brief  UART0 Send interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void Uart0_Dma_Send(uint8_t *tx_buf, uint16_t tx_num)
{
    DMA_Uart0_Tx(DMA_VECTOR_ST0, DMA_Mode_Normal, (void *)tx_buf, (void *)&UART0_TX, tx_num); //config dma transmission
    DMA_Trigger(DMA_VECTOR_ST0);
}
/****************************************************************************
* Function Name: uart0_interrupt_send
* @brief  UART0 Send interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void uart0_interrupt_send(void *msg)
{
    ATE_FRAME_t *pFrame = (ATE_FRAME_t *)msg;
    INTC_ClearPendingIRQ(ST0_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART0_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else //send finished
    {
    }
}

/****************************************************************************
* Function Name: Uart0_Dma_Rcv
* @brief  UART0 dma receive data using ringr buffer
* @param  buf
* @return read buff length
*****************************************************************************/
uint8_t Uart0_Dma_Rcv(uint8_t *buf)
{
    static uint16_t read = 0, write = 0;
    static uint16_t raw_len = UART_MAX_RECV_LEN;
    int16_t readlen = 0;

    if (buf == NULL)
    {
        return 0;
    }

    write = raw_len - DMA_TRANS_TIMES;  //写指针位置
    readlen = write - read;                   //读数据长度

    if (!readlen)
    {
        return 0;
    }

    if (readlen  < 0)
    {
        readlen += raw_len;
    }

    if (readlen < (raw_len - read))     //无需折返
    {
        memcpy(buf, &UART0_RX_BUF[read], readlen);
    }
    else
    {
        memcpy(buf, &UART0_RX_BUF[read], raw_len - read);
        memcpy(&buf[raw_len - read], UART0_RX_BUF, readlen - (raw_len - read));
    }

    read = (readlen + read) % raw_len; //更新读指针
    return readlen;
}
/*****************************************************************************
* Function Name: uart_callback_error
* @brief
* @param  None
* @return None
******************************************************************************/
void uart_callback_error(uint8_t err_type)
{
    //user edit here when appear error
}


/****************************************************************************
* Function Name: Uart1_IntSend
* @brief  UART1 Send data by interrupt
* @param  None
* @return None
****************************************************************************/
void Uart1_IntSend(uint8_t *tx_buf, uint16_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST1_IRQn);
}
/***************************************************************************
* Function Name: uart1_interrupt_send
* @brief  UART1 Send interrupt service routine
* @param  None
* @return None
***************************************************************************/
void uart1_interrupt_send(void *msg)
{
    ATE_FRAME_t *pFrame = (ATE_FRAME_t *)msg;

    INTC_ClearPendingIRQ(ST1_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART1_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else  //send finished
    {
    }
}
/**************************************************************************
* Function Name: Uart1_IntRcv
* @brief  UART1 Receive data by interrupt
* @param  None
* @return rx_num
**************************************************************************/
int  Uart1_IntRcv(uint8_t *rx_buf)
{
    uint32_t rx_num = 0;

    if ((UART1_RX_STA & 0x8000U) == 1)
    {
        rx_num = UART1_RX_STA & 0x3fff;
        memcpy(rx_buf, UART1_RX_BUF, rx_num);
        return rx_num;
    }

    return 0;
}
/************************************************************************
* Function Name: uart1_interrupt_receive
* @brief  UART1 Receive interrupt service routine
* @param  None
* @return None
*************************************************************************/
void uart1_interrupt_receive(void *msg)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;

    INTC_ClearPendingIRQ(SR1_IRQn);
    err_type = UART_GetErrStaus(UART1, UART_FLAG_FEF | UART_FLAG_PEF | UART_FLAG_OVF);

    if (err_type)
    {
        uart_callback_error(err_type);
    }

    rx_data = UART1_RX;

    if ((UART1_RX_STA & 0x8000U) == 0) //接收未完成
    {
        if (UART1_RX_STA & 0x4000U) //接收到0x0d
        {
            if (rx_data != 0x0a)
			{
				UART0_RX_BUF[UART0_RX_STA & 0x3fff] = rx_data;
                UART0_RX_STA++;				
			}
            else
            {
                UART1_RX_STA |= 0x8000;
                UART1_RX_BUF[UART1_RX_STA & 0x3fff] = rx_data;
                UART1_RX_STA ++;
            }
        }
        else                     //还未接收到0x0d
        {
            if (rx_data == 0x0d)
            {
                UART1_RX_STA |= 0x4000;
                UART1_RX_BUF[UART1_RX_STA & 0x3fff] = rx_data;
                UART1_RX_STA ++;
            }
            else
            {
                UART1_RX_BUF[UART1_RX_STA & 0x3fff] = rx_data;
                UART1_RX_STA ++;
            }
        }
    }
    else if ((UART1_RX_STA & 0x8000U) == 1) //received finished
    {
    }
}

/*********************************************************************
* Function Name: Uart2_IntSend
* @brief  UART2 Send data by interrupt
* @param  None
* @return None
**********************************************************************/
void Uart2_IntSend(uint8_t *tx_buf, uint16_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST2_IRQn);
}

/********************************************************************
* Function Name: uart2_interrupt_send
* @brief  UART2 Send interrupt service routine
* @param  None
* @return None
*********************************************************************/
void uart2_interrupt_send(void *msg)
{
    ATE_FRAME_t *pFrame = (ATE_FRAME_t *)msg;

    INTC_ClearPendingIRQ(ST2_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART2_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else  //send finished
    {
    }
}
/*******************************************************************
* Function Name: uart2_interrupt_receive
* @brief  UART2 Receive interrupt service routine
* @param  None
* @return None
********************************************************************/
void uart2_interrupt_receive(void *msg)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;


    INTC_ClearPendingIRQ(SR2_IRQn);
    err_type = UART_GetErrStaus(UART2, UART_FLAG_FEF | UART_FLAG_PEF | UART_FLAG_OVF);

    if (err_type)
    {
        uart_callback_error(err_type);
    }

    rx_data = UART2_RX;

    if ((UART2_RX_STA & (1 << 15)) == 0)
    {
        if (UART2_RX_STA < UART_MAX_RECV_LEN)
        {
            TIM_SetCounter(TIM40, TTM_Channel_0, 20); //清除定时器，定时器向下计数，初始化默认为20，因此设置为20
            TIM_Cmd(TIM40, TTM_Channel_0, Enable); //开启计数器
            UART2_RX_BUF[UART2_RX_STA++] = rx_data;
        }
        else
        {
            UART2_RX_STA |= 1 << 15;                //强制标记接收完成
        }
    }
}

#if defined( BAT32G1XX_100PIN) || defined( BAT32G1XX_80PIN)
/******************************************************************************
* Function Name: uart3_init
* @brief  UART3 init
* @param  bound
* @return error or success
******************************************************************************/
int8_t Uart3_Init(uint32_t bound)
{
    int8_t res ;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    GPIO_PinAFConfig(GPIO_PORT14, GPIO_Pin_4, GPIO_P144, GROUP_AF_ODEFAULT);
    GPIO_PinAFConfig(GPIO_PORT14, GPIO_Pin_3, GPIO_P143, GROUP_AF_ODEFAULT);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;		
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT14, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT14, &GPIO_InitStruct);

    /*USART CONFIG*/
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
    UART_InitStructure.phase = UART_Phase_Normal;
    UART_InitStructure.bitorder = UART_Bit_LSB;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //收发模式

    res = UART_Init(UART3, &UART_InitStructure); //初始化串口

    if (res)
    {
        SCI_ERROR_LOG(res);
        return res;
    }

    ISR_Register(ST3_IRQn, uart3_interrupt_send);     //串口2发送中断服务路径注册
    ISR_Register(SR3_IRQn, uart3_interrupt_receive);  //串口2接收中断服务路径注册

    return SCI_SUCCESS;
}

/************************************************************************
* Function Name: uart1_interrupt_receive
* @brief  UART1 Receive interrupt service routine
* @param  None
* @return None
*************************************************************************/
void uart3_interrupt_receive(void *msg)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;

    INTC_ClearPendingIRQ(SR3_IRQn);
    err_type = UART_GetErrStaus(UART3, UART_FLAG_FEF | UART_FLAG_PEF | UART_FLAG_OVF);

    if (err_type)
    {
        uart_callback_error(err_type);
    }

    rx_data = UART3_RX;

    if ((UART3_RX_STA & 0x8000U) == 0) //接收未完成
    {
        if (UART3_RX_STA & 0x4000U) //接收到0x0d
        {
            if (rx_data != 0x0a)
			{
				UART3_RX_BUF[UART3_RX_STA & 0x3fff] = rx_data;
                UART3_RX_STA++;				
			}
            else
            {
                UART3_RX_STA |= 0x8000;
                UART3_RX_BUF[UART3_RX_STA & 0x3fff] = rx_data;
                UART3_RX_STA ++;
            }
        }
        else                     //还未接收到0x0d
        {
            if (rx_data == 0x0d)
            {
                UART3_RX_STA |= 0x4000;
                UART3_RX_BUF[UART3_RX_STA & 0x3fff] = rx_data;
                UART3_RX_STA ++;
            }
            else
            {
                UART3_RX_BUF[UART3_RX_STA & 0x3fff] = rx_data;
                UART3_RX_STA ++;
            }
        }
    }
    else if ((UART3_RX_STA & 0x8000U) == 1) //received finished
    {
    }
}

/****************************************************************************
* Function Name: Uart3_IntSend
* @brief  UART1 Send data by interrupt
* @param  None
* @return None
****************************************************************************/
void Uart3_IntSend(uint8_t *tx_buf, uint16_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST3_IRQn);
}
/***************************************************************************
* Function Name: uart1_interrupt_send
* @brief  UART1 Send interrupt service routine
* @param  None
* @return None
***************************************************************************/
void uart3_interrupt_send(void *msg)
{
    ATE_FRAME_t *pFrame = (ATE_FRAME_t *)msg;

    INTC_ClearPendingIRQ(ST3_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART3_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else  //send finished
    {
    }
}
#endif

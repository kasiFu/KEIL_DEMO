/***********************************************************************************************************************
* @file    lin.c
* @brief   This file implements device driver for LIN Send and Receive.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/
#include "lin_protocol.h"


volatile uint8_t *gp_uart0_rx_address;       /* uart0 receive buffer address */
volatile uint16_t g_uart0_rx_count;          /* uart0 receive data number */
volatile uint16_t g_uart0_rx_length;         /* uart0 receive data length */
volatile uint8_t g_uart_tx_end, g_uart_rx_end;
volatile uint32_t g_inttm03Taken = 0UL;
volatile uint32_t g_tm40_ch3_width = 0UL;
const uint16_t Sps_table[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

#define LIN_BAUDRATE     (19200)        /* baudrate: 19200bps */

uint8_t LinSendingFlag = 0;//置起表示Lin正在发送中
bool ReallyTxBuffFlag = 0;//数据链路层通讯使用的发送标志，表示数据传输完毕从机节点可以发送应答了
lin_frame_t txFrame, rxFrame;
lin_state_t lin_state;
uint8_t SyncEdgeCount = 0;
uint32_t break_timeout, rx_timeout;
uint8_t lin_check_timeout;
uint8_t RightID_SaveReceBuff[8];//保存正确ID接收的数据
uint8_t lin_tx_buf[9];

//中断处理代码
uint8_t Lin_CheckPID(uint8_t id)
{
	uint8_t pid ;
	uint8_t P0 ;
	uint8_t P1 ;
	
	P0 = (((id)^(id>>1)^(id>>2)^(id>>4))&0x01)<<6 ;
	P1 = ((~((id>>1)^(id>>3)^(id>>4)^(id>>5)))&0x01)<<7 ;
	
	pid = id|P0|P1 ;
	
	return pid ;
} 

uint8_t Lin_Checksum(uint8_t id , uint8_t *data)
{
	uint8_t t ;
	uint16_t sum ;

	sum = data[0];
	if((id == LIN_MASTER_REQUEST_ID) || (id == LIN_SLAVER_RESPOND_ID)) // 如果是诊断帧，用经典校验
	{
		
		for(t=1;t<8;t++)
		{
			sum += data[t];
			if(sum&0xff00)
			{
				sum&=0x00ff;
				sum+=1;
			}
		}
		sum = ~sum;	
		
		return (uint8_t)sum ;
	}
	
	for(t=1;t<8;t++)
	{
		sum += data[t];
		if(sum&0xff00)
		{
			sum&=0x00ff;
			sum+=1;
		}
	}
	sum+=Lin_CheckPID(id);
	if(sum&0xff00)
	{
		sum&=0x00ff;
		sum+=1;
	}
	sum = ~sum;	
	return (uint8_t)sum ;
}


void LIN_SleepIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_LOW;	
    GPIO_InitStruct.GPIO_Ctrl  = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT3, &GPIO_InitStruct);
}

/***********************************************************************************************************************
* Function Name: LIN_SetConfig
* @brief  This function sets the configure of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_SetConfig(void)
{
    MISC->ISC = 0x03; /* RXD0 is used as TI03 and INTP0 input */

	CGC_PER0PeriphClockCmd(CGC_PER0Periph_TIM40,ENABLE);/* enables input clock supply */
    TM40->TPS0 = 0x0000;

    /* INTP0 setting */
	Intp_Init();
    /* UART0 setting */
    Uart0_Init(LIN_BAUDRATE);
    ISR_Register(ST0_IRQn, uart0_lin_send);     //lin发送中断服务路径注册
    ISR_Register(SR0_IRQn, uart0_lin_receive);  //lin接收中断服务路径注册

    ISR_Register(TM03_IRQn, tim40_channel3_interrupt);//定时器中断服务注册
	INTC_DisableIRQ(TM03_IRQn);

    TIM_SetCounter(TIM40, TTM_Channel_1, 48000); //使用一个定时器轮询Lin状态机,约1ms调用一次LinMainFun
    TIM_Start(TIM40, TTM_Channel_1);
    ISR_Register(TM01_IRQn, LinMainFun);//定时器中断服务注册
	NVIC_SetPriority(TM01_IRQn, 0);
    
    LIN_ReceiveBreak();
}

/***********************************************************************************************************************
* Function Name: LIN_ReceiveBreak
* @brief  This function implements measure the break field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_ReceiveBreak(void)
{
	TIM_Stop(TIM40,TTM_Channel_3);
    INTC_ClearPendingIRQ(TM03_IRQn);  /* clear interrupt flag */

    /* Channel 3 is used to measure input pulse width */
//    TM40->TMR03 = _0000_TM4_CLOCK_SELECT_CKM0 | _0000_TM4_CLOCK_MODE_CKS | _0200_TM4_TRIGGER_TIMN_BOTH |
//                  _0080_TM4_TIMN_EDGE_BOTH_LOW | _000C_TM4_MODE_HIGHLOW_MEASURE;
    TM40->TMR03 = 0x0000 | 0x0000 | 0x0200 | 0x0080 | 0x000C;//设置定时器通道3为测量低电平模式

    INTC_EnableIRQ(TM03_IRQn);  /* enable interrupt */
	TIM_Start(TIM40,TTM_Channel_3);
	
    g_inttm03Taken = 0;
}

/***********************************************************************************************************************
* Function Name: LIN_GetBaudRate
* @brief  This function gets the baud rate by measure the sync field of LIN.
* @param  None
* @return the setting value of baudrate
***********************************************************************************************************************/
uint16_t LIN_GetBaudRate(void)
{
    uint32_t period = 0;
    uint8_t  tmp;

    while(g_inttm03Taken < 2);
    period = g_tm40_ch3_width;
    /* calculate baudrate value */
    tmp = period / Sps_table[UART_PRS] / 4 - 1;

    return (tmp << 9);
}
/***********************************************************************************************************************
* Function Name: LIN_ReceiveSync
* @brief  This function implements measure the sync field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_ReceiveSync(void)
{
	TIM_Stop(TIM40,TTM_Channel_3);
    INTC_ClearPendingIRQ(TM03_IRQn);  /* clear interrupt flag */

    /* Channel 3 is used to measure input pulse interval at falling edge */
//    TM40->TMR03 = _0000_TM4_CLOCK_SELECT_CKM0 | _0000_TM4_CLOCK_MODE_CKS | _0100_TM4_TRIGGER_TIMN_VALID |
//                  _0000_TM4_TIMN_EDGE_FALLING | _0004_TM4_MODE_CAPTURE | _0000_TM4_START_INT_UNUSED;
    TM40->TMR03 = 0x0000 | 0x0000 | 0x0100 | 0x0000 | 0x0004;//设置定时器通道3为测量脉冲间隔
	
    INTC_EnableIRQ(TM03_IRQn);  /* enable interrupt */
	TIM_Start(TIM40,TTM_Channel_3);

    g_inttm03Taken = 0;
}




void lin_callback_error(uint8_t err_type)
{
    //user edit here when appear error
}

void LIN_Receive(uint8_t * rx_buf, uint8_t rx_num)
{
	INTC_ClearPendingIRQ(SR0_IRQn);  /* enable interrupt */		//不清此标志，使能后会进一次中断
    INTC_EnableIRQ(SR0_IRQn);  /* enable interrupt */
    g_uart0_rx_count = 0U;
    g_uart0_rx_length = rx_num;
    gp_uart0_rx_address = rx_buf;
}

void LIN_Send(uint8_t * tx_buf, uint8_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST0_IRQn);
}

/*****************************************************************************
* Function Name: uart0_lin_receive
* @brief  UART0 Receive interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void uart0_lin_send(void *msg)
{
    ATE_FRAME_t *pFrame = (ATE_FRAME_t *)msg;

    INTC_ClearPendingIRQ(ST0_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART0_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else  //send finished
    {
		g_uart_tx_end = 1;
    }
}


/*****************************************************************************
* Function Name: uart0_lin_receive
* @brief  UART0 Receive interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void uart0_lin_receive(void *msg)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
	volatile uint8_t ID;
	
    INTC_ClearPendingIRQ(SR0_IRQn);
    err_type = UART_GetErrStaus(UART0, UART_FLAG_FEF | UART_FLAG_PEF | UART_FLAG_OVF);

    if (err_type)
    {
        lin_callback_error(err_type);
    }

    rx_data = UART0_RX;

    if(lin_state==LIN_TX)//发送模式下屏蔽掉接收信息
    {
        return;
    }

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
		//判断数据
		if(g_uart0_rx_count == 1)
		{
            if(rx_data==Lin_CheckPID(LIN_SLAVER_RESPOND_ID))//判断PID,如果是诊断帧的0X3D表示主机读，从机转换为发送模式
            {
                lin_state = LIN_TX;
            }
        }

        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            g_uart_rx_end = 1;
        }
        else
        {
            
        }
	}
}


/***********************************************************************************************************************
* Function Name: tim40_channel3_interrupt
* @brief  TIM40 Channel interrupt service routine
* @param  msg
* @return None
***********************************************************************************************************************/
void tim40_channel3_interrupt(void *msg)
{
    INTC_ClearPendingIRQ(TM03_IRQn);    // clear INTTM00 interrupt flag
    g_inttm03Taken++;
    if (1U == (TM40->TSR03 & 0x0001))    /* overflow occurs */
    {
        g_tm40_ch3_width = (uint32_t)(TM40->TDR03 + 1UL) + 0x10000UL;
    }
    else
    {
        g_tm40_ch3_width = (uint32_t)(TM40->TDR03 + 1UL);
    }
		
    if(lin_state==LIN_IDLE)
    {
        //中断判断间隔符宽度
        if(g_tm40_ch3_width > 11 * 2500)		//(SystemCoreClock / LIN_BAUDRATE)=2500
        {
            SyncEdgeCount = 0;
            
            lin_state = BREAK;
            rx_timeout = 0;
            
            //关闭定时器，开始计算超时
            TM40->TMR03 = 0x0000U | 0x0000U | 0x0100U | 0x0040U | 0x000CU;
            lin_check_timeout = 1;
        }
    }
    else if(lin_state == BREAK)
    {
        SyncEdgeCount++;
        if(SyncEdgeCount>=4)//检测同步完毕，设置新波特率
        {
            SyncEdgeCount = 0;
            rx_timeout = 0;
            LIN_Receive((uint8_t *)&rxFrame, sizeof(rxFrame));
            lin_check_timeout = 1;
            lin_state = LIN_RX;
            TM40->TMR03 = 0x0000U | 0x0000U | 0x0200U | 0x0080U | 0x000CU;
        }
        
    }
    else
	{
        
	}

}

void OverTimeTask()//超时恢复
{
    lin_state = LIN_IDLE;
    lin_check_timeout	= 0;
    TM40->TMR03 = 0x0000U | 0x0000U | 0x0200U | 0x0080U | 0x000CU;
}

//做从机时的应答
void LinSlaverRespond(uint8_t pid,uint8_t* send_data)
{
    uint8_t i;
	for(i=0; i<8; i++)
	{
		txFrame.Data[i] = *(send_data+i);
	}
    txFrame.ChkSum = Lin_Checksum(pid,send_data);
    LIN_Send((uint8_t *)&txFrame.Data, sizeof(txFrame.Data)+1);//加上一个校验位

}

void LinMainFun()
{
    INTC_ClearPendingIRQ(TM01_IRQn);    // clear INTTM00 interrupt flag

    switch(lin_state)
    {
        case LIN_IDLE:
        case WAKE:
            break_timeout = 0;
            lin_check_timeout	= 0;
               
            break;
        case BREAK:
            if(lin_check_timeout)
            {
                if(break_timeout ++ > 1500)//超过1500ms复原LIN状态
                {
                    OverTimeTask();
                }
                
            }
            break;
        case LIN_RX:
            if(g_uart_rx_end == 1)
            {
                g_uart_rx_end = 0;
                OverTimeTask();
                uint8_t checksum = Lin_Checksum(rxFrame.PID_b.ID,rxFrame.Data);
                #ifdef DEBUG_PRINTF
                printf("PID = %02X\n",rxFrame.PID_b.ID);
                printf("Data[0] = %02X\n",rxFrame.Data[0]);
                printf("Data[1] = %02X\n",rxFrame.Data[1]);
                printf("Data[2] = %02X\n",rxFrame.Data[2]);
                printf("Data[3] = %02X\n",rxFrame.Data[3]);
                printf("Data[4] = %02X\n",rxFrame.Data[4]);
                printf("Data[5] = %02X\n",rxFrame.Data[5]);
                printf("Data[6] = %02X\n",rxFrame.Data[6]);
                printf("Data[7] = %02X\n",rxFrame.Data[7]);
                printf("CheckSum = %02X\n",rxFrame.ChkSum);
                printf("PlusTimerCount = %d\n",PlusTimerCount/2);//观察同步段一个位所需使用的时间
                
                if(checksum!=rxFrame.ChkSum)
                {
                    printf("CheckSum error! %02X\n",checksum);
                    
                }
                else//解析接收数据执行相应操作
                {
                    if(rxFrame.PID_b.ID==LIN_MASTER_REQUEST_ID)
                    {
                        for(int i = 0;i<8;i++)
                        {
                            RightID_SaveReceBuff[i] = rxFrame.Data[i];
                        }
                        TP_DriverWriteDataInTP(LIN_MASTER_REQUEST_ID, sizeof(RightID_SaveReceBuff), RightID_SaveReceBuff);
                    }
                    
                }
                printf("\n");
                #else
                if(checksum==rxFrame.ChkSum)//解析接收数据执行相应操作
                {
                    if(rxFrame.PID_b.ID==LIN_MASTER_REQUEST_ID)
                    {
                        for(int i = 0;i<8;i++)
                        {
                            RightID_SaveReceBuff[i] = rxFrame.Data[i];
                        }
                        TP_DriverWriteDataInTP(LIN_MASTER_REQUEST_ID, sizeof(RightID_SaveReceBuff), RightID_SaveReceBuff);
                    }
                }
                #endif
            }
            //处理超时
            if(rx_timeout++ > 500)//超过500ms复原LIN状态
            {
                OverTimeTask();
            }
            
            break;
        case LIN_TX://回应发送数据
            if(LinSendingFlag==0)
            {
                #ifdef DEBUG_PRINTF
                printf("Respond Data\n");
                #endif
                if(ReallyTxBuffFlag==TRUE)
                {
                    for(int i = 0;i<8;i++)
                    {
                        lin_tx_buf[i] = 0xff;
                    }    
                    lin_tx_buf[0] = LIN_NAD;//NAD
                    if((gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen) <= 6) /* SF frame */
                    {
                        lin_tx_buf[1] = gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen; /* SF frame len */
                        for(int i = 0;i<gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen;i++)
                        {
                            lin_tx_buf[2+i] = gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.aDataBuf[i];
                        }
                    }
                    else if((gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen >> 12u) == 1) /* FF frame */
                    {
                        lin_tx_buf[1] = (gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen >> 8u) & 0xFF; /* FF frame len */
                        lin_tx_buf[2] = (gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen) & 0xFF; /* FF frame len */
                        for(int i = 0;i<5u;i++)
                        {
                            lin_tx_buf[3+i] = gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.aDataBuf[i];
                        }
                    }
                    else if((gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen >> 12u) == 2) /* CF frame */
                    {
                        lin_tx_buf[1] = (gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.xPduDataLen >> 8u) & 0xFF; /* CF frame cnt */
                        for(int i = 0;i<6u;i++)
                        {
                            lin_tx_buf[2+i] = gs_stLinTPTxRxDataInfo.stLinTpTxDataInfo.aDataBuf[i];
                        }
                    }

                    LinSlaverRespond(LIN_SLAVER_RESPOND_ID,lin_tx_buf);//把上一命令处理的结果返回
                                     
                    LinSendingFlag = 1;
                    ReallyTxBuffFlag = FALSE;
                    if(lin_tx_buf[2]==0x51)//判断是否为复位应答信号，如果是需要在发送应答后置起复位标志进行复位
                    {
                         ResetFlag = TRUE;
                    }
                }                
            }
            else if(LinSendingFlag==1)
            {
                if(g_uart_tx_end==1)
                {
                    OverTimeTask();
                    g_uart_tx_end = 0;
                    LinSendingFlag = 0;
                }
            }
        
            break;
        default:break;
    }
}

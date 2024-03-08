#include "dma_drv.h"
unsigned char CTRL_DATA_UART0 = 6;
/***********************************************************************************************************************
* Function Name: DMA_Uart0_Tx
* @brief  config DMA for uart tx
* @param  src_adr: dma source address
* @param  dst_adr: dma destine address
* @param  count: dma transform num
* @return None
***********************************************************************************************************************/
extern  volatile uint8_t   *sfr_addr;
void DMA_Uart0_Tx(DMA_VECTOR_t dma_vector, DMA_Mode_t mode, void *src_adr, void *dst_adr, uint16_t count)
{
    DMA_InitTypeDef  DMA_InitStructure = {0};
    unsigned int CTRL_DATA = 7;

    DMA_InitStructure.DMA_Vector = dma_vector;    //根据功能选择不同的dma向量区
    DMA_InitStructure.DMA_CtrlId = CTRL_DATA;     //选择控制数据区
    DMA_InitStructure.DMA_SrcAddr = (uint32_t)src_adr;  //配置dma源地址
    DMA_InitStructure.DMA_DstAddr = (uint32_t)dst_adr;  //配置dma目标地址
    DMA_InitStructure.DMA_BufferSize = count;
    DMA_InitStructure.DMA_SrcInc = DMA_SrcInc_Enable;//源地址增量模式
    DMA_InitStructure.DMA_DstInc = DMA_DstInc_Disable;//目标地址固定
    DMA_InitStructure.DMA_DataSize = DMA_DataSize_Byte;//传输数据长度选择
    DMA_InitStructure.DMA_Mode = mode;//普通模式
	DMA_Init(&DMA_InitStructure);

	DMA_Start(DMA_InitStructure.DMA_Vector);
}

/***********************************************************************************************************************
* Function Name: DMA_Uart0_Rx
* @brief  config DMA for uart read
* @param  src_adr: dma source address
* @param  dst_adr: dma destine address
* @param  count: dma transform num
* @return None
***********************************************************************************************************************/
void DMA_Uart0_Rx(DMA_VECTOR_t dma_vector, DMA_Mode_t mode, void *src_adr, void *dst_adr, uint16_t count)
{
    DMA_InitTypeDef  DMA_InitStructure = {0};


    DMA_InitStructure.DMA_Vector = dma_vector;    //根据功能选择不同的dma向量区
    DMA_InitStructure.DMA_CtrlId = CTRL_DATA_UART0;     //选择控制数据区
    DMA_InitStructure.DMA_SrcAddr = (uint32_t)src_adr;  //配置dma源地址
    DMA_InitStructure.DMA_DstAddr = (uint32_t)dst_adr;  //配置dma目标地址
    DMA_InitStructure.DMA_BufferSize = count;
    DMA_InitStructure.DMA_SrcInc = DMA_SrcInc_Disable;//源地址固定模式
    DMA_InitStructure.DMA_DstInc = DMA_DstInc_Enable;//目标地址增量
    DMA_InitStructure.DMA_DataSize = DMA_DataSize_Byte;//传输数据长度选择
    DMA_InitStructure.DMA_Mode = mode;
    DMA_InitStructure.DMA_RepeatAreaAddr = DMA_RepeatAreaAddr_Dst;//正常模式下：重复区选择无效；重复模式下有效：传送目标为重复区
    DMA_InitStructure.DMA_RepeatInt = DMA_RepeatInt_Disable;//重复模式下有效：允许中断
	DMA_Init(&DMA_InitStructure);

	DMA_Start(DMA_InitStructure.DMA_Vector);

}


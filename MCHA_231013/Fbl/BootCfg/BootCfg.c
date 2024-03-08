#include "BootCfg.h"
#include "uds_lin_tp.h"
#include "uds_app.h"
#include "SecM.h"
#include "wdt.h"

const uint8_t ReadDID_0xF194DataBuff[9] = 
{
    /* 供应商:2 Byte */
    0x00, 0x00,
    /* 类型:1 Byte */
    0x00,
    /* 车型代码:2 Byte */
    0x00, 0x00,
    /* ECU类型:1 Byte */
    0x00,
    /* 地区代码:1 Byte */
    0x00,
    /* 扩展信息1:1 Byte */
    0x00,
    /* 扩展信息2:1 Byte */
    0x00,
};
const uint8_t ReadDID_0xF193DataBuff[5] = 
{
    /* 版本号:1 Byte */
    0x00,
    /* 年:1 Byte */
    0x00,
    /* 月:1 Byte */
    0x00,
    /* 日:1 Byte */
    0x00,
    /* 次:1 Byte */
    0x00,
};
const uint8_t ReadDID_0xF180DataBuff[9] = 
{
    /* 年:1 Byte */
    0x00,
    /* 周:1 Byte */
    0x00,
    /* 次:1 Byte */
    0x00,
    /* 软件版本:1 Byte */
    0x00,
    /* 次要信息:1 Byte */
    0x00,
    /* MCU型号:1 Byte */
    0x00,
    /* 年:1 Byte */
    0x00,
    /* 月:1 Byte */
    0x00,
    /* 日:1 Byte */
    0x00,
};

uint8_t ResetFlag = FALSE;
uint32_t CurrentMode;//当前工作模式
uint8_t EraseOneSector(uint32_t addr)
{
    if(EraseSector(addr))
    {
        return FALSE;
    }
    return TRUE;
}
uint8_t EraseAllApp(uint32_t addr)
{
    uint16_t i;
	uint16_t k = 0;

	if(addr==APP_BASE_ADDR)
	{
		k = (APP_FLASH_SIZE/FLASH_ONE_SECTOR_SIZE);
	}
    else
    {
        return FALSE;
    }
    for(i=0;i<k;i++)
    {
        WDT_Restart();
		if(EraseOneSector(addr+FLASH_ONE_SECTOR_SIZE*i)==FALSE)
        {
            return FALSE;
        }        
        //回应流控帧
        if(i%10==9)
        {
            
        }
    }
    return TRUE;
}
void BootInit()
{
    tUdsAppMsgInfo stUdsAppMsg = {0u, 0u, {0u}, NULL_PTR};
    
    CurrentMode = *(uint32_t *)BOOT_FLAG_ADDR ;
    if(CurrentMode == REPROGRAM_MODE)
    {
        SetCurrentSession(PROGRAM_SESSION);
        /*restart s3server time*/
        RestartS3Server();
        CurrentMode = BOOT_MODE;
        (void)ReflashWorkMode(BOOT_MODE);
        stUdsAppMsg.aDataBuf[0u] = 0x50u;
        stUdsAppMsg.aDataBuf[1u] = 0x02u;
        stUdsAppMsg.aDataBuf[2u] = 0x00u;
        stUdsAppMsg.aDataBuf[3u] = 0x32u;
        stUdsAppMsg.aDataBuf[4u] = 0x01u;
        stUdsAppMsg.aDataBuf[5u] = 0xF4u;
        stUdsAppMsg.xDataLen = 6u;
        LINTP_TxMsg(SLAVER_RESPOND_ID,stUdsAppMsg.xDataLen, stUdsAppMsg.aDataBuf,g_stLINUdsNetLayerCfgInfo.txBlockingMaxTimeMs);
        ReallyTxBuffFlag = TRUE;
        ClearRxMsgReadlly();
    }
    else
    {
        CurrentMode = CurrentMode;
    }
}
uint8_t FlashReadOneByte(uint32_t addr)	//读单字节IAP操作
{   
    uint8_t IAP_IapData; 
    IAP_IapData = *(uint32_t *)addr;
	return IAP_IapData;
}

uint8_t FlashWriteOneByte(uint32_t addr,uint8_t dat)
{
    uint8_t *ptr;
    
    ptr = (uint8_t *) addr;
    
    FMC->FLPROT = 0xF1;
    
    FMC->FLOPMD1 = 0xAA;
    FMC->FLOPMD2 = 0x55;  
    *ptr = dat;    
    // polling OVER Flag
    while((FMC->FLSTS & FMC_FLSTS_OVF_Msk) == 0);
    FMC->FLSTS |= FMC_FLSTS_OVF_Msk;

    FMC->FLPROT = 0x00;
	
    if(FlashReadOneByte(addr) == dat)
    {
        return TRUE;	//写入准确
    }
    else
    {
        return FALSE;	//写入有误
    }
}
uint8_t FlashWriteMultiByte(uint32_t addr,uint8_t * buff,uint32_t len)	//写多字节IAP操作
{
	uint32_t i;
	uint8_t Write_IAP_IapData;
	for(i=0;i<len;i++)
	{
		Write_IAP_IapData = buff[i];
        if(FlashWriteOneByte(addr+i,Write_IAP_IapData)==FALSE)//判断写入是否正确
		{
			return FALSE;
		}
	}
	return TRUE;
}

#define _0002_SCI_CH1_STOP_TRG_ON   (0x0002U) /* clears the SEmn bit to 0 and stops the communication operation */
#define _0001_SCI_CH0_STOP_TRG_ON   (0x0001U) /* clears the SEmn bit to 0 and stops the communication operation */

void ResetMcu(uint32_t addr)
{
    SCI0->SCI.ST0   = _0002_SCI_CH1_STOP_TRG_ON | _0001_SCI_CH0_STOP_TRG_ON;
	CGC->PER0 &= ~CGC_PER0_SCI0EN_Msk;
	INTC_DisableIRQ(SR0_IRQn);
    __disable_irq();//关闭所有中断
    __set_MSP(*(__IO uint32_t*) addr);
    ((void (*)()) (*(volatile unsigned long *)(addr+0x04)))();//to APP    
    /* Trap the CPU */
    while(1);
}

void AppRemap(uint32_t vector_addr)
{
	__disable_irq();
	SCB->VTOR = vector_addr;/*vector address*/
	__DMB();	
	__enable_irq();

}

uint8_t FlashCheckSum(uint32_t addr ,uint32_t len ,uint32_t value)
{
        /* CRC32 parameter */
    SecM_CRCParamType crcParam;
    
    /* compute CRC for NVM information */
    crcParam.crcState = (uint8_t)SECM_CRC_INIT;
    crcParam.crcSourceBuffer = (const uint8_t *)addr;
    crcParam.crcByteCount = (uint16_t)len;
    (void)SecM_ComputeCRC(&crcParam);
    crcParam.crcState = (uint8_t)SECM_CRC_COMPUTE;
    (void)SecM_ComputeCRC(&crcParam);
    crcParam.crcState = (uint8_t)SECM_CRC_FINALIZE;
    (void)SecM_ComputeCRC(&crcParam);

    if(crcParam.currentCRC == value)
    {
        return TRUE;
    }
    return FALSE;
}

extern void delayMS(uint32_t n);
void ResetMonitor()
{
    if(ResetFlag==TRUE)
    {
        delayMS(500);//延时一段时间保证LIN响应完毕
        ResetMcu(APP_BASE_ADDR);
    }
}

void BootMonitor()//检查APP是否完整和检查BOOT等待时间是否足够,请每1ms调用一次
{
    static int BootMonitorTimeCount = 0;
    static uint8_t BootMonitorTimeOverFlag = 0;
    if(BootMonitorTimeCount>=3000)
    {
        BootMonitorTimeOverFlag = 1;
    }
    else
    {
        BootMonitorTimeCount++;
    }
    if(BootMonitorTimeOverFlag==1)
    {
        if(CurrentMode == APP_MODE)
        {
            ResetMcu(APP_BASE_ADDR);
        }
    }
}

uint8_t ReflashWorkMode(uint32_t mode_num)
{
    
    EraseOneSector(BOOT_FLAG_ADDR);
    uint32_t *ptr;
    
    ptr = (uint32_t *) BOOT_FLAG_ADDR;
    
    FMC->FLPROT = 0xF1;
    
    FMC->FLOPMD1 = 0xAA;
    FMC->FLOPMD2 = 0x55;  
    *ptr = mode_num;
    // polling OVER Flag
    while((FMC->FLSTS & FMC_FLSTS_OVF_Msk) == 0);
    FMC->FLSTS |= FMC_FLSTS_OVF_Msk;

    FMC->FLPROT = 0x00;
	
    if(*(uint32_t *)BOOT_FLAG_ADDR == mode_num)
    {
        return TRUE;	//写入准确
    }
    else
    {
        return FALSE;	//写入有误
    }
    
    
}


#include "BootCfg.h"
#include "uds_lin_tp.h"
#include "uds_app.h"
#include "SecM.h"
#include "wdt.h"

const uint8_t ReadDID_0xF194DataBuff[9] = 
{
    /* ��Ӧ��:2 Byte */
    0x00, 0x00,
    /* ����:1 Byte */
    0x00,
    /* ���ʹ���:2 Byte */
    0x00, 0x00,
    /* ECU����:1 Byte */
    0x00,
    /* ��������:1 Byte */
    0x00,
    /* ��չ��Ϣ1:1 Byte */
    0x00,
    /* ��չ��Ϣ2:1 Byte */
    0x00,
};
const uint8_t ReadDID_0xF193DataBuff[5] = 
{
    /* �汾��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
};
const uint8_t ReadDID_0xF180DataBuff[9] = 
{
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ����汾:1 Byte */
    0x00,
    /* ��Ҫ��Ϣ:1 Byte */
    0x00,
    /* MCU�ͺ�:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
    /* ��:1 Byte */
    0x00,
};

uint8_t ResetFlag = FALSE;
uint32_t CurrentMode;//��ǰ����ģʽ
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
        //��Ӧ����֡
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
uint8_t FlashReadOneByte(uint32_t addr)	//�����ֽ�IAP����
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
        return TRUE;	//д��׼ȷ
    }
    else
    {
        return FALSE;	//д������
    }
}
uint8_t FlashWriteMultiByte(uint32_t addr,uint8_t * buff,uint32_t len)	//д���ֽ�IAP����
{
	uint32_t i;
	uint8_t Write_IAP_IapData;
	for(i=0;i<len;i++)
	{
		Write_IAP_IapData = buff[i];
        if(FlashWriteOneByte(addr+i,Write_IAP_IapData)==FALSE)//�ж�д���Ƿ���ȷ
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
    __disable_irq();//�ر������ж�
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
        delayMS(500);//��ʱһ��ʱ�䱣֤LIN��Ӧ���
        ResetMcu(APP_BASE_ADDR);
    }
}

void BootMonitor()//���APP�Ƿ������ͼ��BOOT�ȴ�ʱ���Ƿ��㹻,��ÿ1ms����һ��
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
        return TRUE;	//д��׼ȷ
    }
    else
    {
        return FALSE;	//д������
    }
    
    
}


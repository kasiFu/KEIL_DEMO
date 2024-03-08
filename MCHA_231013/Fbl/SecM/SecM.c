/*=======[I N C L U D E S]====================================================*/
#include "secm.h"
#include "cal.h"

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/**
 * @brief               <compute CRC>
 * 
 * <process CRC compute,include init,compute and finish> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <crcParam (IN/OUT)>
 * @return              <SecM_StatusType>    
 */
/******************************************************************************/
SecM_StatusType SecM_ComputeCRC(SecM_CRCParamType *crcParam)
{
    SecM_StatusType ret = SECM_OK;

    switch (crcParam->crcState)
    {
    case SECM_CRC_INIT:
        /* CRC value initialize */
        Cal_CrcInit(&crcParam->currentCRC);
        break;

    case SECM_CRC_COMPUTE:
        /* CRC value compute */
        Cal_CrcCal(&crcParam->currentCRC,
                   crcParam->crcSourceBuffer,
                   (uint32_t)crcParam->crcByteCount);
        break;

    case SECM_CRC_FINALIZE:
        /* CRC value finish */
        Cal_CrcFinalize(&crcParam->currentCRC);
        break;

    default:
        ret = SECM_NOT_OK;
        break;
    }

    return ret;
}

/*=======[E N D   O F   F I L E]==============================================*/

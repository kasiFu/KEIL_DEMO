#include "UDS_alg_hal.h"

bool UDS_ALG_HAL_EncryptData(const uint8_t *i_pPlainText, const uint32_t i_dataLen, uint8_t *o_pCipherText)
{
}
bool UDS_ALG_HAL_DecryptData(const uint8_t *i_pCipherText, const uint32_t i_dataLen, uint8_t *o_pPlainText)
{
    
}
/* Random value, seed is 0x12345678 */
static uint32_t u32RandVal = 0x12345678U; 
void fsl_srand (uint32_t u32Seed9)
{
     /* Value 0 is forbidden because it would block the LFSR */
     if (0U == u32Seed9)
     {   /* Forbidden value */
         u32Seed9++; /* Correct the value */
     }
     u32RandVal = u32Seed9; /* Set the seed */
}

/**
* @brief Returns pseudo-random number
* @details Function generates pseudo-random number using the LFSR algorithm.
* @return Pseudo-random number in the interval from 0 to 0xFFFFFFFF
*/
uint32_t fsl_rand(void)
{
    /* Generate the next value in the sequence */
    u32RandVal = (u32RandVal >> 1U) ^ ((0U - (u32RandVal & 1U)) & 0x80200003U);
    /* Return the value */
    return u32RandVal;
}
static uint32_t gs_UDS_SWTimerTickCnt;
/*UDS software timer tick*/
void UDS_ALG_HAL_AddSWTimerTickCnt(void)
{
	gs_UDS_SWTimerTickCnt++;
}

bool UDS_ALG_HAL_GetRandom(const uint32_t i_needRandomDataLen, uint8_t *o_pRandomDataBuf)
{
    bool ret = TRUE;
	uint8_t index = 0u;
	uint8_t *pRandomTmp = NULL_PTR;
	uint32_t random = (uint32_t)&index;

	if((0u == i_needRandomDataLen) || (NULL_PTR == o_pRandomDataBuf))
	{
		ret = FALSE;
	}

	random = TIMER_HAL_GetTimerTickCnt();
	
	random |= (gs_UDS_SWTimerTickCnt << 16u);
	fsl_srand(random);

	if(TRUE == ret)
	{		
		pRandomTmp = (uint8_t *)&random;
		
		for(index = 0u; index < i_needRandomDataLen; index++)
		{
			if(((index & 0x03u) == 0x03u))
			{
				/*get random*/
				random = fsl_rand();
			}
			
			o_pRandomDataBuf[index] = pRandomTmp[index & 0x03];
		}
	}


	return ret;
}

void UDS_ALG_HAL_GetKey(const uint16_t localSeed, uint8_t *i_pCalculateKey)
{
    uint16_t KeyResult;

    KeyResult = ((((localSeed >> 1) ^ localSeed) << 3) ^ (localSeed >> 2));

    KeyResult = (KeyResult ^ UDS_ALG_HAL_KEYK);

    i_pCalculateKey[0] = (KeyResult >> 8u) & 0xFF;
    i_pCalculateKey[1] = (KeyResult >> 0u) & 0xFF;
}

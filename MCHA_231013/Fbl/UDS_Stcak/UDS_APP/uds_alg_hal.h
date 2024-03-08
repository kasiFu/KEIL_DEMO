#ifndef UDS_ALG_HAL_H
#define UDS_ALG_HAL_H

#include "BAT32A237.h"
#include "common_type.h"
#include "timer_hal.h"
#include "BootCfg.h"

extern void UDS_ALG_HAL_AddSWTimerTickCnt(void);
extern bool UDS_ALG_HAL_EncryptData(const uint8_t *i_pPlainText, const uint32_t i_dataLen, uint8_t *o_pCipherText);
extern bool UDS_ALG_HAL_DecryptData(const uint8_t *i_pCipherText, const uint32_t i_dataLen, uint8_t *o_pPlainText);
extern bool UDS_ALG_HAL_GetRandom(const uint32_t i_needRandomDataLen, uint8_t *o_pRandomDataBuf);
extern void UDS_ALG_HAL_GetKey(const uint16_t localSeed, uint8_t *i_pCalculateKey);

#endif

#ifndef BOOT_CFG_H
#define BOOT_CFG_H

#include "BAT32A237.h"
#include "flash.h"
#include "common_type.h"


#define BOOT_BASE_ADDR  0x00000000
#define BOOT_FLASH_SIZE 0x00005000
#define APP_BASE_ADDR   0x00005000
#define APP_FLASH_SIZE  0x0001B000
#define BOOT_FLAG_ADDR  0x00500200
#define REPROGRAM_FLAG_ADDR  0x00500204
#define BOOT_FLAG_SIZE  0x00000200

#define FLASH_ONE_SECTOR_SIZE   512
#define BOOT_MODE     0X55AAAA55
#define APP_MODE      0XAA559669
#define REPROGRAM_MODE 0XAA5555AA
#define NORMAL_MODE 0XAA559669

#define UDS_ALG_HAL_KEYK 0x1000

extern const uint8_t ReadDID_0xF194DataBuff[9];
extern const uint8_t ReadDID_0xF193DataBuff[5];
extern const uint8_t ReadDID_0xF180DataBuff[9];
extern uint8_t ResetFlag;
extern uint32_t CurrentMode;

uint8_t EraseOneSector(uint32_t addr);
uint8_t EraseAllApp(uint32_t addr);
uint8_t FlashReadOneByte(uint32_t addr);	
uint8_t FlashWriteOneByte(uint32_t addr,uint8_t dat);
uint8_t FlashWriteMultiByte(uint32_t addr,uint8_t * buff,uint32_t len);
void ResetMcu(uint32_t addr);
void AppRemap(uint32_t vector_addr);
uint8_t FlashCheckSum(uint32_t addr ,uint32_t len ,uint32_t value);
void ResetMonitor(void);
uint8_t ReflashWorkMode(uint32_t mode_num);
void BootMonitor(void);
void BootInit(void);
#endif

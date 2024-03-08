#ifndef SECM_H
#define SECM_H

/*=======[I N C L U D E S]====================================================*/
#include "secm_cfg.h"
#include <stdint.h>
/*=======[M A C R O S]========================================================*/

/** CRC buffer length */
#define SECM_CRC_BUFFER_LEN 100

#define SECM_OK            (uint8_t)0x00U
#define SECM_NOT_OK    (uint8_t)0x01U

/** CRC step */
#define SECM_CRC_INIT	    0x00u
#define SECM_CRC_COMPUTE	0x01u
#define SECM_CRC_FINALIZE	0x02u

#if (CAL_CRC32 == CAL_METHOD)
#define SECM_CRC_LENGTH     0x04u
#else
#define SECM_CRC_LENGTH     0x02u
#endif

/* frc base address */
#define FRC_REG_BASE                    (0x40u)
#define FRC_TCNTH                       (0x04u)
#define FRC_TCNTL                       (0x05u)
#define FRC_TSCR1                       (0x06u)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** return type for SecM module */
typedef uint8_t SecM_StatusType;

/** type for Seed */
typedef uint32_t SecM_WordType;

/** struct type for Seed */
typedef uint32_t SecM_SeedType;

/** type for Key */
typedef SecM_WordType SecM_KeyType;

/** type for Crc value */
#if (CAL_CRC32 == CAL_METHOD)
typedef uint32_t SecM_CRCType;
#else
typedef uint16_t SecM_CRCType;
#endif

/** struct type for Crc */
typedef struct
{
    /* current CRC value */
    SecM_CRCType currentCRC;

    /* CRC step */
    uint8_t crcState;

    /* CRC buffer point */
    const uint8_t *crcSourceBuffer;

    /* CRC length */
    uint16_t crcByteCount;

} SecM_CRCParamType;

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern SecM_StatusType SecM_ComputeCRC(SecM_CRCParamType *crcParam);
#if 0
extern void Frc_Init(void);
extern void Frc_Uninit(void);
#endif
#endif/* endof SECM_H */

/*=======[E N D   O F   F I L E]==============================================*/

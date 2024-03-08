#ifndef SECM_CFG_H
#define SECM_CFG_H

/*=======[M A C R O S]========================================================*/

/*  CRC method */
#define CAL_CRC16    0x00u
#define CAL_CRC32    0x01u
#define CAL_METHOD   CAL_CRC32

/** value k for security access */
/* @type:uint32_t range:0x00000000~0xFFFFFFFF note:NONE */
#define SECM_ECU_KEY 0x8704162BU

#endif/* endof SECM_CFG_H */

/*=======[E N D   O F   F I L E]==============================================*/

#ifndef CAL_H
#define CAL_H

/*=======[I N C L U D E S]====================================================*/
#include "secm.h"
#include <stdint.h>
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern void Cal_CrcInit(SecM_CRCType *curCrc);

extern void Cal_CrcCal(SecM_CRCType *curCrc, const uint8_t *buf, const uint32_t size);

extern void Cal_CrcFinalize(SecM_CRCType *curCrc);

#endif/* end of CAL_H */

/*=======[E N D   O F   F I L E]==============================================*/

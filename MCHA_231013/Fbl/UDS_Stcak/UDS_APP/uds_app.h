#ifndef UDS_APP_H
#define UDS_APP_H

#include "common_type.h"
#include "uds_lin_tp.h"
#include "UDS_alg_hal.h"
#include "BootCfg.h"
#include <string.h>

typedef struct
{
    uint32_t StartAddr;         /*data start address*/
    uint32_t DataLen;           /*data len*/
} tDowloadDataInfo;

typedef struct
{
    uint32_t StartAddr;         /*data start address*/
    uint32_t DataLen;           /*data len*/
} tEraseDataInfo;

typedef struct
{
    uint32_t StartAddr;         /*data start address*/
    uint32_t DataLen;           /*data len*/
    uint32_t CheckSum;          /*CheckSum*/
} tCheckSumDataInfo;

/*define security access info*/
typedef struct
{
    uint8_t SubfunctionNumber;    /*subfunction number*/
    uint8_t RequestSession;       /*request session*/
    uint8_t RequestIDMode;        /*request id mode*/
    uint8_t RequestSecurityLevel; /*request security level*/
    void (*pfRoutine)(void);    /*routine*/
} tSecurityAccessInfo;

/*define write data subfunction*/
typedef struct
{
    uint8_t Subfunction;      /*subfunction*/
    uint8_t RequestSession;   /*request session*/
    uint8_t RequestIdMode;    /*request id mode*/
    uint8_t RequestLevel;     /*request level*/
    void (*pfRoutine)(void);/*routine*/
} tWriteDataByIdentifierInfo;

typedef enum
{
    START_ERASE_MEMORY_ROUTINE_CONTROL,       /*check erase memory routine control*/
    START_CHECK_SUM_ROUTINE_CONTROL,          /*check sum routine control*/
    RESULT_ERASE_MEMORY_ROUTINE_CONTROL,       /*check erase memory routine control*/
    RESULT_CHECK_SUM_ROUTINE_CONTROL,          /*check sum routine control*/
} tCheckRoutineCtlInfo;

typedef struct
{
    tUdsId xUdsId;
    tUdsLen xDataLen;
    uint8_t aDataBuf[512u];
    /*tx message call back*/
    void (*pfUDSTxMsgServiceCallBack)(uint8_t);
} tUdsAppMsgInfo;

typedef struct UDSServiceInfo
{
    uint8_t SerNum;     /*service num. eg 0x3e/0x87...*/
    uint8_t SessionMode;/*default session / program session / extend session*/
    uint8_t SupReqMode; /*support physical / function addr*/
    uint8_t ReqLevel;   /*request level.Lock/unlock*/
    void (*pfSerNameFun)(struct UDSServiceInfo*, tUdsAppMsgInfo *);//根据UDSServiceInfo解析将需要回应的信息装入tUdsAppMsgInfo
} tUDSService;

/*uds negative value define*/
#define NEGTIVE_ID (0x7Fu)
#define	SNS (0x11u)          /*service not support*/
#define	SFNS (0x12u)        /*subfunction not support*/
#define	IMLOIF (0x13u)       /*incorrect message length or invalid format*/
#define	BRR (0x21u)          /*busy repeat request*/
#define	CNC (0x22u)          /*conditions not correct*/
#define	RSE (0x24u)          /*request 	sequence error*/
#define	ROOR (0x31u)         /*request out of range*/
#define	SAD (0x33u)          /*security access denied*/
#define	IK (0x35u)           /*invalid key*/
#define	ENOA (0x36u)         /*exceed number of attempts*/
#define	RTDNE (0x37u)         /*required time delay not expired*/
#define	RCRRP (0x78u)        /*request correctly received-response pending*/

/*define session mode*/
#define DEFALUT_SESSION (1u << 0u)       /*default session*/
#define PROGRAM_SESSION (1u << 1u)       /*program session*/
#define EXTEND_SESSION (1u << 2u)        /*extend session*/

/*security request*/
#define NONE_SECURITY (1u << 0u)                          /*none security can request*/
#define SECURITY_LEVEL_1 ((1 << 1u))      /*security level 1 request*/
#define SECURITY_LEVEL_2 ((1u << 2u))  /*security level 2 request*/

#define DOWLOAD_DATA_ADDR_LEN (4u)      /*dowload data addr len*/
#define DOWLOAD_DATA_LEN (4u)           /*dowload data len*/

/*support function/physical ID request*/
#define ERRO_REQUEST_ID (0u)             /*received ID failled*/
#define SUPPORT_PHYSICAL_ADDR (1u << 0u) /*support physical ID request */
#define SUPPORT_FUNCTION_ADDR (1u << 1u)  /*support function ID request*/

/** flashloader download step */
typedef enum
{
    FL_REQUEST_STEP,      /*flash request step*/
    FL_TRANSFER_STEP,     /*flash transfer data step*/
    FL_EXIT_TRANSFER_STEP,/*exit transfter data step*/
    FL_CHECKSUM_STEP      /*check sum step*/

}tFlDownloadStepType;
void UDS_SystemTickCtl(void);
void UDS_MainFun(void);

#endif

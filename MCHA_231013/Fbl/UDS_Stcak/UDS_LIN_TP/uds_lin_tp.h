#ifndef UDS_CAN_TP_H
#define UDS_CAN_TP_H

#include "BAT32A237.h"
#include "common_type.h"
#include "lin_protocol.h"

#define MASTER_REQUEST_ID   0x3c
#define SLAVER_RESPOND_ID   0x3d
#define LIN_NAD             0x07        /*�ӻ��ڵ���?*/
#define LIN_BROADCAST_ID    0x7f        /*�㲥�ڵ���?*/
#define LIN_FUNC_ID    		0x7e		/*功能ID*/
/*********************************************************
**	SF	-- 	signle frame
**	FF	--	first frame
**	CF	--	consective frame
*********************************************************/

typedef enum
{
	IDLE,      /*idle*/
	RX_SF,   /*wait signle frame*/
	RX_FF,   /*wait first frame*/
	RX_CF,   /*wait consective frame*/

	TX_SF,     /*tx signle frame*/
	TX_FF,     /*tx first frame*/
	TX_CF,     /*tx consective frame*/

	WAITTING_TX, /*watting tx message*/

	WAIT_CONFIRM /*wait confrim*/
}tLinTpWorkStatus;

typedef enum
{
	SF=0,        /*signle frame value*/
	FF=1,        /*first frame value*/
	CF=2,        /*consective frame value*/
}tNetWorkFrameType;

typedef enum
{
	CONTINUE_TO_SEND, /*continue to send*/
	WAIT_FC,          /*wait flow control*/
	OVERFLOW_BUF      /*overflow buf*/
}tFlowStatus;

typedef enum
{
	N_OK = 0,    /*This value means that the service execution has completed successfully; it can be issued to a service user on both the sender and receiver side*/
	N_TIMEOUT_A, /*This value is issued to the protocol user when the timer N_Ar/N_As has passed its time-out 
	                        value N_Asmax/N_Armax; it can be issued to service user on both the sender and receiver side.*/
	N_TIMEOUT_Bs, /*This value is issued to the service user when the timer N_Bs has passed its time-out value 
                                N_Bsmax; it can be issued to the service user on the sender side only.*/
	N_TIMEOUT_Cr, /*This value is issued to the service user when the timer N_Cr has passed its time-out value
					N_Crmax; it can be issued to the service user on the receiver side only.*/
	N_WRONG_SN,   /*This value is issued to the service user upon reception of an unexpected sequence number
					(PCI.SN) value; it can be issued to the service user on the receiver side only.*/
	N_INVALID_FS, /*This value is issued to the service user when an invalid or unknown FlowStatus value has
					been received in a flow control (FC) N_PDU; it can be issued to the service user on the sender side only.*/
	N_UNEXP_PDU,  /*This value is issued to the service user upon reception of an unexpected protocol data unit;
					it can be issued to the service user on the receiver side only.*/
	N_WTF_OVRN,   /*This value is issued to the service user upon reception of flow control WAIT frame that
					exceeds the maximum counter N_WFTmax.*/
	N_BUFFER_OVFLW, /*This value is issued to the service user upon reception of a flow control (FC) N_PDU with
 					FlowStatus = OVFLW. It indicates that the buffer on the receiver side of a segmented
 					message transmission cannot store the number of bytes specified by the FirstFrame
 					DataLength (FF_DL) parameter in the FirstFrame and therefore the transmission of the
 					segmented message was aborted. It can be issued to the service user on the sender side
 					only.*/
	N_ERROR       /*This is the general error value. It shall be issued to the service user when an error has been
					detected by the network layer and no other parameter value can be used to better describe
					the error. It can be issued to the service user on both the sender and receiver side.*/
}tN_Result;

typedef enum{
	LINTP_TX_MSG_IDLE = 0, /*LIN TP tx message idle*/
	LINTP_TX_MSG_SUCC,     /*LIN TP tx message successful*/
	LINTP_TX_MSG_FAIL,     /*LIN TP tx message fail*/
	LINTP_TX_MSG_WAITTING /*LIN TP waitting tx message*/
}tLinTPTxMsgStatus;

typedef uint32_t tUdsId;
typedef uint32_t tUdsLen;
typedef uint16_t tNetTime;
typedef uint16_t tBlockSize;
typedef void (*tpfNetTxCallBack)(void);
typedef uint8_t (*tNetTxMsg)(const tUdsId, const uint16_t, const uint8_t *, const uint32_t);
typedef uint8_t (*tNetRx)(tUdsId *, tUdsLen *, uint8_t *);
typedef uint16_t tLinTpDataLen;
typedef void (*tpfAbortTxMsg)(void);
typedef struct
{
	uint8_t ucCalledPeriod;      /*called CAN tp main function period*/
	tUdsId master_request_id;    /*master request id*/
    tUdsId slaver_respond_id;    /*slaver respond id*/
    tUdsId lin_nad;              /*lin_nad*/
	tBlockSize xBlockSize;       /*BS*/
	tNetTime xSTmin;             /*STmin*/
	tNetTime xNAs;               /*N_As*/
	tNetTime xNAr;               /*N_Ar*/
	tNetTime xNBs;               /*N_Bs*/
	tNetTime xNBr;               /*N_Br*/
	tNetTime xNCs;               /*N_Cs*/
	tNetTime xNCr;               /*N_Cr*/
	uint32_t txBlockingMaxTimeMs;/*TX message blocking max time (MS)*/
	tNetTxMsg pfNetTxMsg;        /*net tx message with non blocking*/
	tNetRx pfNetRx;              /*net rx*/
}tUdsLINNetLayerCfg;

typedef struct
{
	uint32_t TxMsgID;       /*Tx message ID*/
	uint32_t TxMsgLength;   /*TX message length*/
	uint32_t TxMsgCallBack; /*Tx message callback*/
}tTPTxMsgHeader;

#define MAX_DATA_RX_LEN (512u)   /*缓存内的最大数据长�? */
#define MAX_DATA_TX_LEN (128u)
typedef struct
{
	tUdsId xLinTpId;                        /*lin tp message id*/
	tLinTpDataLen xPduDataLen;              /*pdu data len(Rx/Tx data len)*/
	tLinTpDataLen xFFDataLen;               /*Rx/Tx FF data len*/
	uint8_t aDataBuf[MAX_DATA_TX_LEN];         /*Rx/Tx data buf*/
}tLinTpTxDataInfo;
typedef struct
{
	tUdsId xLinTpId;                        /*lin tp message id*/
	tLinTpDataLen xPduDataLen;              /*pdu data len(Rx/Tx data len)*/
	tLinTpDataLen xFFDataLen;               /*Rx/Tx FF data len*/
	uint8_t aDataBuf[MAX_DATA_RX_LEN];         /*Rx/Tx data buf*/
}tLinTpRxDataInfo;
typedef struct
{
	uint8_t ucSN;          /*SN*/
	uint8_t ucBlockSize;   /*Block size*/
	tNetTime xSTmin;             /*STmin*/
	tNetTime xMaxWatiTimeout;    /*timeout time*/
	tLinTpTxDataInfo stLinTpTxDataInfo;
    tLinTpRxDataInfo stLinTpRxDataInfo;
}tLinTpInfo;

extern volatile tLinTpInfo gs_stLinTPTxRxDataInfo; /*数据传输层数据保存用缓存，收发分时复�?*/
extern const tUdsLINNetLayerCfg g_stLINUdsNetLayerCfgInfo;
bool TP_DriverWriteDataInTP(const uint32_t i_RxID, const uint32_t i_RxDataLen, const uint8_t *i_pRxDataBuf);
bool IsRxMsgReally(void);
void ClearRxMsgReadlly(void);
bool IsTxMsgReally(void);
void ClearTxMsgReadlly(void);
void SetTxMsgReadlly(void);
void SetRxMsgReadlly(void);
uint8_t LINTP_TxMsg(const tUdsId i_xTxId,
                      const uint16_t i_DataLen, 
                      const uint8_t* i_pDataBuf, 
                      const uint32_t txBlockingMaxtime);
uint8_t LINTP_RxMsg(tUdsId * o_pxRxId,
                     tUdsLen * o_pRxDataLen,
                     uint8_t *o_pRxBuf);
#endif

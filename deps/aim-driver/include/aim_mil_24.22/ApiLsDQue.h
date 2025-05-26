/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsDQueue.h
 *
 *  Data Queue: Defines, Structures and Functions
 *
 */

#ifndef API_LS_DQUE_H
#define API_LS_DQUE_H


/* function ApiCmdDataQueueOpen() */
#define API_DATA_QUEUE_ID_BM_REC_BIU1   0
#define API_DATA_QUEUE_ID_BM_REC_BIU2   1
#define API_DATA_QUEUE_ID_BM_REC_BIU3   2
#define API_DATA_QUEUE_ID_BM_REC_BIU4   3
#define API_DATA_QUEUE_ID_BM_REC_BIU5   4
#define API_DATA_QUEUE_ID_BM_REC_BIU6   5
#define API_DATA_QUEUE_ID_BM_REC_BIU7   6
#define API_DATA_QUEUE_ID_BM_REC_BIU8   7

#define API_DATA_QUEUE_ID_MIL_SCOPE     8
#define API_DATA_QUEUE_GEN_ASP          9
#define MAX_DATA_QUEUE_ID               API_DATA_QUEUE_GEN_ASP


#define API_DATA_QUEUE_CTRL_MODE_START  0
#define API_DATA_QUEUE_CTRL_MODE_STOP   1
#define API_DATA_QUEUE_CTRL_MODE_RESUME 2
#define API_DATA_QUEUE_CTRL_MODE_FLUSH  3
#define MAX_DATA_QUEUE_CONTROL_MODE     API_DATA_QUEUE_CTRL_MODE_FLUSH

/* function ApiCmdDataQueueRead() */
#define API_DATA_QUEUE_STATUS_ASP_OVERFLOW  0x00008000
#define API_DATA_QUEUE_STATUS_LOC_OVERFLOW  0x00000001
#define API_DATA_QUEUE_STATUS_REM_OVERFLOW  0x00000002
#define API_DATA_QUEUE_STATUS_LOC_BUF_ERR   0x00000004
#define API_DATA_QUEUE_STATUS_REM_BUF_ERR   0x00000008
#define API_DATA_QUEUE_STATUS_CHN_OPERATING 0x00600000
#define API_DATA_QUEUE_STATUS_CAPMODE       0x00800000
#define API_DATA_QUEUE_STATUS_ENABLED       0x80000000
#define API_DATA_QUEUE_STATUS_RESUMED       0x20000000


/* TSW data queue header offset */
#define API_DATA_QUEUE_TSW_OFFSET_STATUS    0x00
#define API_DATA_QUEUE_TSW_OFFSET_PUT       0x04
#define API_DATA_QUEUE_TSW_OFFSET_GET       0x08
#define API_DATA_QUEUE_TSW_OFFSET_START     0x0C
#define API_DATA_QUEUE_TSW_OFFSET_SIZE      0x10




typedef struct ty_api_data_queue_status
{
    AiUInt32 status;
    AiUInt32 bytes_transfered;
    AiUInt32 bytes_in_queue;
    AiUInt64 total_bytes_transfered;
} TY_API_DATA_QUEUE_STATUS;

typedef struct ty_api_queue_table
{
    AiUInt8                  uc_QueueFlag;
    AiUInt32                 ul_QueueStart;
    TY_API_DATA_QUEUE_STATUS x_QueueStatus;
} TY_API_QUEUE_TABLE;

typedef struct ty_api_data_queue_read
{
    AiUInt32 id;
    void     *buffer;
    AiUInt32 bytes_to_read;
} TY_API_DATA_QUEUE_READ;

typedef struct ty_api_data_queue_write
{
	AiUInt8  uc_Id;
	AiUInt8  uc_Padding1;
	AiUInt16 uw_Padding2;
	void     *pv_MemBuf;
	AiUInt32 ul_BytesToWrite;
	TY_API_DATA_QUEUE_STATUS x_Info;
} TY_API_DATA_QUEUE_WRITE;


/****************************************************************
**    Data Queue Functions
****************************************************************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDataQueueOpen(AiUInt32 ul_Module, AiUInt32 id, AiUInt32 * queue_size);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDataQueueClose(AiUInt32 ul_Module, AiUInt32 id );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDataQueueControl(AiUInt32 ul_Module, AiUInt32 id, AiUInt32 mode);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDataQueueRead(AiUInt32 ul_Module, TY_API_DATA_QUEUE_READ *px_QueueData, TY_API_DATA_QUEUE_STATUS * info);

/* The following functions are called by the ANS server. */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiAnsDataQueueOpen(AiUInt32 ul_Module, AiUInt32 id, AiUInt32 * queue_size);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiAnsDataQueueClose(AiUInt32 ul_Module, AiUInt32 id);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiAnsDataQueueControl(AiUInt32 ul_Module, AiUInt32 id, AiUInt32 mode);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiAnsDataQueueRead(AiUInt32 ul_Module, TY_API_DATA_QUEUE_READ *px_QueueData, TY_API_DATA_QUEUE_STATUS * info);

AiInt16 ApiCmdDataQueueOpen_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id, AiUInt32 * queue_size );
AiInt16 ApiCmdDataQueueClose_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id );
AiInt16 ApiCmdDataQueueControl_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id, AiUInt32 mode);
AiInt16 ApiCmdDataQueueRead_(AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_DATA_QUEUE_READ *px_QueueData, TY_API_DATA_QUEUE_STATUS * info);


AiUInt8 _ApiCmdDataQueueDirectIsHs(AiUInt8 ucDeviceType, AiUInt8 ucId);
AiUInt8 _ApiCmdDataQueueDirectGetBiu(AiUInt8 ucId);
AiUInt8 _ApiCmdDataQueueDirectIsBmRecording(AiUInt8 ucId);

AiInt16 ApiCmdDataQueueOpenDirect_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id, AiUInt32 * queue_size);
AiInt16 ApiCmdDataQueueCloseDirect_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id );
AiInt16 ApiCmdDataQueueControlDirect_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 id, AiUInt32 mode);
AiInt16 ApiCmdDataQueueReadDirect_(AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_DATA_QUEUE_READ *px_QueueData, TY_API_DATA_QUEUE_STATUS * info);


AiInt16 ApiCmdDataQueueOpenBuffer_(AiUInt32 module, AiUInt8 biu, AiUInt32 id, AiUInt32 * queue_size);
AiInt16 ApiCmdDataQueueCloseBuffer_(AiUInt32 module, AiUInt8 biu, AiUInt32 id );
AiInt16 ApiCmdDataQueueControlTsw_(AiUInt32 module, AiUInt8 biu, AiUInt32 id, AiUInt32 mode);
AiInt16 ApiCmdDataQueueReadBuffer_(AiUInt32 module, AiUInt8 biu, TY_API_DATA_QUEUE_READ *px_QueueData, TY_API_DATA_QUEUE_STATUS * info);

#endif /*API_LS_DQUE_H*/

/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsBuf.h
 *
 *  This header file contains declarations for
 *  buffer related API functionality
 *
 */

#ifndef AI1553_LS_BUF_H
#define AI1553_LS_BUF_H 



/************/
/* defines  */
/************/

/* function ApiCmdBufDef() */
#define API_BUF_BC_MSG                1
#define API_BUF_RT_MSG                2
/* alt */
#define API_BC_BUF 1
#define API_RT_BUF 2

#define API_BUF_WRITE_TO_CURRENT      0
#define API_BUF_WRITE_TO_NEXT         0xffff


#define API_BUF_READ_FROM_CURRENT     0
#define API_BUF_READ_FROM_LAST        0xffff

#define API_BUF_HOST_CONTROL_NEXT_BUFFER 0

/* The following defines are redundant as MEM_TYPE_* in Api_defs.h, Ai_def.h */
#define API_MEMTYPE_GLOBAL            AI_MEMTYPE_GLOBAL
#define API_MEMTYPE_SHARED            AI_MEMTYPE_SHARED
#define API_MEMTYPE_LOCAL             AI_MEMTYPE_LOCAL
#define API_MEMTYPE_IO                AI_MEMTYPE_IO
#define API_MEMTYPE_GLOBAL_DIRECT     AI_MEMTYPE_GLOBAL_DIRECT
#define API_MEMTYPE_GLOBAL_EXTENSION  AI_MEMTYPE_GLOBAL_EXTENSION

#define API_REP_REC_AREA_OFFSET       0x10000

/* function ApiBHModify() */
#define API_BSIZEM_STATIC           0
#define API_BSIZEM_DYNAMIC          1


/**************************/
/* structs and typedefs   */
/**************************/

typedef struct ty_api_bh_modify
{
    AiUInt32 ul_BHAddr;
    AiUInt32 ul_DataBufferStoreMode;
    AiUInt32 ul_BufferSizeMode;
    AiUInt32 ul_BufferSize;
    AiUInt32 ul_BufferQueueMode;
    AiUInt32 ul_BufferQueueSize;
    AiUInt32 ul_StatusQueueSize;
    AiUInt32 ul_EventQueueSize;
    AiUInt32 ul_CurrentBufferIndex;
} TY_API_BH_MODIFY;


typedef struct ty_api_c1760_con
{
    AiUInt8  mode;
    AiUInt8  c01;
    AiUInt16 buf_id;
    AiUInt8  c02[MAX_API_C1760_C02];
} TY_API_C1760_CON;


/**************************/
/* BC interface functions */
/**************************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufDef            (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid,
                                                           AiUInt16 bid, AiUInt8 len, AiUInt16 *data, AiUInt16 *rid, AiUInt32 *raddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufRead           (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid,
                                                           AiUInt16 bid, AiUInt8 len, AiUInt16 *data, AiUInt16 *rid, AiUInt32 *raddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRamWriteDataset   (AiUInt32 bModule, AiUInt16 dsid, AiUInt16 *data);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRamReadDataset    (AiUInt32 bModule, AiUInt16 dsid, AiUInt16 *data);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadMemData          (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiWriteMemData         (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadBlockMemData     (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p, AiUInt32 size, AiUInt32 *pul_BytesRead);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiWriteBlockMemData    (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p, AiUInt32 size, AiUInt32 *pul_BytesWritten);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufWrite          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt16 bid,
                                                           AiUInt8 data_pos, AiUInt8 bit_pos, AiUInt8 bit_len, AiUInt16 data);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufWriteBlock     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid,
                                                          AiUInt16 bid, AiUInt8 offset, AiUInt8 len, AiUInt16* data);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufHostControl    (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt32 con);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBufC1760Con       (AiUInt32 bModule, AiUInt8 biu, TY_API_C1760_CON *pc1760);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiBHModify             (AiUInt32 bModule, TY_API_BH_MODIFY *px_BHInfo);



/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdBufDef_         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt16 bid, AiUInt8 len, AiUInt16 *data, AiUInt16 *rid, AiUInt32 *raddr);
AiInt16 ApiCmdBufRead_        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt16 bid, AiUInt8 len, AiUInt16 *data, AiUInt16 *rid, AiUInt32 *raddr);
AiInt16 ApiCmdRamWriteDataset_(AiUInt32 bModule, AiUInt16 dsid, AiUInt16 *data);
AiInt16 ApiCmdRamReadDataset_ (AiUInt32 bModule, AiUInt16 dsid, AiUInt16 *data);
AiInt16 ApiReadMemData_       (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p);
AiInt16 ApiWriteMemData_      (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p);
AiInt16 ApiReadBlockMemData_  (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p, AiUInt32 size, AiUInt32 *pul_BytesRead);
AiInt16 ApiWriteBlockMemData_ (AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt8 width, void* data_p, AiUInt32 size, AiUInt32 *pul_BytesWritten);
AiInt16 ApiCmdBufWrite_       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt16 bid, AiUInt8 data_pos, AiUInt8 bit_pos, AiUInt8 bit_len, AiUInt16 data);
AiInt16 ApiCmdBufWriteBlock_  (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt16 bid, AiUInt8 offset, AiUInt8 len, AiUInt16* data);
AiInt16 ApiCmdBufHostControl_ (AiUInt32 bModule, AiUInt8 biu, AiUInt8 bt, AiUInt16 hid, AiUInt32 con); 
AiInt16 ApiCmdBufC1760Con_    (AiUInt32 bModule, AiUInt8 biu, TY_API_C1760_CON *pc1760);
AiInt16 ApiBHModify_          (AiUInt32 bModule, TY_API_BH_MODIFY *px_BHInfo);

#endif /*AI1553_LS_BUF_H*/

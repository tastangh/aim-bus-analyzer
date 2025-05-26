/* SPDX-FileCopyrightText: 2016-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsQue.h
 *
 *  This header file contains declarations for
 *  queueing API functionality
 *
 */


#ifndef API_LS_QUE_H
#define API_LS_QUE_H


/************/
/* defines  */
/************/





/**************************/
/* structs and typedefs   */
/**************************/

typedef struct ty_api_queue_buf
{
    AiUInt8  rt_addr;
    AiUInt8  sa_mc;
    AiUInt8  sa_type;
    AiUInt8  rt_addr2;
    AiUInt8  sa_mc2;
    AiUInt8  sa_type2;
    AiUInt8  word_cnt;
    AiUInt8  rbf_trw;
    AiUInt16 msg_trw ;
    AiUInt16 status_word1;
    AiUInt16 status_word2;
    AiUInt16 buffer[32];
    AiUInt32 ttag;
} TY_API_QUEUE_BUF;

typedef struct ty_api_queue_buf_ex
{
    AiUInt8  rt_addr;
    AiUInt8  sa_mc;
    AiUInt8  sa_type;
    AiUInt8  rt_addr2;
    AiUInt8  sa_mc2;
    AiUInt8  sa_type2;
    AiUInt8  word_cnt;
    AiUInt8  rbf_trw;
    AiUInt16 msg_trw;
    AiUInt16 status_word1;
    AiUInt16 status_word2;
    AiUInt16 buffer[32];
    AiUInt32 ttag_hi;
    AiUInt32 ttag_lo;
} TY_API_QUEUE_BUF_EX;

/***********************/
/* interface functions */
/***********************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdQueueIni          (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdQueueRead         (AiUInt32 bModule, AiUInt8 biu, TY_API_QUEUE_BUF *qmesg);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdQueueFlush        (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdQueueStart        (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdQueueHalt         (AiUInt32 bModule, AiUInt8 biu);


/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdQueueIni_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdQueueFlush_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdQueueStart_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdQueueHalt_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdQueueRead_(AiUInt32 bModule, AiUInt8 biu, TY_API_QUEUE_BUF *qmesg);

#endif


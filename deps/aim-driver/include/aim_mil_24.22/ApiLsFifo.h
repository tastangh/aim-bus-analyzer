/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsFifo.h
 *
 *  This header file contains declarations for
 *  ls fifo related API functionality
 *
 */

#ifndef AI1553_LS_FIFO_H
#define AI1553_LS_FIFO_H 



/************/
/* defines  */
/************/


/**************************/
/* structs and typedefs   */
/**************************/


/***********************/
/* interface functions */
/***********************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdFifoIni           (AiUInt32 bModule, AiUInt8 biu, AiUInt8 fifo_nbr, AiUInt16 buf_nbr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdFifoWrite         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 f_id, AiUInt16 size, AiUInt16 *data);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdFifoReadStatus    (AiUInt32 bModule, AiUInt8 biu, AiUInt8 f_id, AiUInt16 *status);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCAssignFifo      (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 f_id, AiUInt16 xid);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSAAssignFifo    (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 f_id, AiUInt8 rt, AiUInt8 sa);



/**********************/
/* internal functions */
/**********************/
AiInt16 ApiCmdFifoIni_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 fifo_nbr, AiUInt16 buf_nbr);
AiInt16 ApiCmdFifoWrite_(AiUInt32 ulModHandle, AiUInt8 biu, AiUInt8 f_id, AiUInt16 size, AiUInt16 *data);
AiInt16 ApiCmdFifoReadStatus_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 f_id, AiUInt16 *b_status);
AiInt16 ApiCmdBCAssignFifo_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 f_id, AiUInt16 xid);
AiInt16 ApiCmdRTSAAssignFifo_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 f_id, AiUInt8 rt, AiUInt8 sa);

#endif

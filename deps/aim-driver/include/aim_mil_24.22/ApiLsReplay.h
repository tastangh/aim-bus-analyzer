/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsCal.h
 *
 *  This header file contains declarations for
 *  replay related API functionality
 *
 */

#ifndef AI1553_LS_REPLAY_H
#define AI1553_LS_REPLAY_H 



/************/
/* defines  */
/************/


/* function ApiCmdReplayIni() */
#define API_REP_NO_ABS_TIME           0
#define API_REP_ABS_TIME              1

#define API_REP_RLT_LOW_TT            0
#define API_REP_RLT_ALL               1

#define API_REP_NO_INT                0
#define API_REP_HFI_INT               1

#define API_REP_HALTED                0
#define API_REP_BUSY                  1

/* Replay Defs */
#define TG_EXT_SET_REPLAY_ADDR 5





/**************************/
/* structs and typedefs   */
/**************************/




typedef struct ty_api_rep_status
{
    AiUInt8  status;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 rpi_cnt;
    AiUInt32 saddr;
    AiUInt32 size;
    AiUInt32 entry_cnt;
} TY_API_REP_STATUS;





/***********************/
/* interface functions */
/***********************/


/* Replay Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReplayIni         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 cet,
                                                           AiUInt8 nct, AiUInt8 cyc, AiUInt8 nimg, AiUInt8 alt, AiUInt8 rlt,
                                                           AiUInt8 rint, AiUInt32 min, AiUInt32 msec, AiUInt8 sign,
                                                           AiUInt32 fsize);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReplayStart       (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReplayStop        (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReplayStatus      (AiUInt32 bModule, AiUInt8 biu, TY_API_REP_STATUS *prep);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReplayRT          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 mode, AiUInt8 rt);




/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdReplayIni_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 cet, AiUInt8 nct, AiUInt8 cyc, AiUInt8 nimg, AiUInt8 alt, AiUInt8 rlt, AiUInt8 rint, AiUInt32 min, AiUInt32 msec, AiUInt8 sign, AiUInt32 fsize);
AiInt16 ApiCmdReplayStart_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdReplayStop_(AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdReplayStatus_(AiUInt32 bModule, AiUInt8 biu, TY_API_REP_STATUS *prep);
AiInt16 ApiCmdReplayRT_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 mode, AiUInt8 rt);

#endif

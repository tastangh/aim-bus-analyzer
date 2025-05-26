/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsCal.h
 *
 *  This header file contains declarations for
 *  calibration related API functionality
 *
 */

#ifndef AI1553_LS_CAL_H
#define AI1553_LS_CAL_H 



/************/
/* defines  */
/************/

/* function ApiCmdCalSigCon() */
#define API_CAL_BUS_PRIMARY           1
#define API_CAL_BUS_SECONDARY         2


/* function ApiCmdCalXmtCon() */
#define API_CAL_XMT_FULL_RANGE_PRI    1
#define API_CAL_XMT_FULL_RANGE_SEC    2
#define API_CAL_XMT_HIGHRES_RANGE     3



/* function ApiCmdCalCplCon() */
#define API_CAL_CPL_ISOLATED          0
#define API_CAL_CPL_TRANSFORM         1
#define API_CAL_CPL_DIRECT            2
#define API_CAL_CPL_EXTERNAL          3
#define API_CAL_CPL_WRAP_AROUND_LOOP      4
#define API_CAL_CPL_EXT_WRAP_AROUND_LOOP  5
/* alt */
#define API_CPL_ISO    0
#define API_CPL_TRF    1
#define API_CPL_DIR    2
#define API_CPL_NET    3
#define API_CPL_WRAP   4



/* function ApiCmdCalTransCon() */
#define API_CAL_TRANS_1M            0
#define API_CAL_TRANS_500K          1

/**************************/
/* structs and typedefs   */
/**************************/






/***********************/
/* interface functions */
/***********************/


AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdCalSigCon(   AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 con);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdCalCplCon(   AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 cpl);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdCalXmtCon(   AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 amp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdCalTransCon( AiUInt32 bModule, AiUInt8 biu, AiUInt8 trans_mode );




/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdCalSigCon_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 con);
AiInt16 ApiCmdCalCplCon_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 cpl);
AiInt16 ApiCmdCalXmtCon_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 bus, AiUInt8 amp);
AiInt16 ApiCmdCalTransCon_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 trans_mode);

#endif /*AI1553_LS_CAL_H*/

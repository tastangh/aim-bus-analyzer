/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsTrack.h
 *
 *  This header file contains declarations for
 *  tracking API functionality
 *
 */


#ifndef API_LS_TRACK_H
#define API_LS_TRACK_H


/************/
/* defines  */
/************/


/* function ApiCmdTrackDef() */
#define API_TRACK_XFERID_MASK        0xFFFF
#define API_TRACK_RTADDR_MASK        0x7F00
#define API_TRACK_SA_MID_MASK        0x00FF
#define API_TRACK_XFERID_POS         0
#define API_TRACK_RTADDR_POS         8
#define API_TRACK_SA_MID_POS         0

#define API_TRACK_TYPE_BC            API_BC_MODE
#define API_TRACK_TYPE_RT            API_RT_MODE
#define API_TRACK_TYPE_MUX_TRACK     (API_RT_MODE+1)


/* function ApiCmdTrackRead() */
#define API_TRACK_DONT_CLR_UDF      0
#define API_TRACK_CLR_UDF           1

#define API_DATA_NOT_VALID          0
#define API_DATA_VALID              1




/**************************/
/* structs and typedefs   */
/**************************/

typedef struct ty_api_track_def_in
{
    AiUInt32 ul_TrackId;
    AiUInt32 ul_BT;
    AiUInt32 ul_XidRtSa;
    AiUInt32 ul_Mode;
    AiUInt32 ul_TrackStartPos;
    AiUInt32 ul_TrackBPos;
    AiUInt32 ul_TrackBLen;
    AiUInt32 ul_TrackLen;
    AiUInt32 ul_MultiplexedTrackNb;
    AiUInt32 ul_ContinousTrackNb;
    AiInt32  l_Offset;
} TY_API_TRACK_DEF_IN;

typedef struct ty_api_track_def_out
{
    AiUInt32 ul_TrackBufferStartAddr;
} TY_API_TRACK_DEF_OUT;

typedef struct ty_api_track_read_in
{
    AiUInt32 ul_TrackId;
    AiUInt32 ul_MultiplexedTrackIndex;
    AiUInt32 ul_ReadMode;
} TY_API_TRACK_READ_IN;

typedef struct ty_api_track_read_out
{
    AiUInt32 ul_DataValid;
    AiUInt32 ul_LastTT;
    AiUInt16 *puw_TrackDataWords;
} TY_API_TRACK_READ_OUT;

typedef struct ty_api_track_scan_in
{
    AiUInt32 ul_TrackId;
    AiUInt32 ul_ChunkNb;
    AiUInt32 ul_ChunkSize;
} TY_API_TRACK_SCAN_IN;

typedef struct ty_api_track_scan_out
{
    AiUInt32 ul_NumberOfReturnedStates;
    AiUInt32 ul_MoreData;
    AiUInt32 *pul_ReturnedStates;
} TY_API_TRACK_SCAN_OUT;

typedef struct ty_api_track_prealloc_in
{
    AiUInt32 ul_TrackId;
    AiUInt32 ul_PreAllocNb;
    AiUInt32 *pul_MuxStates;
} TY_API_TRACK_PREALLOC_IN;

typedef struct ty_api_track_prealloc_out
{
    AiUInt32 *pul_TrackBufferStartAddr;
} TY_API_TRACK_PREALLOC_OUT;



/***********************/
/* interface functions */
/***********************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackDef          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 uc_TrackId,
                                                           AiUInt8 uc_BT, AiUInt16 uw_XidRtSa, AiUInt8 uc_Mode,
                                                           AiUInt16 uw_TrackStartPos, AiUInt8 uc_TrackBPos,
                                                           AiUInt8 uc_TrackBLen, AiUInt16 uw_TrackLen,
                                                           AiUInt16 uw_MultiplexedTrackNb, AiUInt16 uw_ContinousTrackNb,
                                                           AiUInt32 *pul_TrackBufferStartAddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackDefEx        (AiUInt32 bModule, AiUInt8 biu, 
                                                           TY_API_TRACK_DEF_IN  *px_TrackDefIn,
                                                           TY_API_TRACK_DEF_OUT *px_TrackDefOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackRead         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 uc_TrackId, 
                                                           AiUInt16 uw_MultiplexedTrackIndex, AiUInt8 uc_ReadMode,
                                                           AiUInt8 *puc_DataValid, AiUInt16 *puw_TrackDataWords);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackReadEx       (AiUInt32 bModule, AiUInt8 biu, 
                                                           TY_API_TRACK_READ_IN *px_TrackReadIn,
                                                           TY_API_TRACK_READ_OUT *px_TrackReadOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackScan         (AiUInt32 bModule, AiUInt8 biu, 
                                                           TY_API_TRACK_SCAN_IN *px_TrackScanIn,
                                                           TY_API_TRACK_SCAN_OUT *px_TrackScanOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdTrackPreAlloc     (AiUInt32 bModule, AiUInt8 biu, 
                                                           TY_API_TRACK_PREALLOC_IN *px_TrackPreAllocIn,
                                                           TY_API_TRACK_PREALLOC_OUT *px_TrackPreAllocOut );



/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdTrackDefEx_   (AiUInt32 bModule, AiUInt8 biu, TY_API_TRACK_DEF_IN  *px_TrackDefIn, TY_API_TRACK_DEF_OUT *px_TrackDefOut );
AiInt16 ApiCmdTrackReadEx_  (AiUInt32 bModule, AiUInt8 biu, TY_API_TRACK_READ_IN *px_TrackReadIn, TY_API_TRACK_READ_OUT *px_TrackReadOut );
AiInt16 ApiCmdTrackScan_    (AiUInt32 bModule, AiUInt8 biu, TY_API_TRACK_SCAN_IN *px_TrackScanIn, TY_API_TRACK_SCAN_OUT *px_TrackScanOut );
AiInt16 ApiCmdTrackPreAlloc_(AiUInt32 bModule, AiUInt8 biu, TY_API_TRACK_PREALLOC_IN *px_TrackPreAllocIn, TY_API_TRACK_PREALLOC_OUT *px_TrackPreAllocOut );

#endif
/* end: ApiLsTrack.h */

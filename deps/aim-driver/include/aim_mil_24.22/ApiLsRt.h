/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsRt.h
 *
 *  This header file contains declarations for
 *  RT (remote terminal) related API functionality
 *
 */

#ifndef AI1553_LS_RT_H
#define AI1553_LS_RT_H 



/************/
/* defines  */
/************/


/* function ApiCmdRTIni() */
#define API_RT_DISABLE_OPERATION      0
#define API_RT_ENABLE_SIMULATION      1
#define API_RT_ENABLE_MONITORING      2
#define API_RT_ENABLE_DUAL_REDUNDANT  3

#define API_RT_RSP_BOTH_BUSSES        0
#define API_RT_RSP_PRI_BUS            1
#define API_RT_RSP_SEC_BUS            2

/* function ApiCmdRTSACon() */
#define API_RT_TYPE_RECEIVE_SA        0
#define API_RT_TYPE_TRANSMIT_SA       1
#define API_RT_TYPE_RECEIVE_MODECODE  2
#define API_RT_TYPE_TRANSMIT_MODECODE 3
#define API_RT_TYPE_ALL               4 /* used for ApiCmdBMRTSAActRead */
/* alt */
#define API_RT_SA_TYPE_RCV          0
#define API_RT_SA_TYPE_XMT          1
#define API_RT_MC_TYPE_RCV          2
#define API_RT_MC_TYPE_XMT          3


#define API_RT_DISABLE_SA             0
#define API_RT_ENABLE_SA              1
#define API_RT_ENABLE_SA_INT_XFER     2
#define API_RT_ENABLE_SA_INT_ERR      3
/* alt */
#define API_RT_SA_DIS       0
#define API_RT_SA_ENA       1
#define API_RT_SA_XFER_INT  2
#define API_RT_SA_ERR_INT   3

#define API_RT_SWM_OR                 0
#define API_RT_SWM_AND                1
#define API_RT_SWM_NXW_OR             2
#define API_RT_SWM_NXW_AND            3

/* function ApiCmdRTStatusRead() */
#define API_RT_STATUS_HALTED          1
#define API_RT_STATUS_BUSY            2

/* function ApiCmdRTSAMsgRead() */
#define API_RT_REPORT_NO_ERR          0
#define API_RT_REPORT_ERR_FRAME_COD   1
#define API_RT_REPORT_ERR_RSP_TIMEOUT 2

/* function ApiCmdRTGlobalCon() */
#define API_RT_SA_USAGE_INIT          0
#define API_RT_SA_USAGE_BUF_INIT      1
#define API_RT_SA_USAGE_UPDATE        2

/* function ApiCmdRTStart() */
/* function ApiCmdRTStop()  */
#define API_RT_MODE_HALT              1
#define API_RT_MODE_BUSY              2

/* function ApiCmdRTModeCtrl() */
#define API_RT_MODE_CONFIGURED_DYTAGS     3
#define API_RT_MODE_LANE_CTRL             4
#define API_RT_MODE_LSW_HANDLING          5   /* Implemented only in special BSP V11.11 Patch 5 for Dassault */
#define API_RT_MODE_OPERATION_CTRL        6



/**************************/
/* structs and typedefs   */
/**************************/


typedef struct ty_api_rt_bh_info
{
    AiUInt16 bid;
    AiUInt16 sid;
    AiUInt16 eid;
    AiUInt16 nbufs;
    AiUInt32 hid_addr;
    AiUInt32 bq_addr;
    AiUInt32 sq_addr;
    AiUInt32 eq_addr;
} TY_API_RT_BH_INFO;


typedef struct ty_api_rt_err
{
    AiUInt8  type;
    AiUInt8  sync;
    AiUInt8  contig;
    AiUInt8  padding1;
    AiUInt32 err_spec;
} TY_API_RT_ERR;

typedef struct ty_api_rt_dytag
{
    AiUInt16 tag_fct;
    AiUInt16 min;
    AiUInt16 max;
    AiUInt16 step;
    AiUInt16 wpos;
} TY_API_RT_DYTAG;

typedef struct ty_api_rt_status_dsp
{
    AiUInt8  status;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 glb_msg_cnt;
    AiUInt32 glb_err_cnt;
}TY_API_RT_STATUS_DSP;


typedef struct ty_api_rt_msg_dsp
{
    AiUInt16 nxw;
    AiUInt16 lsw;
    AiUInt16 lcw;
    AiUInt16 padding1;
    AiUInt32 msg_cnt;
    AiUInt32 err_cnt;
} TY_API_RT_MSG_DSP;

typedef struct ty_api_rt_sa_msg_dsp
{
    AiUInt16 bid;
    AiUInt16 trw;
    AiUInt16 lcw;
    AiUInt16 lsw;
    AiUInt32 bufp;
    AiUInt32 ttag;
} TY_API_RT_SA_MSG_DSP;

typedef struct ty_api_rt_sa_status_queue
{
    AiUInt32 ul_SqCtrlWord;
    AiUInt32 ul_SqStatusWords;
    AiUInt32 ul_SqActBufPtr;
    AiUInt32 ul_SqTimeTag;
} TY_API_RT_SA_STATUS_QUEUE;

typedef struct ty_api_rt_sa_event_queue
{
    AiUInt32 ul_EqEntryWord1;
    AiUInt32 ul_EqEntryWord2;
    AiUInt32 ul_EqEntryWord3;
    AiUInt32 ul_EqEntryWord4;
} TY_API_RT_SA_EVENT_QUEUE;

typedef struct ty_api_rt_sa_status_info
{
    AiUInt32 ul_ActBufferId;
    AiUInt32 ul_ActionWord;
    AiUInt32 ul_XferCnt;
    AiUInt32 ul_ErrCnt;
} TY_API_RT_SA_STATUS_INFO;

typedef struct ty_api_rt_sa_status_ex
{
    TY_API_RT_SA_STATUS_QUEUE      x_XferSQueue[256];
    TY_API_RT_SA_EVENT_QUEUE       x_XferEQueue[1];
    TY_API_RT_SA_STATUS_INFO       x_XferInfo;
    AiUInt32                       ul_BufferQueueSize;
} TY_API_RT_SA_STATUS_EX;

typedef struct ty_api_rt_sa_msg_read_in
{
    AiUInt32 ul_RtAddr;
    AiUInt32 ul_SA;
    AiUInt32 ul_SaType;
    AiUInt32 ul_Biu;
} TY_API_RT_SA_MSG_READ_IN;

typedef struct ty_api_rt_msg_all_dsp_rt
{
    AiUInt32 rt_msg;
    AiUInt32 rt_err;
}TY_API_RT_MSG_ALL_DSP_RT;


typedef struct ty_api_rt_msg_all_dsp
{
    TY_API_RT_MSG_ALL_DSP_RT rt[32];
} TY_API_RT_MSG_ALL_DSP;

typedef struct ty_api_rt_sa
{
    AiUInt16 buffer[32];
    AiUInt8  mode;
    AiUInt8  rt;
    AiUInt8  rt_con;
    AiUInt8  sa_mc;
    AiUInt8  sa_type;
    AiUInt8  sa_con;
    AiUInt8  resp_time;
    AiUInt8  smod;
    AiUInt16 nxw;
    AiUInt16 swm;
    AiUInt16 hid;
    AiUInt16 bid;
} TY_API_RT_SA;

typedef struct ty_api_rt_mode_ctrl
{
    AiUInt32 ul_RtMode;
    AiUInt32 ul_Ctrl;
    AiUInt32 ul_Param1;
    AiUInt32 ul_Param2;
    AiUInt32 ul_Param3;
} TY_API_RT_MODE_CTRL;

typedef struct rt_info_tag {
    AiUInt8 uc_Mode;    /* Off/Sim/Mbx                */  
    /* 0 = API_RT_DISABLE_OPERATION */
    /* 1 = API_RT_ENABLE_SIMULATION */
    /* 2 = API_RT_ENABLE_MONITORING */
    AiUInt32 ul_RxSA;
    AiUInt32 ul_TxSA;
    AiUInt32 ul_RxMC;
    AiUInt32 ul_TxMC;
    AiUInt32 ul_HSRxMID[8];
    AiUInt32 ul_HSTxMID[8];
    AiUInt32 ul_HSMC;
} TY_RT_INFO;

/**************************/
/* BC interface functions */
/**************************/


AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTIni             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 con, AiUInt8 bus, AiFloat resp_time, AiUInt16 nxw);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTEnaDis          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 con);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTRespTime        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiFloat resp_time);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTRespTimeGet     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiFloat *pResp_time);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTNXW             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 nxw);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTLCW             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 lcw);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTLSW             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 lsw);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTBHDef           (AiUInt32 bModule, AiUInt8 biu, AiUInt16 hid,
                                                           AiUInt16 bid, AiUInt16 sid, AiUInt16 eid,
                                                           AiUInt8 qsize, AiUInt8 bqm, AiUInt8 bsm,
                                                           AiUInt8 sqm, AiUInt8 eqm, AiUInt8 res,
                                                           TY_API_RT_BH_INFO *pbh); 
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSACon           (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt8 sa, AiUInt16 hid, AiUInt8 sa_type, AiUInt8 con,
                                                           AiUInt8 rmod, AiUInt8 smod, AiUInt16 swm);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTGetSAConErr     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt8 sa, AiUInt8 sa_type, TY_API_RT_ERR *perr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSAConErr        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt8 sa, AiUInt8 sa_type, TY_API_RT_ERR *perr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTDytagDef        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con,
                                                           AiUInt16 rt_hid, AiUInt16 mode, TY_API_RT_DYTAG rt_dytag[4]);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTGetDytagDef     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 rt_hid, AiUInt16 *mode,
                                                           TY_API_RT_DYTAG *rt_dytag);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTStart           (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTHalt            (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTStatusRead      (AiUInt32 bModule, AiUInt8 biu, TY_API_RT_STATUS_DSP *pdsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTMsgRead         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           TY_API_RT_MSG_DSP *pmsg_dsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTMsgReadAll      (AiUInt32 bModule, AiUInt8 biu, TY_API_RT_MSG_ALL_DSP *pall_dsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSAMsgRead       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt8 sa, AiUInt8 sa_type, AiUInt8 clr,
                                                           TY_API_RT_SA_MSG_DSP *psa_dsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSAMsgReadEx     (AiUInt32 bModule, TY_API_RT_SA_MSG_READ_IN *px_MsgReadIn,
                                                           TY_API_RT_SA_STATUS_EX *px_RtSaStatus);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTGlobalCon       (AiUInt32 bModule, AiUInt8 biu, AiUInt16 count, TY_API_RT_SA *lpBuf);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTGetSimulationInfo(AiUInt32 Module, AiUInt8 biu, AiUInt8 rt, TY_RT_INFO* px_RT_Info);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTGetSABufferHeaderInfo(AiUInt32 Module, AiUInt8 Biu, AiUInt8 RT, AiUInt8 Type, AiUInt8 SA_MC, AiUInt32* BufHeaderIndex, AiUInt32 *BufHeaderAddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTBHRead          (AiUInt32 Module, AiUInt8 Biu,AiUInt16 uw_HeaderId,TY_API_RT_BH_INFO *pbh);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTModeCtrl        (AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_RT_MODE_CTRL *px_RtModeCtrl);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSADWCSet        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, AiUInt32 ul_WordCnt);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdRTSADWCGet        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, AiUInt32 *pul_WordCnt);




/**********************/
/* internal functions */
/**********************/

AiReturn ApiCmdRTIni_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 con, AiUInt8 bus, AiFloat resp_time, AiUInt16 nxw);
AiReturn ApiCmdRTEnaDis_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 con);
AiReturn ApiCmdRTRespTime_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiFloat resp_time);
AiReturn ApiCmdRTRespTimeGet_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiFloat *pResp_time);
AiReturn ApiCmdRTNXW_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 nxw);
AiReturn ApiCmdRTLCW_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 lcw);
AiReturn ApiCmdRTLSW_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt16 lsw);
AiReturn ApiCmdRTBHDef_(AiUInt32 bModule, AiUInt8 biu, AiUInt16 hid, AiUInt16 bid, AiUInt16 sid, AiUInt16 eid, AiUInt8 qsize, AiUInt8 bqm, AiUInt8 bsm, AiUInt8 sqm, AiUInt8 eqm, AiUInt8 res, TY_API_RT_BH_INFO *pbh);
AiReturn ApiCmdRTSACon_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt16 hid, AiUInt8 sa_type, AiUInt8 con, AiUInt8 rmod, AiUInt8 smod, AiUInt16 swm);
AiReturn ApiCmdRTGetSAConErr_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, TY_API_RT_ERR *perr);
AiReturn ApiCmdRTSAConErr_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, TY_API_RT_ERR *perr);
AiReturn ApiCmdRTDytagDef_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt16 rt_hid, AiUInt16 mode, TY_API_RT_DYTAG rt_dytag[4]);
AiReturn ApiCmdRTGetDytagDef_(AiUInt32 bModule, AiUInt8 biu, AiUInt16 rt_hid, AiUInt16 *mode, TY_API_RT_DYTAG *rt_dytag);
AiReturn ApiCmdRTStart_(AiUInt32 bModule, AiUInt8 biu);
AiReturn ApiCmdRTHalt_(AiUInt32 bModule, AiUInt8 biu);
AiReturn ApiCmdRTStatusRead_(AiUInt32 bModule, AiUInt8 biu, TY_API_RT_STATUS_DSP *pdsp);
AiReturn ApiCmdRTMsgRead_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, TY_API_RT_MSG_DSP *pmsg_dsp);
AiReturn ApiCmdRTMsgReadAll_(AiUInt32 bModule, AiUInt8 biu, TY_API_RT_MSG_ALL_DSP *pall_dsp);
AiReturn ApiCmdRTSAMsgRead_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, AiUInt8 clr, TY_API_RT_SA_MSG_DSP *psa_dsp);
AiReturn ApiCmdRTSAMsgReadEx_(AiUInt32 bModule, TY_API_RT_SA_MSG_READ_IN *px_MsgReadIn, TY_API_RT_SA_STATUS_EX *px_RtSaStatus);
AiReturn ApiCmdRTGlobalCon_(AiUInt32 bModule, AiUInt8 biu, AiUInt16 count, TY_API_RT_SA *lpBuf);
AiReturn ApiCmdRTGetSimulationInfo_(AiUInt32 Module, AiUInt8 biu, AiUInt8 rt, TY_RT_INFO* px_RT_Info);
AiReturn ApiCmdRTGetSABufferHeaderInfo_(AiUInt32 Module, AiUInt8 Biu, AiUInt8 RT, AiUInt8 Type, AiUInt8 SA_MC, AiUInt32* BufHeaderIndex, AiUInt32 *BufHeaderAddr);
AiReturn ApiCmdRTBHRead_(AiUInt32 Module, AiUInt8 Biu, AiUInt16 uw_HeaderId, TY_API_RT_BH_INFO *pbh);
AiReturn ApiCmdRTModeCtrl_(AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_RT_MODE_CTRL *px_RtModeCtrl);
AiReturn ApiCmdRTSADWCSet_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, AiUInt32 ul_WordCnt);
AiReturn ApiCmdRTSADWCGet_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt8 sa, AiUInt8 sa_type, AiUInt32 *pul_WordCnt);


#endif /*AI1553_LS_BM_H*/

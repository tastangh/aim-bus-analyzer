/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsBm.h
 *
 *  This header file contains declarations for
 *  BM (bus monitor) related API functionality
 *
 */

#ifndef AI1553_LS_BM_H
#define AI1553_LS_BM_H 



/************/
/* defines  */
/************/

/* function ApiCmdBMIntrMode() */
#define API_BM_MODE_NO_INT            0
#define API_BM_MODE_HFI_INT           1
#define API_BM_MODE_START_EVENT_INT   2
#define API_BM_MODE_STOP_EVENT_INT    3

#define API_BM_NO_STROBE              0
#define API_BM_STROBE_ON_HFI          1
#define API_BM_STROBE_ON_START_EVENT  2
#define API_BM_STROBE_ON_STOP_EVENT   3

/* function ApiCmdBMCapMode() */
#define API_BM_CAPMODE_ALL            0
#define API_BM_CAPMODE_ONLY           1
#define API_BM_CAPMODE_RECORDING      2
#define API_BM_CAPMODE_FILTER         3

/* function ApiCmdBMStatusRead() */
#define API_BM_MENSW_TRIG_MASK        0x08
#define API_BM_MENSW_STOP_MASK        0x04
#define API_BM_MENSW_START_MASK       0x02
#define API_BM_MENSW_MEN_MASK         0x01


#define API_BM_MSW_SYNC_MASK          0x40

#define API_BM_MSW_EVENT_MASK         0x20
#define API_BM_MSW_TRIG_MASK          0x10
#define API_BM_MSW_STOP_MASK          0x02
#define API_BM_MSW_START_MASK         0x01

/* function ApiCmdBMActRead()*/
#define API_BM_ENTRY_NOT_FOUND        0
#define API_BM_ENTRY_FOUND            1

#define API_BM_ET_ERR_MASK            0x00008000
#define API_BM_ET_ALTER_MASK          0x00004000
#define API_BM_ET_LCNT_MASK           0x00002000
#define API_BM_ET_HCNT_MASK           0x00001000
#define API_BM_ET_STAT_MASK           0x00000800
#define API_BM_ET_TADDR_MASK          0x00000400
#define API_BM_ET_GAP_MASK            0x00000200
#define API_BM_ET_ILLEGL_MASK         0x00000100
#define API_BM_ET_TX_MASK             0x00000080
#define API_BM_ET_IWGAP_MASK          0x00000040
#define API_BM_ET_ISYNC_MASK          0x00000020
#define API_BM_ET_PAR_MASK            0x00000010
#define API_BM_ET_LBIT_MASK           0x00000008
#define API_BM_ET_HBIT_MASK           0x00000004
#define API_BM_ET_MANCH_MASK          0x00000002
#define API_BM_ET_NRESP_MASK          0x00000001

/* function ApiCmdBMStackpRead() */
#define API_BM_TRG_NOT_OCCURED        0
#define API_BM_START_EVENT_OCCURED    1
#define API_BM_STOP_EVENT_OCCURED     2

/* function ApiCmdBMStackEntryRead() */
#define API_BM_READ_STP               0
#define API_BM_READ_CTP               1
#define API_BM_READ_ETP               2
#define API_BM_READ_ABS               3

#define API_BM_SIGN_POS               0
#define API_BM_SIGN_NEG               1

#define API_BM_MBUF_ENTRY_TYPE_MASK   0xF0000000
#define API_BM_MBUF_ENTRY_EC_MASK     0x08000000
#define API_BM_ENTRY_NOT_UPDATED      0
#define API_BM_ENTRY_ERROR_WORD       1
#define API_BM_ENTRY_TIMETAG_LOW      2
#define API_BM_ENTRY_TIMETAG_HIGH     3
#define API_BM_ENTRY_CW_PRIMARY       8
#define API_BM_ENTRY_CW2_PRIMARY      9
#define API_BM_ENTRY_DW_PRIMARY       10
#define API_BM_ENTRY_SW_PRIMARY       11
#define API_BM_ENTRY_CW_SECONDARY     12
#define API_BM_ENTRY_CW2_SECONDARY    13
#define API_BM_ENTRY_DW_SECONDARY     14
#define API_BM_ENTRY_SW_SECONDARY     15

#define API_BM_TIMETAG_SEC_MASK       0x03F00000
#define API_BM_TIMETAG_MSEC_MASK      0x000FFFFF
#define API_BM_TIMETAG_DAYS_MASK      0x000FF800
#define API_BM_TIMETAG_HOURS_MASK     0x000007C0
#define API_BM_TIMETAG_MIN_MASK       0x0000003F

#define API_BM_ENTRY_GAP_MASK         0x01FF0000
#define API_BM_ENTRY_BUSWORD_MASK     0x0000FFFF

/* function ApiCmdBMStackEntryFind() */
#define API_BM_SEARCH_CMD_MASK        0x2000
#define API_BM_SEARCH_DATA_MASK       0x1000
#define API_BM_SEARCH_STAT_MASK       0x0800
#define API_BM_SEARCH_CMD2_MASK       0x0400
#define API_BM_SEARCH_WORDB_MASK      0x0200
#define API_BM_SEARCH_WORDA_MASK      0x0100
#define API_BM_SEARCH_ENTRY_TYPE_MASK 0x0003

#define API_BM_ENTRY_BUS_WORD         0

/* function ApiCmdBMRecStatusRead() */
#define API_BM_HALTED                 1
#define API_BM_BUSY                   2

/* function ApiCmdBMFTWIni() */
#define API_BM_WRITE_ALL              0
#define API_BM_WRITE_STC              1
#define API_BM_WRITE_STM              2
#define API_BM_WRITE_HTC              3
#define API_BM_WRITE_HTM              4

/* function ApiCmdBMTIWIni() */
#define API_BM_WRITE_TI0              1
#define API_BM_WRITE_TI1              2
#define API_BM_WRITE_AEI              3
#define API_BM_WRITE_ETI              4

/* function ApiCmdBMTCIIni() */
#define API_BM_TTMODE_LOW_HIGH        0

/* function ApiCmdBMTCBIni() */
#define API_BM_TRG_ERR_CONDITION      0
#define API_BM_TRG_EXTERNAL_EVENT     1
#define API_BM_TRG_RECEIVED_WORD      2
#define API_BM_TRG_DATA_VALUE         3
#define API_BM_TRG_HS_EVENT           4

#define API_BM_PULSE_STROBE_ON_TRG    1

#define API_BM_ASSERT_TRG_INT         1

#define API_BM_INVERT_RESULT          1

#define API_BM_TRG_SPEC_RXA_MASK      0x20
#define API_BM_TRG_SPEC_RXB_MASK      0x10
#define API_BM_TRG_SPEC_CW2_MASK      0x08
#define API_BM_TRG_SPEC_ST_MASK       0x04
#define API_BM_TRG_SPEC_DW_MASK       0x02
#define API_BM_TRG_SPEC_CW_MASK       0x01
#define API_BM_TRG_SPEC_WPOS_MASK     0x3F

/* function ApiCmdBMReadMsgFltRec() */
#define API_BM_FLT_MODE_INDEPENDENT   0
#define API_BM_FLT_MODE_DEPENDENT     1

#define API_BM_MSG_FLT_FORMAT_0       0
#define API_BM_MSG_FLT_FORMAT_1       1
#define API_BM_MSG_FLT_FORMAT_2       2
#define API_BM_MSG_FLT_FORMAT_3       3
#define API_BM_MSG_FLT_FORMAT_4       4
#define API_BM_MSG_FLT_FORMAT_5       5

#define API_BM_NO_OVERFLOW            0
#define API_BM_OVERFLOW               1

#define API_BM_MSG_NO_ERR             0
#define API_BM_MSG_ERR_NO_RESP        1
#define API_BM_MSG_ERR_MANCH          2
#define API_BM_MSG_ERR_HBIT           3
#define API_BM_MSG_ERR_LBIT           4
#define API_BM_MSG_ERR_PARITY         5
#define API_BM_MSG_ERR_ISYNC          6
#define API_BM_MSG_ERR_GAP            7
#define API_BM_MSG_ERR_BOTH_CHN       8
#define API_BM_MSG_ERR_MODECODE       9
#define API_BM_MSG_ERR_EARLY_RESP     10
#define API_BM_MSG_ERR_BIT_SET        11
#define API_BM_MSG_ERR_SW_EXCEPT      12
#define API_BM_MSG_ERR_HCNT           13
#define API_BM_MSG_ERR_LCNT           14
#define API_BM_MSG_ERR_ILLEGL         15

#define API_BM_MSG_GERR_MASK          0x8000
#define API_BM_MSG_PERR_MASK          0x4000
#define API_BM_MSG_TBUSY_MASK         0x0010




/**************************/
/* structs and typedefs   */
/**************************/

typedef struct ty_api_bm_cap_setup
{
    AiUInt8  cap_mode;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 cap_tat;
    AiUInt16 cap_mcc;
    AiUInt16 cap_fsize;
} TY_API_BM_CAP_SETUP;

typedef struct ty_api_bm_status_dsp
{
    AiUInt8  men;
    AiUInt8  msw;
    AiUInt8  msp;
    AiUInt8  padding1;
    AiUInt32 glb_msg_cnt;
    AiUInt32 glb_err_cnt;
    AiUInt32 glb_word_cnt_sec;
    AiUInt32 glb_word_cnt_pri;

    AiUInt32 glb_msg_cnt_sec;
    AiUInt32 glb_msg_cnt_pri;
    AiUInt32 glb_err_cnt_sec;
    AiUInt32 glb_err_cnt_pri;

    AiUInt32 bus_load_sec;     /* last 500ms */
    AiUInt32 bus_load_pri;     /* last 500ms */
    AiUInt32 bus_load_sec_avg; /* since BM start */
    AiUInt32 bus_load_pri_avg; /* since BM start */
} TY_API_BM_STATUS_DSP;

typedef struct ty_api_bm_act_dsp
{
    AiUInt8  fnd;
    AiUInt8  rt;
    AiUInt8  tr;
    AiUInt8  sa;
    AiUInt32 cc;
    AiUInt32 ec;
    AiUInt32 et;
} TY_API_BM_ACT_DSP;

typedef struct ty_api_bm_stackp_dsp
{
    AiUInt8  status;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 stp;
    AiUInt32 ctp;
    AiUInt32 etp;
} TY_API_BM_STACKP_DSP;

typedef struct ty_api_bm_stack_dsp
{
    AiUInt32 entry;
} TY_API_BM_STACK_DSP;

typedef struct ty_api_bm_stack_fnd
{
    AiUInt8  efnd;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 eptr;
    AiUInt32 entry;
} TY_API_BM_STACK_FND;

typedef struct ty_api_bm_rec
{
    AiUInt8  status;
    AiUInt8  padding1;
    AiUInt16 padding2;
    AiUInt32 hfi_cnt;
    AiUInt32 saddr;
    AiUInt32 size;
} TY_API_BM_REC;

typedef struct ty_api_bm_tcb
{
    AiUInt8  tt;
    AiUInt8  sot;
    AiUInt8  tri;
    AiUInt8  inv;
    AiUInt8  tres;
    AiUInt8  tset;
    AiUInt8  tsp;
    AiUInt8  next;
    AiUInt8  eom;
    AiUInt8  padding1;
    AiUInt16 tdw;
    AiUInt16 tmw;
    AiUInt16 tuli;
    AiUInt16 tlli;
} TY_API_BM_TCB;

typedef struct ty_api_bm_rt_act
{
    AiUInt32 mc;
    AiUInt32 ec;
    AiUInt32 et;
} TY_API_BM_RT_ACT;

typedef struct ty_api_bm_ini_msg_flt_rec
{
    AiUInt16 cw;
    AiUInt8 pos1;
    AiUInt8 pos2;
} TY_API_BM_INI_MSG_FLT_REC;

typedef struct ty_api_bm_dytag_mon_def
{
    AiUInt8 uc_Id;
    AiUInt8 uc_Con;
    AiUInt8 uc_RtAddr;
    AiUInt8 uc_SubAddrMsgId;
    AiUInt8 uc_SubAddrMsgIdType;
    AiUInt8 uc_Padding1;
    AiUInt16 uw_DytagType;
    AiUInt16 uw_DytagWPos;
    AiUInt16 uw_DytagBPos;
    AiUInt16 uw_DytagBLen;
    AiUInt16 uw_DytagStep;
} TY_API_BM_DYTAG_MON_DEF;

typedef struct ty_api_bm_dytag_mon_read_ctrl
{
    AiUInt8 uc_Id;
} TY_API_BM_DYTAG_MON_READ_CTRL;

typedef struct ty_api_bm_dytag_mon_act
{
    AiUInt32 ul_Stale;
    AiUInt32 ul_Bad;
    AiUInt32 ul_Good;
} TY_API_BM_DYTAG_MON_ACT;




/**************************/
/* BC interface functions */
/**************************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMIni             (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMIntrMode        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 imod, AiUInt8 smod, AiUInt8 res);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMCapMode         (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_CAP_SETUP *pcap);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMSWXMIni         (AiUInt32 bModule, AiUInt8 biu, AiUInt16 swxm);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMStart           (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMHalt            (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMStatusRead      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_STATUS_DSP *pdsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMActRead         (AiUInt32 bModule, AiUInt8 biu, AiUInt16 entry_no, TY_API_BM_ACT_DSP *pact);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMStackpRead      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_STACKP_DSP *pstackp);
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiCmdBMStackEntryRead(AiUInt32 bModule, AiUInt8 biu, AiUInt8 ptype,AiUInt8 sgn, AiUInt32 offset, TY_API_BM_STACK_DSP *pentry));
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiCmdBMStackEntryFind(AiUInt32 bModule, AiUInt8 biu, AiUInt8 ptype,AiUInt8 sgn, AiUInt32 offset,AiUInt16 fspec, AiUInt16 fdata, AiUInt16 fmask,TY_API_BM_STACK_FND *pfnd));
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMRecStatusRead   (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_REC *prec);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMCopyRecDat      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_REC *prec);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMFilterIni       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt32 rx_sa, AiUInt32 tx_sa, AiUInt32 rx_mc, AiUInt32 tx_mc);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMIllegalIni      (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr,
                                                           AiUInt32 rx_sa, AiUInt32 tx_sa, AiUInt32 rx_mc, AiUInt32 tx_mc);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMFTWIni          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 htm, AiUInt8 htc, AiUInt8 stm, AiUInt8 stc); 
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMTIWIni          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 eti, AiUInt8 aei, AiUInt8 ti0, AiUInt8 ti1); 
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMTCIIni          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt, AiUInt8 sa,
                                                           AiUInt8 sa_type, AiUInt8 tagm, AiUInt8 tfct, AiUInt8 tid);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMTCBIni          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 tid, AiUInt8 ten,
                                                           TY_API_BM_TCB *ptcb);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMRTActRead       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt,
                                                           TY_API_BM_RT_ACT *pact);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMRTSAActRead     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt, AiUInt8 sa, AiUInt8 sa_type,
                                                           TY_API_BM_RT_ACT *pact);
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiCmdBMIniMsgFltRec(AiUInt32 bModule, AiUInt8 biu, AiUInt8 cnt,TY_API_BM_INI_MSG_FLT_REC *pmrec));
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiCmdBMReadMsgFltRec(AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode,AiUInt8 con, AiUInt32 max_size, AiUInt16 max_msg,void *lpBuf, AiUInt8 *ovfl, AiUInt32 *lWordsRead));
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMDytagMonDef     (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_DYTAG_MON_DEF *px_DytagMon);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBMDytagMonRead    (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_DYTAG_MON_READ_CTRL *px_DytagMonCtrl,
                                                           TY_API_BM_DYTAG_MON_ACT *px_DytagMonAct);





/**********************/
/* internal functions */
/**********************/
AiInt16 ApiCmdBMIni_             (AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdBMIntrMode_        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 imod, AiUInt8 smod, AiUInt8 res);
AiInt16 ApiCmdBMCapMode_         (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_CAP_SETUP *pcap);
AiInt16 ApiCmdBMSWXMIni_         (AiUInt32 bModule, AiUInt8 biu, AiUInt16 swxm);
AiInt16 ApiCmdBMStart_           (AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdBMHalt_            (AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdBMStatusRead_      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_STATUS_DSP *pdsp);
AiInt16 ApiCmdBMActRead_         (AiUInt32 bModule, AiUInt8 biu, AiUInt16 entry_no, TY_API_BM_ACT_DSP *pact);
AiInt16 ApiCmdBMStackpRead_      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_STACKP_DSP *pstackp);
AiInt16 ApiCmdBMStackEntryRead_  (AiUInt32 bModule, AiUInt8 biu, AiUInt8 ptype, AiUInt8 sgn, AiUInt32 offset, TY_API_BM_STACK_DSP *pentry);
AiInt16 ApiCmdBMStackEntryFind_  (AiUInt32 bModule, AiUInt8 biu, AiUInt8 ptype, AiUInt8 sgn, AiUInt32 offset, AiUInt16 fspec, AiUInt16 fdata, AiUInt16 fmask, TY_API_BM_STACK_FND *pfnd);
AiInt16 ApiCmdBMRecStatusRead_   (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_REC *prec);
AiInt16 ApiCmdBMCopyRecDat_      (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_REC *prec);
AiInt16 ApiCmdBMFilterIni_       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt32 rx_sa, AiUInt32 tx_sa, AiUInt32 rx_mc, AiUInt32 tx_mc);
AiInt16 ApiCmdBMIllegalIni_      (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt_addr, AiUInt32 rx_sa, AiUInt32 tx_sa, AiUInt32 rx_mc, AiUInt32 tx_mc);
AiInt16 ApiCmdBMFTWIni_          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 htm, AiUInt8 htc, AiUInt8 stm, AiUInt8 stc); 
AiInt16 ApiCmdBMTIWIni_          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt8 eti, AiUInt8 aei, AiUInt8 ti0, AiUInt8 ti1); 
AiInt16 ApiCmdBMTCIIni_          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt, AiUInt8 sa, AiUInt8 sa_type, AiUInt8 tagm, AiUInt8 tfct, AiUInt8 tid);
AiInt16 ApiCmdBMTCBIni_          (AiUInt32 bModule, AiUInt8 biu, AiUInt8 tid, AiUInt8 ten, TY_API_BM_TCB *ptcb);
AiInt16 ApiCmdBMRTActRead_       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt, TY_API_BM_RT_ACT *pact);
AiInt16 ApiCmdBMRTSAActRead_     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rt, AiUInt8 sa, AiUInt8 sa_type, TY_API_BM_RT_ACT *pact);
AiInt16 ApiCmdBMIniMsgFltRec_    (AiUInt32 bModule, AiUInt8 biu, AiUInt8 cnt, TY_API_BM_INI_MSG_FLT_REC *pmrec);
AiInt16 ApiCmdBMReadMsgFltRec_   (AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode, AiUInt8 con, AiUInt32 max_size, AiUInt16 max_msg, void *lpBuf, AiUInt8 *ovfl, AiUInt32 *lWordsRead);
AiInt16 ApiCmdBMDytagMonDef_     (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_DYTAG_MON_DEF *px_DytagMon);
AiInt16 ApiCmdBMDytagMonRead_    (AiUInt32 bModule, AiUInt8 biu, TY_API_BM_DYTAG_MON_READ_CTRL *px_DytagMonCtrl, TY_API_BM_DYTAG_MON_ACT *px_DytagMonAct);




#endif /*AI1553_LS_BM_H*/

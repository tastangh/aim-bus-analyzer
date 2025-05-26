/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsBc.h
 *
 *  This header file contains declarations for
 *  BC (bus controller) related API functionality
 *
 */

#ifndef AI1553_LS_BC_H
#define AI1553_LS_BC_H 










/***************/
/* BC defines  */
/***************/

#define MIN_API_BC_XFRAME       1   /* Transfers in Minor Frame    */
#define MAX_API_BC_XFRAME       128 /* Transfers in Minor Frame    */
#define MAX_API_BC_XACYC        127 /* Transfers in Acyclic Frame  */
#define MIN_API_BC_MFRAME       1
#define MAX_API_BC_MFRAME       64 /* Minor Frames in Major Frame  */
#define MAX_API_BC_MFRAME_EX   512 /* Minor Frames in Major Frame for EX function */

#define MIN_API_BC_MFRAME_ID    1
#define MAX_API_BC_MFRAME_ID   64 /* Minor Frames in Major Frame   */


/* function ApiCmdBCXferDef() */
#define API_BC_TYPE_MASK_TRANSFER     0x000000FF

#define API_BC_TYPE_BCRT              0
#define API_BC_TYPE_RTBC              1
#define API_BC_TYPE_RTRT              2
/* alt */
#define API_BC_XFER_BCRT  0
#define API_BC_XFER_RTBC  1
#define API_BC_XFER_RTRT  2
#define API_BC_XFER_BUSA  0
#define API_BC_XFER_BUSB  1

#define API_BC_XFER_BUS_PRIMARY       0
#define API_BC_XFER_BUS_SECONDARY     1

#define API_BC_TIC_NO_INT             0
#define API_BC_TIC_INT_ON_XFER_END    1
#define API_BC_TIC_INT_ON_XFER_ERR    2
#define API_BC_TIC_INT_ON_STAT_EXCEPT 3

#define API_BC_HLT_NO_HALT            0
#define API_BC_HLT_HALT_ON_XFER_ERR   1
#define API_BC_HLT_HALT_ON_STAT_EXCEPT  2
#define API_BC_HLT_HALT_ON_ERR_EXCEPT 3
#define API_BC_HLT_HALT_ON_ANY_INT    4

#define API_BC_SRVW_DIS               0
#define API_BC_SRVW_ENA               1
#define API_BC_SRVW_SINGLE            2
#define API_BC_SRVW_MULTIPLE          3

#define API_TOGGLE_BUS_KEEP           2
#define API_TOGGLE_BUS_ALT            3

#define API_BC_RSP_AUTOMATIC          0
#define API_BC_RSP_NO_SW1_EXPECTED    1
#define API_BC_RSP_NO_SW2_EXPECTED    2

#define API_BC_SWXM_MERR_MASK         0x0400
#define API_BC_SWXM_INSTR_MASK        0x0200
#define API_BC_SWXM_SREQ_MASK         0x0100
#define API_BC_SWXM_BRCV_MASK         0x0010
#define API_BC_SWXM_BUSY_MASK         0x0008
#define API_BC_SWXM_SUB_MASK          0x0004
#define API_BC_SWXM_DBCA_MASK         0x0002
#define API_BC_SWXM_TERM_MASK         0x0001


#define API_BC_GAP_MODE_DELAY         0
#define API_BC_GAP_MODE_STANDARD      1
#define API_BC_GAP_MODE_FAST          2


#define API_BC_ERRSPEC_WPOS_MASK      0x00FF0000
#define API_BC_ERRSPEC_BPOS_MASK      0x0000FF00
#define API_BC_ERRSPEC_BCBITS_MASK    0x000000FF
#define API_BC_ERRSPEC_WPOS_POS       16
#define API_BC_ERRSPEC_BPOS_POS       8
#define API_BC_ERRSPEC_BCBITS_POS     0

/* function ApiCmdBCDytagDef() */
#define API_DYTAG_STD_MODE              0
#define API_DYTAG_EFA_MODE              1

#define API_DYTAG_FCT_DISABLE           0
#define API_DYTAG_FCT_POS_RAMP          1
#define API_DYTAG_FCT_NEG_RAMP          2
#define API_DYTAG_FCT_POS_TRIANGLE      3
#define API_DYTAG_FCT_NEG_TRIANGLE      4
#define API_DYTAG_FCT_XMT_WORD          5
#define API_DYTAG_FCT_SYNC_COUNTER      6

#define API_DYTAG_DIRECT_MODIFY         0
#define API_DYTAG_FUNCTION_MODIFY       1

#define API_DYTAG_FCT_SAWTOOTH_16       1
#define API_DYTAG_FCT_SAWTOOTH_8_LOW    2
#define API_DYTAG_FCT_SAWTOOTH_8_HIGH   3

#define API_DYTAG_FCT_MASK              0x007F

/* function ApiCmdBCFrameDef() */
#define API_BC_INSTR_TRANSFER         1
#define API_BC_INSTR_SKIP             2
#define API_BC_INSTR_WAIT             3
#define API_BC_INSTR_STROBE           4

/* function ApiCmdBCStart() */
#define API_BC_START_IMMEDIATELY      1
#define API_BC_START_EXT_TRIGGER      2
#define API_BC_START_RT_MODECODE      3
#define API_BC_START_EXTERNAL_PULSE   4
#define API_BC_START_SETUP            5
#define API_BC_START_FAST             6
#define API_BC_START_INSTR_TABLE          7
#define API_BC_START_CONTINUE_ON_BC_HALT  8
#define API_BC_START_INSTR_TABLE_DBA      9

#define API_BC_EXEC_CYCLIC            0

#define API_BC_ACYC_SEND_IMMEDIATELY     0
#define API_BC_ACYC_SEND_ON_TIMETAG      1
#define API_BC_ACYC_SEND_AT_END_OF_FRAME 2

/* function ApiCmdBCInstrTblGen() */
/* Transfer Descriptor size */
#define MAX_API_BC_FW_XFER_DESC_SIZE 8
/* BC Firmware Instruction opcodes */
#define API_BC_FWI_XFER  0x10  /* Transfer */
#define API_BC_FWI_EFEX_XFER  0x11  /* EFEX Transfer */
#define API_BC_FWI_CALL  0x20  /* Call Subtable */
#define API_BC_FWI_RET   0x21  /* Return from Subtable */
#define API_BC_FWI_JUMP  0x22  /* Jump to Subtable */
#define API_BC_FWI_SKIP  0x24  /* Relative Branch (Skip) */
#define API_BC_FWI_WTRG  0x25  /* Wait for BC Trigger input */
#define API_BC_FWI_STRB  0x26  /* Strobe BC Trigger output */
#define API_BC_FWI_DJZ   0x27  /* Decrement and Jump on Zero */
#define API_BC_FWI_WMFT  0x28  /* Wait for next Minor Frame Time slot */
#define API_BC_FWI_HALT  0x29  /* BC Operation Halt */
#define API_BC_FWI_DELAY 0x2A  /* Delay */
#define API_BC_FWI_MODIFY 0x2B  /* Modify instruction */
#define API_BC_FWI_CMFT  0x2C  /* Change Minor Frame Time */
#define API_BC_FWI_RESMF 0x2D  /* Reset Major Frame */
// #define API_BC_FWI_PRE_ACYC_ON_TTAG 0x2E  /* Reserved internal use only */
#define API_BC_FWI_RET_ON_XFER_ERR 0x2F /* Return on Transfer Error */
/* Instruction macro */
#define API_BC_FW_MAKE_INSTR(b1,b2) ((AiUInt32)(((AiUInt32)(b1) << 26) + ((AiUInt32)(b2) & 0x03FFFFFFL)))

/* function ApiCmdBCStatusRead() */
#define API_BC_STATUS_HALTED          1
#define API_BC_STATUS_BUSY            2

/* function ApiCmdBCXferRead() */
#define API_DONT_MODIFY_STATUS_BITS   0
#define API_RESET_STATUS_BITS         1
#define API_RESET_ERROR_BITS          2
#define API_RESET_ERR_STAT_BITS       3
#define API_RESET_SRVREQ_CNT          4


#define API_BC_REPORT_NO_ERR          0
#define API_BC_REPORT_ERR_FRAME_COD   1
#define API_BC_REPORT_ERR_SW1_NOT_RCV 2
#define API_BC_REPORT_ERR_SW2_NOT_RCV 3
#define API_BC_REPORT_ERR_SW_EXCEPTION  4

/* function ApiCmdBCSrvReqVecCon() */
#define API_SEND_SRVW_ON_SA0          0
#define API_SEND_SRVW_ON_SA31         1

/* function ApiCmdBCModeCtrl() */
#define API_BC_MODE_INSERT_MC17_SYNC_CNT    1
#define API_BC_MODE_STOP_ON_MC0_DBCA_FLAG   2
#define API_BC_MODE_CONFIGURED_DYTAGS       3



/**************************/
/* structs and typedefs   */
/**************************/
typedef struct ty_api_bc_bh_info
{
    AiUInt16 bid;
    AiUInt16 sid;
    AiUInt16 eid;
    AiUInt16 nbufs;
    AiUInt32 hid_addr;
    AiUInt32 bq_addr;
    AiUInt32 sq_addr;
    AiUInt32 eq_addr;
} TY_API_BC_BH_INFO;

typedef struct ty_api_bc_err
{
    AiUInt8  type;
    AiUInt8  sync;
    AiUInt8  contig;
    AiUInt8  padding1;
    AiUInt32 err_spec;
} TY_API_BC_ERR;

typedef struct ty_api_bc_xfer
{
    AiUInt16 xid;
    AiUInt16 hid;
    AiUInt8 type;
    AiUInt8 chn;
    AiUInt8 xmt_rt;
    AiUInt8 rcv_rt;
    AiUInt8 xmt_sa;
    AiUInt8 rcv_sa;
    AiUInt8 wcnt;
    AiUInt8 tic;
    AiUInt8 hlt;
    AiUInt8 rte;
    AiUInt8 res;
    AiUInt8 sxh;
    AiUInt8 rsp;
    AiUInt8 gap_mode;
    AiUInt16 swxm;
    struct ty_api_bc_err err;
    AiUInt16 gap;
} TY_API_BC_XFER;

typedef struct ty_api_bc_dytag
{
    AiUInt16 tag_fct;
    AiUInt16 min;
    AiUInt16 max;
    AiUInt16 step;
    AiUInt16 wpos;
} TY_API_BC_DYTAG;

typedef struct ty_api_bc_frame
{
    AiUInt8 id;
    AiUInt8 cnt;
    AiUInt16 instr[MAX_API_BC_XFRAME];
    AiUInt16 xid[MAX_API_BC_XFRAME];
} TY_API_BC_FRAME;

typedef struct ty_api_bc_mframe
{
    AiUInt8 cnt;
    AiUInt8 fid[MAX_API_BC_MFRAME];
} TY_API_BC_MFRAME;

typedef struct ty_api_bc_mframe_ex
{
    AiUInt16 cnt;
    AiUInt16 padding1;
    AiUInt8 fid[MAX_API_BC_MFRAME_EX];
} TY_API_BC_MFRAME_EX;

typedef struct ty_api_bc_acyc
{
    AiUInt8 cnt;
    AiUInt8 padding1;
    AiUInt16 instr[MAX_API_BC_XACYC];
    AiUInt16 xid[MAX_API_BC_XACYC];
} TY_API_BC_ACYC;

typedef struct ty_api_bc_status_dsp
{
    AiUInt8  status;
    AiUInt8  padding1;
    AiUInt16 hxfer;
    AiUInt32 glb_msg_cnt;
    AiUInt32 glb_err_cnt;
    AiUInt32 hip;
    AiUInt32 mfc;
} TY_API_BC_STATUS_DSP;

typedef struct ty_api_bc_xfer_dsp
{
    AiUInt16 cw1;
    AiUInt16 st1;
    AiUInt16 cw2;
    AiUInt16 st2;
    AiUInt16 bid;
    AiUInt16 brw;
    AiUInt32 bufp;
    AiUInt32 ttag;
    AiUInt32 msg_cnt;
    AiUInt32 err_cnt;
    AiUInt32 lvw;
    AiUInt32 srvreq_cnt;
} TY_API_BC_XFER_DSP;

typedef struct ty_api_bc_xfer_status_queue
{
    AiUInt32 ul_SqCtrlWord;
    AiUInt32 ul_SqStatusWords;
    AiUInt32 ul_SqActBufPtr;
    AiUInt32 ul_SqTimeTag;
} TY_API_BC_XFER_STATUS_QUEUE;

typedef struct ty_api_bc_xfer_event_queue
{
    AiUInt32 ul_EqEntryWord1;
    AiUInt32 ul_EqEntryWord2;
    AiUInt32 ul_EqEntryWord3;
    AiUInt32 ul_EqEntryWord4;
} TY_API_BC_XFER_EVENT_QUEUE;

typedef struct ty_api_bc_xfer_status_info
{
    AiUInt32 ul_ActBufferId;
    AiUInt32 ul_ActionWord;
    AiUInt32 ul_XferCnt;
    AiUInt32 ul_ErrCnt;
    AiUInt32 ul_LastVectorWord;
    AiUInt32 ul_ServiceRequestCnt;
} TY_API_BC_XFER_STATUS_INFO;

typedef struct ty_api_bc_xfer_status_ex
{
    TY_API_BC_XFER_STATUS_QUEUE      x_XferSQueue[256];
    TY_API_BC_XFER_EVENT_QUEUE       x_XferEQueue[1];
    TY_API_BC_XFER_STATUS_INFO       x_XferInfo;
    AiUInt32                         ul_BufferQueueSize;
} TY_API_BC_XFER_STATUS_EX;

typedef struct ty_api_bc_xfer_read_in
{
    AiUInt32 ul_XferId;
    AiUInt32 ul_Clear;
    AiUInt32 ul_Biu;
} TY_API_BC_XFER_READ_IN;

typedef struct ty_api_bc_srvw
{
    AiUInt16 uw_XferId;
    AiUInt16 uw_LastVecWord;
    AiUInt32 ul_SrvReqCnt;
} TY_API_BC_SRVW;

typedef struct ty_api_bc_srvw_con
{
    AiUInt8  uc_SubAddress;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_Reserved1;
    AiUInt32 ul_Reserved2;
    AiUInt32 ul_Reserved3;
} TY_API_BC_SRVW_CON;

typedef struct ty_api_bc_fw_instr
{
    AiUInt16 label;
    AiUInt8  op;
    AiUInt8  res;
    AiUInt32 par1;  
    AiUInt32 par2;  
    AiUInt32 laddr;
} TY_API_BC_FW_INSTR;

typedef struct ty_api_bc_fw_rsrc_desc
{
    AiUInt32 max_cnt;  
    AiUInt32 start_addr;  
} TY_API_BC_FW_RSRC_DESC;

typedef struct ty_api_bc_fw_rsrc
{
    TY_API_BC_FW_RSRC_DESC xfer;  
    TY_API_BC_FW_RSRC_DESC modif;  
    TY_API_BC_FW_RSRC_DESC instr;  
} TY_API_BC_FW_RSRC;

typedef struct ty_api_bc_mode_ctrl
{
    AiUInt32 ul_BcMode;
    AiUInt32 ul_Ctrl;
    AiUInt32 ul_Param1;
    AiUInt32 ul_Param2;
    AiUInt32 ul_Param3;
} TY_API_BC_MODE_CTRL;




/**************************/
/* BC interface functions */
/**************************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCIni             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 retr, AiUInt8 svrq, AiUInt8 tbm, AiUInt8 gsb);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCBHDef           (AiUInt32 bModule, AiUInt8 biu, AiUInt16 hid, AiUInt16 bid, AiUInt16 sid, AiUInt16 eid,
                                                           AiUInt8 qsize, AiUInt8 bqm, AiUInt8 bsm, AiUInt8 sqm, AiUInt8 eqm, AiUInt8 dbm, TY_API_BC_BH_INFO *pbh);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferDef         (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_XFER *pxfer, AiUInt32 *desc_addr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferDefErr      (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, TY_API_BC_ERR * pErrorInjection );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferCtrl        (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt8 mode);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferDescGet     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt32 ulMode, AiUInt32 aulValue[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferDescMod     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt32 ulMode, AiUInt32 aulValue[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCDytagDef        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con,
                                                           AiUInt16 bc_hid, AiUInt16 mode, TY_API_BC_DYTAG bc_dytag[4]);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCGetDytagDef     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 bc_hid, AiUInt16 *mode, TY_API_BC_DYTAG *bc_dytag);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCFrameDef        (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_FRAME  *pframe);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCMFrameDef       (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_MFRAME  *pmframe);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCMFrameDefEx     (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_MFRAME_EX  *pmframe);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCStart           (AiUInt32 bModule, AiUInt8 biu, AiUInt8 smod, AiUInt32 cnt, AiFloat frame_time, AiUInt32 saddr, 
                                                           AiUInt32 *maj_frame_addr, AiUInt32 *min_frame_addr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCHalt            (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCStatusRead      (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_STATUS_DSP *pdsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferRead        (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt16 clr, TY_API_BC_XFER_DSP *pxfer_dsp);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCXferReadEx      (AiUInt32 bModule, TY_API_BC_XFER_READ_IN *px_XferReadIn, TY_API_BC_XFER_STATUS_EX *px_XferStat);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCAcycPrep        (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_ACYC *pacyc);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCAcycSend(AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode, AiUInt32 timetag_high, AiUInt32 timetag_low);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCAcycPrepAndSendTransferBlocking(AiUInt32 bModule, AiUInt8 biu, TY_API_BC_XFER * xfer, AiUInt16 data[32], TY_API_BC_XFER_DSP * transfer_status);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCSrvReqVecStatus(AiUInt32 bModule, AiUInt8 uc_Biu, AiUInt8 uc_RtAddr, TY_API_BC_SRVW *px_SrvReqVecStatus);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCSrvReqVecCon    (AiUInt32 bModule, AiUInt8 uc_Biu, AiUInt8 uc_RtAddr, TY_API_BC_SRVW_CON *px_SrvReqVecCon);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCGetXferBufferHeaderInfo (AiUInt32 Module, AiUInt8 Biu,AiUInt32 XferId, AiUInt32 *BufHeaderIndex, AiUInt32 *BufHeaderAddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCBHRead          (AiUInt32 bModule, AiUInt8 biu,AiUInt16 uw_HeaderId, TY_API_BC_BH_INFO *pbh);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCGetMajorFrameDefinition (AiUInt32 Module, AiUInt8 Biu, TY_API_BC_MFRAME* px_BCMajorFrame);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCGetMinorFrameDefinition (AiUInt32 Module, AiUInt8 Biu, AiUInt8 frame_id, TY_API_BC_FRAME* px_BCMinorFrame);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCGetXferDef      (AiUInt32 Module, AiUInt8 Biu,AiUInt16 uw_XferId, TY_API_BC_XFER* px_BCXfer);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCInstrTblIni     (AiUInt32 bModule, AiUInt8 biu);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCInstrTblGetAddrFromLabel(AiUInt32 bModule, AiUInt8 biu, AiUInt32 label, AiUInt32 cnt, TY_API_BC_FW_INSTR *tbl, 
                                                           AiUInt32 *raddr, AiUInt32 *line);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCInstrTblGen     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode, AiUInt32 cnt, AiUInt32 dest_cnt, AiUInt32 dest_offset, 
                                                           TY_API_BC_FW_INSTR *tbl, AiUInt32 *ctbl, AiUInt32 *err_line, AiUInt8 *status);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCModeCtrl        (AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_BC_MODE_CTRL *px_BcModeCtrl);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCInstrModifyDefine(AiUInt32 bModule, AiUInt8 biu, AiUInt32 id, TY_API_BC_MODIFY_INSTRUCTION * modify, AiUInt32 * offset);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCTrafficOverrideControl(AiUInt32 bModule, AiUInt8 biu, AiUInt32 con);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBCTrafficOverrideGet(AiUInt32 bModule, AiUInt8 biu, AiUInt32 * con);



/**********************/
/* internal functions */
/**********************/

AiInt16 ApiCmdBCIni_             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 retr, AiUInt8 svrq, AiUInt8 tbm, AiUInt8 gsb);
AiInt16 ApiCmdBCBHDef_           (AiUInt32 bModule, AiUInt8 biu, AiUInt16 hid, AiUInt16 bid, AiUInt16 sid, AiUInt16 eid, AiUInt8 qsize, AiUInt8 bqm, AiUInt8 bsm, AiUInt8 sqm, AiUInt8 eqm, AiUInt8 dbm, TY_API_BC_BH_INFO *pbh);
AiInt16 ApiCmdBCXferDef_         (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_XFER *pxfer, AiUInt32 *desc_addr);
AiInt16 ApiCmdBCXferDefErr_      (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, TY_API_BC_ERR * pErrorInjection );
AiInt16 ApiCmdBCXferCtrl_        (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt8 mode);
AiInt16 ApiCmdBCDytagDef_        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 con, AiUInt16 bc_hid, AiUInt16 mode, TY_API_BC_DYTAG bc_dytag[4]);
AiInt16 ApiCmdBCGetDytagDef_     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 bc_hid, AiUInt16 *mode, TY_API_BC_DYTAG *bc_dytag);
AiInt16 ApiCmdBCFrameDef_        (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_FRAME  *pframe);
AiInt16 ApiCmdBCMFrameDef_       (AiUInt32 bModule, AiUInt8 biu, AiUInt32 frame_cnt, AiUInt8 *frame_ids);
AiInt16 ApiCmdBCStart_           (AiUInt32 bModule, AiUInt8 biu, AiUInt8 smod, AiUInt32 cnt, AiFloat frame_time, AiUInt32 saddr, AiUInt32 *maj_frame_addr, AiUInt32 *min_frame_addr);
AiInt16 ApiCmdBCHalt_            (AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdBCStatusRead_      (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_STATUS_DSP *pdsp);
AiInt16 ApiCmdBCXferRead_        (AiUInt32 bModule, AiUInt8 biu, AiUInt16 xid, AiUInt16 clr, TY_API_BC_XFER_DSP *pxfer_dsp);
AiInt16 ApiCmdBCXferReadEx_      (AiUInt32 bModule, TY_API_BC_XFER_READ_IN *px_XferReadIn, TY_API_BC_XFER_STATUS_EX *px_XferStat);
AiInt16 ApiCmdBCAcycPrep_        (AiUInt32 bModule, AiUInt8 biu, TY_API_BC_ACYC *pacyc);
AiInt16 ApiCmdBCAcycSend_        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode, AiUInt32 timetag_high, AiUInt32 timetag_low );
AiInt16 ApiCmdBCSrvReqVecStatus_ (AiUInt32 bModule, AiUInt8 uc_Biu, AiUInt8 uc_RtAddr, TY_API_BC_SRVW *px_SrvReqVecStatus);
AiInt16 ApiCmdBCSrvReqVecCon_    (AiUInt32 bModule, AiUInt8 uc_Biu, AiUInt8 uc_RtAddr, TY_API_BC_SRVW_CON *px_SrvReqVecCon);
AiInt16 ApiCmdBCGetXferBufferHeaderInfo_ (AiUInt32 Module, AiUInt8 Biu,AiUInt32 XferId, AiUInt32 *BufHeaderIndex, AiUInt32 *BufHeaderAddr);
AiInt16 ApiCmdBCBHRead_          (AiUInt32 bModule, AiUInt8 biu,AiUInt16 uw_HeaderId, TY_API_BC_BH_INFO *pbh);
AiInt16 ApiCmdBCGetMajorFrameDefinition_ (AiUInt32 Module, AiUInt8 Biu, TY_API_BC_MFRAME* px_BCMajorFrame);
AiInt16 ApiCmdBCGetMinorFrameDefinition_ (AiUInt32 Module, AiUInt8 Biu, AiUInt8 frame_id, TY_API_BC_FRAME* px_BCMinorFrame);
AiInt16 ApiCmdBCGetXferDef_      (AiUInt32 Module, AiUInt8 Biu,AiUInt16 uw_XferId, TY_API_BC_XFER* px_BCXfer);
AiInt16 ApiCmdBCInstrTblIni_     (AiUInt32 bModule, AiUInt8 biu);
AiInt16 ApiCmdBCInstrTblGetAddrFromLabel_(AiUInt32 bModule, AiUInt8 biu, AiUInt32 label, AiUInt32 cnt, TY_API_BC_FW_INSTR *tbl, AiUInt32 *raddr, AiUInt32 *line);
AiInt16 ApiCmdBCInstrTblGen_     (AiUInt32 bModule, AiUInt8 biu, AiUInt8 mode, AiUInt32 cnt, AiUInt32 dest_cnt, AiUInt32 dest_offset, TY_API_BC_FW_INSTR *tbl, AiUInt32 *ctbl, AiUInt32 *err_line, AiUInt8 *status);
AiInt16 ApiCmdBCModeCtrl_        (AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_BC_MODE_CTRL *px_BcModeCtrl);
AiReturn ApiCmdBCInstrModifyDefine_(AiUInt32 bModule, AiUInt8 biu, AiUInt32 id, TY_API_BC_MODIFY_INSTRUCTION * modify, AiUInt32 * offset);
AiReturn ApiCmdBCTrafficOverrideControl_(AiUInt32 bModule, AiUInt8 biu, AiUInt32 con);
AiReturn ApiCmdBCTrafficOverrideGet_(AiUInt32 bModule, AiUInt8 biu, AiUInt32 * con);


#endif /*AI1553_LS_BC_H*/

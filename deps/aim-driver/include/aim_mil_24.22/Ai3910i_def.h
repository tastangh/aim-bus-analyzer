/* SPDX-FileCopyrightText: 2001-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Ai3910i_def.h
 *
 *  Constants and structures for the 3910 Application Interface.
 *
 */

#ifndef AI3910I_DEF_H
#define AI3910I_DEF_H

#include "Ai1553i_def.h"
#include "Ai1553i_fnc.h"

/******************************************
 ***  GENERAL DEFS
 ******************************************/


#define API_HS_MIN_DATA_WORDS         1

#define API_HS_MAX_DATA_WORDS         4096

#define API_HS_BUF_ENTRY_HEADER_SIZE  64  /* in bytes */

/* function ApiCmdBite() */
  /* first status word */
#define API_BITE_ERR_HS_INTERN_SELFTEST      101
#define API_BITE_ERR_HS_TRANSFER             102
#define API_BITE_ERR_HS_TIMING               103

/* second status word */
#define API_BITE_ERR_HS_INT_ADDR             16
#define API_BITE_ERR_HS_INT_PAT_55           17
#define API_BITE_ERR_HS_INT_PAT_AA           18
#define API_BITE_ERR_HS_INT_PAT_FF           19
#define API_BITE_ERR_HS_INT_PAT_00           20
#define API_BITE_ERR_HS_INT_WALK_0           21
#define API_BITE_ERR_HS_INT_WALK_1           22
#define API_BITE_ERR_HS_INT_WRONG_PBI        23
#define API_BITE_ERR_HS_INT_SIM_TX_CNT       24
#define API_BITE_ERR_HS_INT_SIM_ERR_CNT      25
#define API_BITE_ERR_HS_INT_MON_TX_CNT       26
#define API_BITE_ERR_HS_INT_MON_ERR_CNT      27
#define API_BITE_ERR_HS_INT_SIM_BCRT         28
#define API_BITE_ERR_HS_INT_SIM_RTBC         29
#define API_BITE_ERR_HS_INT_MON_DATA         30
#define API_BITE_ERR_HS_INT_MON_TX_CNT_REP   31
#define API_BITE_ERR_HS_INT_MON_ERR_CNT_REP  32
#define API_BITE_ERR_HS_INT_MON_DATA_REP     33
#define API_BITE_ERR_HS_INT_TT_SLOW          40
#define API_BITE_ERR_HS_INT_TT_FAST          41
#define API_BITE_ERR_HS_INT_TT_TIMEOUT       42

#define API_BITE_ERR_HS_BC_BROAD_ISOL_AA     1
#define API_BITE_ERR_HS_BC_BROAD_ISOL_BA     2
#define API_BITE_ERR_HS_BC_BROAD_ISOL_AB     3
#define API_BITE_ERR_HS_BC_BROAD_ISOL_BB     4
#define API_BITE_ERR_HS_BC_BROAD_ELECT_AA    5
#define API_BITE_ERR_HS_BC_BROAD_ELECT_AA_BM 6
#define API_BITE_ERR_HS_BCRT_ISOL_AA         10
#define API_BITE_ERR_HS_BCRT_ISOL_AB         11
#define API_BITE_ERR_HS_BCRT_ISOL_BA         12
#define API_BITE_ERR_HS_BCRT_ISOL_BB         13
#define API_BITE_ERR_HS_RTBC_ISOL_AA         20
#define API_BITE_ERR_HS_RTBC_ISOL_AB         21
#define API_BITE_ERR_HS_RTBC_ISOL_BA         22
#define API_BITE_ERR_HS_RTBC_ISOL_BB         23
#define API_BITE_ERR_HS_RTRT_ISOL_AA         30

#define API_BITE_ERR_HS_TIMING_INIT          1
#define API_BITE_ERR_HS_TIMING_CORR          2
#define API_BITE_ERR_HS_TIMING_TX_TO         3

/* function Api3910CmdHsBufDef() */
#define API_HS_BUF_ALLOC_SUCCESSFUL          0
#define API_HS_BUF_ALLOC_FAILED              1
#define API_HS_BUF_ID_OUT_OF_RANGE           2

/* function Api3910CmdHsCalRxMode() */
#define API_HS_RX_SEL_FIRST_EDGE            0
#define API_HS_RX_SEL_2_VALID_PRE           1
#define API_HS_RX_SEL_4_VALID_PRE           2
#define API_HS_RX_SEL_8_VALID_PRE           3
#define API_HS_RX_SEL_LS_AW                 5
#define API_HS_RX_SEL_FW_DEFAULT            6
#define API_HS_RX_SEL_EXT_LOGIC             7

/* function Api3910CmdEfCalRxMode() */
#define API_EF_RX_SEL_2_VALID_PRE           4

/* function Api3910CmdHsBCBHDef() */
#define API_HS_WCM_ACTION_WORD              0
#define API_HS_WCM_STATUS_QUEUE             1

/* function Api3910CmdHsBCXferDef() */
#define API_HS_BC_TYPE_BCRT                 8
#define API_HS_BC_TYPE_RTBC                 9
#define API_HS_BC_TYPE_BCBR                 10
#define API_HS_BC_TYPE_RTRT                 11
#define API_HS_BC_TYPE_RTBR                 12

#define API_HS_CHANNEL_A                    0
#define API_HS_CHANNEL_B                    1

#define API_HS_ERR_TYPE_NO_INJECTION        0
#define API_HS_ERR_TYPE_START_DEL_INV       1
#define API_HS_ERR_TYPE_START_DEL_PATTERN   2
#define API_HS_ERR_TYPE_END_DEL_INV         3
#define API_HS_ERR_TYPE_END_DEL_NO_END      4
#define API_HS_ERR_TYPE_MANCHESTER_HIGH     5
#define API_HS_ERR_TYPE_MANCHESTER_LOW      6
#define API_HS_ERR_TYPE_BIT_CNT_HIGH        7
#define API_HS_ERR_TYPE_BIT_CNT_LOW         8
#define API_HS_ERR_TYPE_GAP                 9
#define API_HS_ERR_TYPE_INV_BIT             10
#define API_HS_ERR_TYPE_ALTERNATE_BUS       11
#define API_EF_ERR_TYPE_WORDCOUNT           12
#define API_EF_ERR_TYPE_ILLEGAL_WORDCOUNT   13

#define API_HS_BC_ERRSPEC_WPOS_MASK      0xFFFF0000
#define API_HS_BC_ERRSPEC_BPOS_MASK      0x0000FF00
#define API_HS_BC_ERRSPEC_BCBITS_MASK    0x000000FF
#define API_HS_BC_ERRSPEC_WPOS_POS       16
#define API_HS_BC_ERRSPEC_BPOS_POS       8
#define API_HS_BC_ERRSPEC_BCBITS_POS     0

#define API_HS_RCV_RT_MASK                  0xFF00
#define API_HS_XMT_RT_MASK                  0x00FF
#define API_HS_RCV_RT_POS                   8
#define API_HS_XMT_RT_POS                   0

#define API_HS_RCV_MID_MASK                 0xFF00
#define API_HS_XMT_MID_MASK                 0x00FF
#define API_HS_RCV_MID_POS                  8
#define API_HS_XMT_MID_POS                  0

#define API_HS_GAPMODE_MASK                 0x00FF0000
#define API_HS_GAP_MASK                     0x0000FFFF
#define API_HS_GAPMODE_POS                  16
#define API_HS_GAP_POS                      0

#define API_HS_LSCHN_MASK                   0x0000FF00
#define API_HS_LSIR_MASK                    0x000000FF
#define API_HS_LSCHN_POS                    8
#define API_HS_LSIR_POS                     0

#define API_HS_HSXMT_NOPRE_MASK             0x00FF0000
#define API_HS_HSXMT_STARTDEL_MASK          0x0000FFFF
#define API_HS_HSXMT_NOPRE_POS              16
#define API_HS_HSXMT_STARTDEL_POS           0

#define API_HS_HSCHN_MASK                   0x0000FF00
#define API_HS_HSIR_MASK                    0x000000FF
#define API_HS_HSCHN_POS                    8
#define API_HS_HSIR_POS                     0

/* function Api3910CmdHsBCXferRead() */
#define API_HS_BC_MSG_TAG_BUS_MASK          0x8000
#define API_HS_BC_MSG_TAG_MERR_MASK         0x4000
#define API_HS_BC_MSG_TAG_FCHK_MASK         0x2000
#define API_HS_BC_MSG_TAG_DEL_MASK          0x1000
#define API_HS_BC_MSG_TAG_BCNT_MASK         0x0800
#define API_HS_BC_MSG_TAG_WCNT_MASK         0x0400
#define API_HS_BC_MSG_TAG_ITIME_MASK        0x03FF

#define API_HS_BC_ECW_TYPE_MASK             0xC000
#define API_HS_BC_ECW_BUFSTAT_MASK          0x0300
#define API_HS_BC_ECW_BUFINT_MASK           0x0060
#define API_HS_BC_ECW_ERR_REPORT_MASK       0x000F

#define API_HS_BC_TYPE_STATUS_ENTRY         0

#define API_HS_BC_NO_INDEX_INT              0
#define API_HS_BC_ASSERT_INDEX_INT          1

/* function Api3910CmdHsBCModeCtrl() */
#define API_HS_BC_MODE_CONFIGURED_DYTAGS    3

/* function Api3910CmdHsRTXmtStartDel() */
#define API_HS_RT_MODE_SIMULATION           0
#define API_HS_RT_MODE_MAILBOX              1

/* function Api3910CmdHsRTMIDDef() */
#define API_HS_RT_TYPE_RECEIVE_MID          0
#define API_HS_RT_TYPE_TRANSMIT_MID         1
#define API_HS_RT_TYPE_MODECODE             2

#define API_HS_RT_DISABLE_INT               0
#define API_HS_RT_ENABLE_INT_XFER           1
#define API_HS_RT_ENABLE_INT_ERR            2

#define API_BUF_OVERRUN                     3

/* function Api3910CmdHsRTModeCtrl() */
#define API_HS_RT_MODE_CONFIGURED_DYTAGS    3

/* function Api3910CmdHsBMTrgCon */
#define API_HS_BM_TRG_OR                    1
#define API_HS_BM_TRG_AND                   2

#define API_EF_BM_HW_TRG_MASK               0x0800
#define API_EF_BM_CMD_TRG_MASK              0x0400
#define API_HS_BM_LS_TRG_MASK               0x0300
#define API_HS_BM_ERR_TRG_MASK              0x00C0
#define API_HS_BM_DW_TRG_MASK               0x0030
#define API_HS_BM_HW_TRG_MASK               0x000C
#define API_HS_BM_AW_TRG_MASK               0x0003
#define API_EF_BM_HW_TRG_POS                11
#define API_EF_BM_CMD_TRG_POS               10
#define API_HS_BM_LS_TRG_POS                8
#define API_HS_BM_ERR_TRG_POS               6
#define API_HS_BM_DW_TRG_POS                4
#define API_HS_BM_HW_TRG_POS                2
#define API_HS_BM_AW_TRG_POS                0

/* function Api3910CmdHsBMStackpRead */
#define API_HS_BM_TRG_NOT_OCCURED           0
#define API_HS_BM_TRG_EVENT_OCCURED         1
#define API_HS_BM_TRG_BF_OCCURED            2

/* function Api3910CmdHsBMStackEntryRead */
#define API_HS_BM_BCTYPE_BCRT               1
#define API_HS_BM_BCTYPE_RTBC               2
#define API_HS_BM_BCTYPE_RTRT               3

/* function Api3910CmdHsBMStackEntryFind */
#define API_HS_BM_ENTRY_BUS_WORD            0
#define API_HS_BM_ENTRY_ERROR_WORD          1
#define API_HS_BM_ENTRY_LS_CORRELATION      2
#define API_HS_BM_ENTRY_LS_CA_WORD          3

/* function Api3910CmdHsCalCplCon */
/* function Api3910CmdHsCalSigCon */
#define API_HS_CAL_CHANNEL_A                1
#define API_HS_CAL_CHANNEL_B                2

/*************************************/
/*** GPIO Constants                ***/
/*************************************/
#define API_HS_MAX_GPIO_DEVICES                8

#define API_HS_GPIO_RESET             0 /* do a reset */
#define API_HS_GPIO_RESET_INFO        1 /* only read reset info */

/* some device type specifications */
#define API_HS_GPIO_DEVTYPE_RESERVED    0x00    
#define API_HS_GPIO_TYPE_VOX3910CTX        (API_HS_GPIO_DEVTYPE_RESERVED + 1)

/* some command specificaitons */
#define API_HS_GPIO_CMD_RESET      0x01    /* 00000001b */
#define API_HS_GPIO_CMD_BITE       0x02    /* 00000010b */
#define API_HS_GPIO_CMD_ALL_ON     0x03    /* 00000011b */
#define API_HS_GPIO_CMD_ALL_OFF    0x04    /* 00000100b */
#define API_HS_GPIO_CMD_SYS1_START 0x05    /* 00000101b */
#define API_HS_GPIO_CMD_SYS1_END   0x07    /* 00000111b */
#define API_HS_GPIO_CMD_SDEV       0x08    /* 00001dddb */
#define API_HS_GPIO_CMD_SYS2_START 0x10    /* 00010000b */
#define API_HS_GPIO_CMD_SYS2_END   0x3F    /* 00111111b */
#define API_HS_GPIO_CMD_SDREG      0x40    /* 01aaaaaab */
#define API_HS_GPIO_CMD_DEV_START  0x80    /* 10000000b */
#define API_HS_GPIO_CMD_DEV_END    0xFF    /* 11111111b */

/* CTX data registers */
#define API_HS_CTX_REG_MODE          0x00
#define API_HS_CTX_REG_BITE          0x01
#define API_HS_CTX_REG_VERSION       0x04
#define API_HS_CTX_REG_PULSE         0x10
#define API_HS_CTX_REG_RISE          0x11
#define API_HS_CTX_REG_FALL          0x12
#define API_HS_CTX_REG_OVRSHOOT      0x13

/* some status specificaitons */
#define API_HS_CTX_STATUS_DRDY                0x80    /* 100R31.10.00b  Device ready        */
#define API_HS_CTX_STATUS_FAIL                0x40    /* 010R31.10.00b  Selftest failed     */
#define API_HS_CTX_STATUS_ICMD                0x20    /* 001R31.10.00b  Command in progress */
#define API_HS_CTX_STATUS_DRDY_POS        7       /* 100R31.10.00b  Device ready        */
#define API_HS_CTX_STATUS_FAIL_POS        6       /* 010R31.10.00b  Selftest failed     */
#define API_HS_CTX_STATUS_ICMD_POS        5       /* 001R31.10.00b  Command in progress */

#define API_HS_CTX_RISE_MIN           7.5
#define API_HS_CTX_RISE_MAX           17.5

#define API_HS_CTX_FALL_MIN           7.5
#define API_HS_CTX_FALL_MAX           18.5

#define API_HS_CTX_PULSE_MIN          -12.5
#define API_HS_CTX_PULSE_MAX          12.5

#define API_HS_CTX_OVRSHOOT_MIN       4.0
#define API_HS_CTX_OVRSHOOT_MAX       26.0

typedef struct ty_api_hs_gpio_res_inf 
{
    AiUInt8  uc_DeviceId; 
    AiUInt8  uc_DeviceType;
    AiUInt16 uw_Padding1;
} TY_API_HS_GPIO_RES_INF;

/***************************************************************************
 **  API3910 Structures - Typedefs
 ***************************************************************************/

/*************************************************
 **                API-System                   **
 *************************************************/
typedef struct ty_api_hs_reset_info
{
    AiUInt8   uc_LsMBufs;
    AiUInt8   uc_LsSBufs;
    AiUInt8   uc_HsMBufs;
    AiUInt8   uc_HsSBufs;
    AiUInt32  ul_LsMonAddr;
    AiUInt32  ul_LsSimAddr;
    AiUInt32  ul_HsMonAddr;
    AiUInt32  ul_HsSimAddr;
} TY_API_HS_RESET_INFO;

typedef struct ty_api_hs_systag
{
    AiUInt16 uw_XidRtMid;
    AiUInt16 uw_Fct;
    AiUInt16 uw_Min;
    AiUInt16 uw_Max;
    AiUInt16 uw_Step;
    AiUInt16 uw_WPos;
    AiUInt16 uw_BPos;
} TY_API_HS_SYSTAG;

typedef struct ty_api_hs_bh_modify
{
  AiUInt32 ul_BHAddr;
  AiUInt32 ul_DataBufferStoreMode;
  AiUInt32 ul_WordCountMode;
  AiUInt32 ul_BufferSize;
  AiUInt32 ul_BufferQueueMode;
  AiUInt32 ul_BufferQueueSize;
  AiUInt32 ul_StatusQueueSize;
  AiUInt32 ul_EventQueueSize;
  AiUInt32 ul_CurrentBufferIndex;
} TY_API_HS_BH_MODIFY;

typedef struct ty_api_hs_track_def_in
{
  AiUInt32 ul_TrackId;
  AiUInt32 ul_BT;
  AiUInt32 ul_XidRtMid;
  AiUInt32 ul_Mode;
  AiUInt32 ul_TrackStartPos;
  AiUInt32 ul_TrackBPos;
  AiUInt32 ul_TrackBLen;
  AiUInt32 ul_TrackLen;
  AiUInt32 ul_MultiplexedTrackNb;
  AiUInt32 ul_ContinousTrackNb;
  AiInt32  l_Offset;
} TY_API_HS_TRACK_DEF_IN;

typedef struct ty_api_hs_track_def_out
{
  AiUInt32 ul_TrackBufferStartAddr;
} TY_API_HS_TRACK_DEF_OUT;

typedef struct ty_api_hs_track_read_in
{
  AiUInt32 ul_TrackId;
  AiUInt32 ul_MultiplexedTrackIndex;
  AiUInt32 ul_ReadMode;
} TY_API_HS_TRACK_READ_IN;

typedef struct ty_api_hs_track_read_out
{
  AiUInt32 ul_DataValid;
  AiUInt32 ul_LastTT;
  AiUInt16 *puw_TrackDataWords;
} TY_API_HS_TRACK_READ_OUT;

typedef struct ty_api_hs_track_scan_in
{
  AiUInt32 ul_TrackId;
  AiUInt32 ul_ChunkNb;
  AiUInt32 ul_ChunkSize;
} TY_API_HS_TRACK_SCAN_IN;

typedef struct ty_api_hs_track_scan_out
{
  AiUInt32 ul_NumberOfReturnedStates;
  AiUInt32 ul_MoreData;
  AiUInt32 *pul_ReturnedStates;
} TY_API_HS_TRACK_SCAN_OUT;

typedef struct ty_api_hs_track_prealloc_in
{
  AiUInt32 ul_TrackId;
  AiUInt32 ul_PreAllocNb;
  AiUInt32 *pul_MuxStates;
} TY_API_HS_TRACK_PREALLOC_IN;

typedef struct ty_api_hs_track_prealloc_out
{
  AiUInt32 *pul_TrackBufferStartAddr;
} TY_API_HS_TRACK_PREALLOC_OUT;

/*************************************************
 **                API-BC                       **
 *************************************************/
typedef struct ty_api_hs_bc_bh_info
{
    AiUInt16 uw_SqId;
    AiUInt16 uw_EqId;
    AiUInt16 uw_NBufs;
    AiUInt16 uw_Padding1;
    AiUInt32 ul_Mem;
    AiUInt32 ul_HIdAddr;
    AiUInt32 ul_BqAddr;
    AiUInt32 ul_SqAddr;
    AiUInt32 ul_EqAddr;
    AiUInt32 ul_HeaderId2;
    AiUInt32 ul_LsHIdAddr1;
    AiUInt32 ul_LsBqAddr1;
    AiUInt32 ul_LsSqAddr1;
    AiUInt32 ul_LsEqAddr1;
    AiUInt32 ul_LsHIdAddr2;
    AiUInt32 ul_LsBqAddr2;
    AiUInt32 ul_LsSqAddr2;
    AiUInt32 ul_LsEqAddr2;
} TY_API_HS_BC_BH_INFO;

typedef struct ty_api_hs_bc_err
{
    AiUInt8  uc_Type;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_ErrSpec;
} TY_API_HS_BC_ERR;

typedef struct ty_api_hsls_bc_xfer
{
    AiUInt32 ul_Gap;
    AiUInt32 ul_Swxm;
} TY_API_HSLS_BC_XFER;

typedef struct ty_api_hs_bc_xfer
{
	struct ty_api_hs_bc_xfer_x_common
    {
        AiUInt16              uw_XferId;
        AiUInt16              uw_HeaderId;
        AiUInt8               uc_XferType;
        AiUInt8               uc_XferHalt;
        AiUInt8               uc_MsgSize;
        AiUInt8               uc_Padding1;
        AiUInt16              uw_Rt;
        AiUInt16              uw_Mid;
        AiUInt8               uc_StatusWordException;
        AiUInt8               uc_RetryControl;
        AiUInt16              uw_Padding2;
        TY_API_HSLS_BC_XFER x_FirstXfer;
        TY_API_HSLS_BC_XFER x_SecondXfer;
    } x_Common;

	struct ty_api_hs_bc_xfer_x_ls
    {
        AiUInt32            ul_LsChnIr;
        TY_API_BC_ERR            x_LsErr;
    } x_Ls;

	struct ty_api_hs_bc_xfer_x_hs
    {
        AiUInt32            ul_HsChnIr;
        AiUInt32            ul_HsXmt;
        TY_API_HS_BC_ERR  x_HsErr;
    } x_Hs;
} TY_API_HS_BC_XFER;


typedef struct ty_api_hs_bc_xfer_info
{
  AiUInt32 ul_LsXferDescAddr1;
  AiUInt32 ul_LsXferDescAddr2;
  AiUInt32 ul_HsXferDescAddr;
} TY_API_HS_BC_XFER_INFO;

typedef struct ty_api_hs_bc_dytag
{
    AiUInt8  uc_DytagWordMode;
    AiUInt8  uc_Mode;
    AiUInt16 uw_FctWord;
    AiUInt16 uw_WPos;
} TY_API_HS_BC_DYTAG;

typedef struct ty_api_hs_bc_xfer_status_xfer
{
    AiUInt16 uw_CmdWord;
    AiUInt16 uw_StatusWord;
    AiUInt16 uw_XferRepWord;
    AiUInt16 uw_ActionWord;
    AiUInt32 ul_TimeTag;
    AiUInt32 ul_XferCnt;
    AiUInt32 ul_ErrCnt;
    AiUInt32 ul_LastVectorWord;
    AiUInt32 ul_ServiceRequestCnt;
} TY_API_HS_BC_XFER_STATUS_XFER;

typedef struct ty_api_hs_bc_xfer_status
{
    TY_API_HS_BC_XFER_STATUS_XFER x_First;
    TY_API_HS_BC_XFER_STATUS_XFER x_Second;
    AiUInt16 uw_SqDa;
    AiUInt16 uw_SqFrmCtrlWord;
    AiUInt16 uw_SqWCntWord;
    AiUInt16 uw_SqMsgTagWord;
    AiUInt16 uw_SqEntryCtrlWord;
    AiUInt16 uw_Padding1;
    AiUInt32 ul_SqActBufPtr;
    AiUInt32 ul_SqTimeTag;
} TY_API_HS_BC_XFER_STATUS;

/* Api3910CmdHsBCXferReadEx */
typedef struct ty_api_hs_bc_xfer_status_queue
{
  AiUInt32 ul_SqCtrlWord;
  AiUInt32 ul_SqCmdActWord;
  AiUInt32 ul_SqActBufPtr;
  AiUInt32 ul_SqFCPADAWord;
  AiUInt32 ul_SqWCntMsgTagWord;
  AiUInt32 ul_SqTimeTag;
} TY_API_HS_BC_XFER_STATUS_QUEUE;

typedef struct ty_api_hs_bc_xfer_status_info
{
  AiUInt32 ul_ActBufferId;
  AiUInt32 ul_XferCnt;
  AiUInt32 ul_ErrCnt;
} TY_API_HS_BC_XFER_STATUS_INFO;

typedef struct ty_api_hs_bc_xfer_status_ex
{
  AiUInt32                         ul_BufferQueueSize;
  TY_API_BC_XFER_STATUS_INFO       x_LsXfer1Info;
  TY_API_BC_XFER_STATUS_INFO       x_LsXfer2Info;
  TY_API_HS_BC_XFER_STATUS_INFO    x_HsXferInfo;
  TY_API_BC_XFER_STATUS_QUEUE      x_LsXfer1SQueue[256];
  TY_API_BC_XFER_STATUS_QUEUE      x_LsXfer2SQueue[256];
  TY_API_HS_BC_XFER_STATUS_QUEUE   x_HsXferSQueue[256];
  TY_API_BC_XFER_EVENT_QUEUE       x_LsXfer1EQueue[1];
  TY_API_BC_XFER_EVENT_QUEUE       x_LsXfer2EQueue[1];
  TY_API_BC_XFER_EVENT_QUEUE       x_HsXferEQueue[1];
} TY_API_HS_BC_XFER_STATUS_EX;

typedef struct ty_api_hs_bc_xfer_read_in
{
  AiUInt32 ul_XferId;
  AiUInt32 ul_Clear;
  AiUInt32 ul_Mode;
} TY_API_HS_BC_XFER_READ_IN;

typedef struct ty_api_hs_bc_status
{
  AiUInt32 ul_GlbMsgCnt;
  AiUInt32 ul_GlbErrCnt;
  AiUInt32 ul_Reserved1;
  AiUInt32 ul_Reserved2;
  AiUInt32 ul_Reserved3;
  AiUInt32 ul_Reserved4;
} TY_API_HS_BC_STATUS;

typedef struct ty_api_hs_bc_mode_ctrl
{
  AiUInt32 ul_BcMode;
  AiUInt32 ul_Ctrl;
  AiUInt32 ul_Param1;
  AiUInt32 ul_Param2;
  AiUInt32 ul_Param3;
} TY_API_HS_BC_MODE_CTRL;

/*************************************************
 **                API-RT                       **
 *************************************************/

typedef struct ty_api_hs_rt_err
{
    AiUInt8  uc_Type;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_ErrSpec;
} TY_API_HS_RT_ERR;

typedef struct ty_api_hs_rt_bh_info
{
    AiUInt16 uw_SqId;
    AiUInt16 uw_EqId;
    AiUInt16 uw_NBufs;
    AiUInt16 uw_Padding1;
    AiUInt32 ul_BhAddr;
    AiUInt32 ul_BqAddr;
    AiUInt32 ul_SqAddr;
    AiUInt32 ul_EqAddr;
} TY_API_HS_RT_BH_INFO;

typedef struct ty_api_hs_rt_dytag
{
    AiUInt8  uc_DytagWordMode;
    AiUInt8  uc_Mode;
    AiUInt16 uw_FctWord;
    AiUInt16 uw_WPos;
} TY_API_HS_RT_DYTAG;

typedef struct ty_api_hs_rt_status_dsp
{
    AiUInt16 uw_HsStatusWord;
    AiUInt16 uw_HsActionWord;
    AiUInt32 ul_HsMsgCnt;
    AiUInt32 ul_HsErrCnt;
}TY_API_HS_RT_STATUS_DSP;

typedef struct ty_api_hs_rt_mid_status
{
    AiUInt16 uw_HsStatusRepWord;
    AiUInt16 uw_HsSqDa;
    AiUInt16 uw_HsSqFrmCtrlWord;
    AiUInt16 uw_HsSqWCntWord;
    AiUInt16 uw_HsSqMsgTag;
    AiUInt16 uw_HsActionWord;
    AiUInt32 ul_HsCurBufPtr;
    AiUInt32 ul_HsCurTimeTag;
}TY_API_HS_RT_MID_STATUS;

/* Api3910CmdHsRTMIDReadEx */
typedef struct ty_api_hs_rt_mid_status_queue
{
  AiUInt32 ul_SqCtrlWord;
  AiUInt32 ul_SqCmdActWord;
  AiUInt32 ul_SqActBufPtr;
  AiUInt32 ul_SqFCPADAWord;
  AiUInt32 ul_SqWCntMsgTagWord;
  AiUInt32 ul_SqTimeTag;
} TY_API_HS_RT_MID_STATUS_QUEUE;

typedef struct ty_api_hs_rt_mid_status_info
{
  AiUInt32 ul_ActBufferId;
  AiUInt32 ul_XferCnt;
  AiUInt32 ul_ErrCnt;
} TY_API_HS_RT_MID_STATUS_INFO;

typedef struct ty_api_hs_rt_mid_status_ex
{
  AiUInt32                         ul_BufferQueueSize;
  TY_API_RT_SA_STATUS_INFO         x_LsXferInfo;
  TY_API_HS_RT_MID_STATUS_INFO     x_HsXferInfo;
  TY_API_RT_SA_STATUS_QUEUE        x_LsXferSQueue[256];
  TY_API_HS_RT_MID_STATUS_QUEUE    x_HsXferSQueue[256];
  TY_API_RT_SA_EVENT_QUEUE         x_LsXferEQueue[1];
  TY_API_RT_SA_EVENT_QUEUE         x_HsXferEQueue[1];
} TY_API_HS_RT_MID_STATUS_EX;

typedef struct ty_api_hs_rt_mid_read_in
{
  AiUInt32 ul_RtAddr;
  AiUInt32 ul_MsgId;
  AiUInt32 ul_MsgIdType;
  AiUInt32 ul_ClrBufStat;
  AiUInt32 ul_Mode;
} TY_API_HS_RT_MID_READ_IN;

typedef struct ty_api_hs_rt_mode_ctrl
{
  AiUInt32 ul_BcMode;
  AiUInt32 ul_Ctrl;
  AiUInt32 ul_Param1;
  AiUInt32 ul_Param2;
  AiUInt32 ul_Param3;
} TY_API_HS_RT_MODE_CTRL;

/************************************
 ** Replay structure
 ************************************/
typedef struct ty_api_hs_rep_status
{
    AiUInt8  uc_Status;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_RpiCnt;
    AiUInt32 ul_SAddr;
    AiUInt32 ul_Size;
    AiUInt32 ul_MsgCnt;
} TY_API_HS_REP_STATUS;

/*************************************************
 **                API-BM                       **
 *************************************************/
typedef struct ty_api_hs_bm_msg_flt
{
    AiUInt32 ul_MceRxMid1;
    AiUInt32 ul_MceRxMid2;
    AiUInt32 ul_MceRxMid3;
    AiUInt32 ul_MceRxMid4;
    AiUInt32 ul_MceRxMid5;
    AiUInt32 ul_MceRxMid6;
    AiUInt32 ul_MceRxMid7;
    AiUInt32 ul_MceRxMid8;
    AiUInt32 ul_MceTxMid1;
    AiUInt32 ul_MceTxMid2;
    AiUInt32 ul_MceTxMid3;
    AiUInt32 ul_MceTxMid4;
    AiUInt32 ul_MceTxMid5;
    AiUInt32 ul_MceTxMid6;
    AiUInt32 ul_MceTxMid7;
    AiUInt32 ul_MceTxMid8;
} TY_API_HS_BM_MSG_FLT;

typedef struct ty_api_hs_bm_status
{
    AiUInt8  uc_Men;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_Msw;
    AiUInt32 ul_GlbMsgCnt;
    AiUInt32 ul_GlbErrCnt;

    /* new fw counter support */
    AiUInt32 ul_GlbWordCntB;
    AiUInt32 ul_GlbWordCntA;
    AiUInt32 ul_GlbMsgCntB;
    AiUInt32 ul_GlbMsgCntA;
    AiUInt32 ul_GlbErrCntB;
    AiUInt32 ul_GlbErrCntA;

    /* bus load indication (generated by ASP) */
    AiUInt32 ul_BusLoadB;     /* last 500ms */
    AiUInt32 ul_BusLoadA;     /* last 500ms */
    AiUInt32 ul_BusLoadBAvg; /* since BM start */
    AiUInt32 ul_BusLoadAAvg; /* since BM start */
} TY_API_HS_BM_STATUS;

typedef struct ty_api_hs_bm_trg_cond
{
    AiUInt32  ul_ErrTrgWord;
    AiUInt32  ul_MonDataWord;
    AiUInt32  ul_MonCwActWord;
    AiUInt32  ul_MonCwActMask;
    AiUInt32  ul_MonFcPaWord;
    AiUInt32  ul_MonFcPaMask;
} TY_API_HS_BM_TRG_COND;

typedef struct ty_api_hs_bm_act
{
    AiUInt8  uc_Fnd;
    AiUInt8  uc_Rt;
    AiUInt8  uc_Mid;
    AiUInt8  uc_Type;
    AiUInt32 ul_MsgCnt;
    AiUInt32 ul_ErrCnt;
    AiUInt32 ul_ErrTypeWord;
} TY_API_HS_BM_ACT;

typedef struct ty_api_hs_bm_rt_act
{
    AiUInt32 ul_MsgCnt;
    AiUInt32 ul_ErrCnt;
    AiUInt32 ul_ErrTypeWord;
} TY_API_HS_BM_RT_ACT;

typedef struct ty_api_hs_bm_stackp_dsp
{
    AiUInt8  uc_Status;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_StpCtpCnt;
    AiUInt16 uw_CtpEtpCnt;
    AiUInt16 uw_EntryCnt;
    AiUInt32 ul_Stp;
    AiUInt32 ul_Ctp;
    AiUInt32 ul_Etp;
} TY_API_HS_BM_STACKP_DSP;

typedef struct ty_api_hs_bm_stack_entry
{
    AiUInt16 uw_TxInitTime;
    AiUInt16 uw_HsBufTagWord;
    AiUInt32 ul_HsTimetagHigh;
    AiUInt32 ul_HsTimetagLow;
    AiUInt16 uw_LsCmdWord;
    AiUInt16 uw_LsActWord;
    AiUInt32 ul_LsCtrlWord;
    AiUInt16 uw_LsCmdWord2;
    AiUInt16 uw_LsActWord2;
    AiUInt32 ul_LsCtrlWord2;
    AiUInt32 ul_FcPaDa;
    AiUInt16 uw_WCntWord;
    AiUInt16 uw_EntryAmount;
    AiUInt16 auw_Data[ API_HS_MAX_DATA_WORDS ];
    AiUInt32 ul_Reserved1;
    AiUInt32 ul_Reserved2;
    AiUInt32 ul_Reserved3;
    AiUInt32 ul_Reserved4;
    AiUInt16 uw_Reserved5;
    AiUInt16 uw_Fcs;
} TY_API_HS_BM_STACK_ENTRY;


/* Monitor buffer entry at high speed recording */
typedef struct ty_api_hs_bm_fw_mon_buffer_entry
{
     AiUInt32 ul_BufHeadPointer;
     AiUInt32 ul_NextBufPointer;
     AiUInt16 uw_HsBufTagWord;
     AiUInt16 uw_TxInitTime;
     AiUInt32 ul_HsTimetagHigh;
     AiUInt32 ul_HsTimetagLow;
     AiUInt16 uw_LsActWord;
     AiUInt16 uw_LsCmdWord;
     AiUInt32 ul_LsCtrlWord;
     AiUInt16 uw_LsActWord2;
     AiUInt16 uw_LsCmdWord2;
     AiUInt32 ul_LsCtrlWord2;
     AiUInt32 aul_reserved[4];
     AiUInt16 uw_Fcs;
     AiUInt16 uw_reserved;
     AiUInt16 uw_Da;
     AiUInt16 uw_FcPa;
     AiUInt16 uw_EntryAmount;
     AiUInt16 uw_WCntWord;
     AiUInt16 auw_Data[    API_HS_MAX_DATA_WORDS ];
} TY_API_HS_BM_FW_MON_BUFFER_ENTRY;



typedef struct ty_api_hs_bm_stack_fnd
{
    AiUInt8   uc_EntryFnd;
    AiUInt8   uc_Eos;
    AiUInt16  uw_EntryNr;
    AiUInt16  uw_WordNr;
} TY_API_HS_BM_STACK_FND;

typedef struct ty_api_hs_bm_rec
{
    AiUInt8  uc_Status;
    AiUInt8  uc_Padding1;
    AiUInt16 uw_Padding2;
    AiUInt32 ul_HfiCnt;
    AiUInt32 ul_SAddr;
    AiUInt32 ul_Size;
} TY_API_HS_BM_REC;


/* Defines for Frame Mod Select  */
#define API_HS_FMOD_NORM              0
#define API_HS_FMOD_CONT_PREAM        1
#define API_HS_FMOD_DDL               2
#define API_HS_FMOD_CONT_PREAM_DDL    3

#define API_HS_RT_EFEX_PROTOCOL       0
#define API_HS_RT_EFA_PROTOCOL        1

#define MAX_MID_MCODE                  32

/* EFEX Transfers*/
#define APIEF_BC_HS_XFER                    20
#define APIEF_BC_XFER_TYPE_XXRTRT         APIEF_BC_HS_XFER            /* EFEX to EFEX            */
#define APIEF_BC_XFER_TYPE_X_RTBR         (APIEF_BC_HS_XFER + 1)        /* EFEX to EFEX  Broadcast */
#define APIEF_BC_XFER_TYPE_XERTRT       (APIEF_BC_HS_XFER + 2)        /* EFEX to EFA             */
#define APIEF_BC_XFER_TYPE_X_RTBRMIXED  (APIEF_BC_HS_XFER + 3)        /* EFEX to MIX   Broadcast */
#define APIEF_BC_XFER_TYPE_EXRTRT         (APIEF_BC_HS_XFER + 4)        /* EFA  to EFEX            */
#define APIEF_BC_XFER_TYPE_EXRTBR         (APIEF_BC_HS_XFER + 5)        /* EFA  to EFEX  Broadcast */
#define APIEF_BC_XFER_TYPE_EERTRT       (APIEF_BC_HS_XFER + 6)        /* EFA  to EFA             */
#define APIEF_BC_XFER_TYPE_E_RTBR       (APIEF_BC_HS_XFER + 7)        /* EFA  to EFA   Broadcast */
#define APIEF_BC_XFER_TYPE_E_RTBRMIXED    (APIEF_BC_HS_XFER + 8)        /* EFA  to MIX   Broadcast */
#define APIEF_BC_XFER_TYPE_MCRT         (APIEF_BC_HS_XFER + 9)        /* EFEX Modecode           */
#define APIEF_BC_XFER_TYPE_MCBR         (APIEF_BC_HS_XFER + 10)        /* EFEX Modecode Broadcast */


/* Api3910CmdEfRTIni */
#define API_EF_RTMODE_DUAL        0
#define API_EF_RTMODE_EFA         1
#define API_EF_RTMODE_EFEX        2

/* Api3910CmdEfBMTrigType*/
#define API_EF_TRIG_SD         0
#define API_EF_TRIG_S          1

#define API_EF_TRIG_EFEX_CMD   0
#define API_EF_TRIG_EFA_CMD    1

/* Api3910CmdEfSwitchEfaEfex */
#define API_EF_SWITCH_TO_EFA             1
#define API_EF_SWITCH_TO_EFEX            2
#define API_EF_SWITCH_TO_EFA_TEMPORARY   3
#define API_EF_SWITCH_TO_EFEX_TEMPORARY  4


#define API_EF_SWITCH_ERR      0
#define API_EF_SWITCH_OK       1

/*=====================================================*/

#define MAX_HS_DYTAG_WORD_MODE     API_DYTAG_FUNCTION_MODIFY
#define MAX_HS_WPOS                (API_HS_MAX_DATA_WORDS-1)
#define MAX_HS_XFER_HALT           API_BC_HLT_HALT_ON_ANY_INT
#define MAX_HS_MODECODE            31
#define MAX_HS_SW_EXCEPTION        API_BC_SRVW_MULTIPLE
#define MAX_HS_GAPMODE             API_BC_GAP_MODE_FAST
#define MAX_HS_GAP_TIME_MODE_01    16383
#define MAX_HS_GAP_TIME_MODE_2     63
#define MAX_LS_BUS                 API_BC_XFER_BUS_SECONDARY
#define MAX_LS_IR                  API_BC_TIC_INT_ON_STAT_EXCEPT
#define MAX_HS_BUS                 API_HS_CHANNEL_B
#define MAX_HS_IR                  API_BC_TIC_INT_ON_XFER_ERR
#define MAX_HS_NOPRE               255
#define MAX_HS_STARTDEL            1023
#define MAX_HS_INT_MODE            API_BM_MODE_STOP_EVENT_INT
#define MAX_HS_STROBE_MODE         API_BM_STROBE_ON_STOP_EVENT
#define MAX_HS_TRG_SEL             API_HS_BM_TRG_AND
#define MAX_HS_ENTRY_NR            8191
#define MAX_HS_MIDTYPE             1
#define MAX_HS_BM_ENTRY_TYPE       API_HS_BM_ENTRY_LS_CA_WORD
#define MAX_HS_CPL_MODE            API_CAL_CPL_EXT_WRAP_AROUND_LOOP
#define MAX_HS_RT_MODE             API_HS_RT_MODE_MAILBOX
#define MAX_HS_START_DELAY         1023
#define MAX_HS_MSGIDTYPE           API_HS_RT_TYPE_MODECODE
#define MAX_HS_MID_INT             API_HS_RT_ENABLE_INT_ERR
#define MAX_HS_ERR_TYPE            API_EF_ERR_TYPE_ILLEGAL_WORDCOUNT
#define MAX_HS_ERR_WPOS            4099
#define MAX_HS_MBUF                32
#define MAX_HS_RX_INIT_TIMEOUT     1023
#define MAX_HS_WCM                 API_HS_WCM_STATUS_QUEUE
#define MAX_HS_FMOD_CONT           API_HS_FMOD_CONT_PREAM_DDL

#define MAX_HS_RT_PROTOCOL         API_HS_RT_EFA_PROTOCOL

#define MAX_EF_BM_STRIG_TYPE       API_EF_TRIG_S

#define MAX_EF_BM_CWTRIG_TYPE      API_EF_TRIG_EFA_CMD

#define MAX_EF_RTMODE              API_EF_RTMODE_EFEX


#define MIN_HS_MID                 1
#define MAX_HS_MID                 255/*usage uncommented due out of range*/

#define MIN_HS_HEADER_ID           1
#define MAX_HS_HEADER_ID           8191/*2048*/

#define MIN_HS_XFER_ID             1
#define MAX_HS_XFER_ID             4095/*1023*/

#define MIN_HS_BUFFER_ID           1
#define MAX_HS_BUFFER_ID           65535/*14335*/ /*usage uncommented*/

#define MIN_HS_MSGSIZE             1
#define MAX_HS_MSGSIZE             127

#define MIN_HS_XFER_TYPE           API_HS_BC_TYPE_BCRT
#define MAX_HS_XFER_TYPE           APIEF_BC_XFER_TYPE_MCBR

#define MIN_HS_BUFSIZE             3
#define MAX_HS_BUFSIZE             (API_HS_MAX_DATA_WORDS+32+3)

#define MIN_HS_ERR_BPOS            0
#define MAX_HS_ERR_BPOS            15

#define MIN_HS_ERR_BCBITS          1
#define MAX_HS_ERR_BCBITS          15

#define MIN_HS_CONT_TRACK_NB       1
#define MAX_HS_CONT_TRACK_NB       128

#define MIN_HS_TRACK_LEN           1
#define MAX_HS_TRACK_LEN           4096


#define MIN_EF_SWITCHTYPE          API_EF_SWITCH_TO_EFA

#define MAX_EF_SWITCHTYPE          API_EF_SWITCH_TO_EFEX_TEMPORARY

#define MIN_HS_BH_BUF_SIZE         0
#define MAX_HS_BH_BUF_SIZE         127

#define MIN_HS_BC_MODE_CTRL  API_HS_BC_MODE_CONFIGURED_DYTAGS
#define MAX_HS_BC_MODE_CTRL  API_HS_BC_MODE_CONFIGURED_DYTAGS

#define MIN_HS_RT_MODE_CTRL  API_HS_RT_MODE_CONFIGURED_DYTAGS
#define MAX_HS_RT_MODE_CTRL  API_HS_RT_MODE_CONFIGURED_DYTAGS

/***************************************************************************
 **  API3910 EFEX structures and constants
 ***************************************************************************/
typedef struct  ty_api_ef_bm_stack_entry
{
  AiUInt32 ul_BufHeadPtr;
  AiUInt32 ul_NextBufPtr;
  AiUInt32 ul_HsBufTagWord;
  AiUInt32 ul_HsTimetagHigh;
  AiUInt32 ul_HsTimetagLow;
  AiUInt16 uw_FrameDAW;
  AiUInt16 uw_FramePAW;
  AiUInt16 uw_FrameTagWord;
  AiUInt16 uw_FrameWordCnt;
  AiUInt16 uw_ModeDataWord2;
  AiUInt16 uw_Res1;
  AiUInt16 uw_SFrameADWWord;
  AiUInt16 uw_SFramePAWord;
  AiUInt16 uw_SFrameTagWord;
  AiUInt16 uw_Res2;
  AiUInt16 uw_SDFrameTagWord;
  AiUInt16 uw_SDFrameXmtWordCnt;
  AiUInt16 uw_LsActWord;
  AiUInt16 uw_LsCmdWord;
  AiUInt16 uw_LsActWord2;
  AiUInt16 uw_LsCmdWord2;
  AiUInt32 ul_Res5;
  AiUInt16 uw_SDFrameDSIWord;
  AiUInt16 uw_SDFrameFCPA;
  AiUInt16 uw_EntryAmount;
  AiUInt16 uw_SDFrameRcvWordCnt;
  AiUInt16 auw_Data[ API_HS_MAX_DATA_WORDS ];
}TY_API_EF_BM_STACK_ENTRY;

/* Monitor buffer entry at efex recording */
typedef struct ty_api_ef_bm_fw_mon_buffer_entry
{
    AiUInt32 ul_BufHeadPointer;
    AiUInt32 ul_NextBufPointer;
    AiUInt32 ul_HsBufTagWord;
    AiUInt32 ul_HsTimetagHigh;
    AiUInt32 ul_HsTimetagLow;
    AiUInt16 uw_FrameDAW;
    AiUInt16 uw_FramePAW;
    AiUInt16 uw_FrameTagWord;
    AiUInt16 uw_FrameWordCnt;
    AiUInt16 uw_ModeDataWord2;
    AiUInt16 uw_Res1;
    AiUInt16 uw_SFrameADWWord;
    AiUInt16 uw_SFramePAWord;
    AiUInt16 uw_SFrameTagWord;
    AiUInt16 uw_Res2;
    AiUInt16 uw_SDFrameTagWord;
    AiUInt16 uw_SDFrameXmtWordCnt;
    AiUInt16 uw_LsActWord;
    AiUInt16 uw_LsCmdWord;
    AiUInt16 uw_LsActWord2;
    AiUInt16 uw_LsCmdWord2;
    AiUInt32 ul_Res5;
    AiUInt16 uw_SDFrameDSIWord;
    AiUInt16 uw_SDFrameFCPA;
    AiUInt16 uw_EntryAmount;
    AiUInt16 uw_SDFrameRcvWordCnt;
    AiUInt16 auw_Data[API_HS_MAX_DATA_WORDS];
} TY_API_EF_BM_FW_MON_BUFFER_ENTRY;


typedef struct ty_api_ef_bc_xfer
{
	struct ty_api_ef_bc_xfer_x_common
  {
    AiUInt16              uw_XferId;
    AiUInt16              uw_HeaderId;
    AiUInt8               uc_XferType;
    AiUInt8               uc_XferHalt;
    AiUInt8               uc_MsgSize;
    AiUInt8               uc_Padding1;
    AiUInt16              uw_Rt;
    AiUInt16              uw_Mid;
    AiUInt8               uc_StatusWordException;
    AiUInt8               uc_RetryControl;
    AiUInt16              uw_Padding2;
    AiUInt16              uw_WCount;
    AiUInt16              uw_EfexMCWordOne; 
    AiUInt16              uw_EfexMCWordTwo;
    TY_API_HSLS_BC_XFER   x_FirstXfer;
    TY_API_HSLS_BC_XFER   x_SecondXfer;
  } x_Common;

	struct ty_api_ef_bc_xfer_x_ls
  {
    AiUInt32         ul_LsChnIr;
    TY_API_BC_ERR       x_LsErr;
  } x_Ls;

	struct ty_api_ef_bc_xfer_x_hs
  {
    AiUInt32         ul_HsChnIr;
    AiUInt32         ul_HsXmt;
    TY_API_HS_BC_ERR x_HsErr;
  } x_Hs;
} TY_API_EF_BC_XFER;

typedef struct  ty_api_ef_rt_descriptor
{
    AiUInt32 ul_RtTransDescControl;
    AiUInt32 ul_RtTransStatus;
    AiUInt32 ul_RtTransCount;
    AiUInt32 ul_RtTransErrCount;
    AiUInt16 uw_NextStatus;
    AiUInt16 uw_LastStatus;
    AiUInt16 uw_NextADW;
    AiUInt16 uw_LastADW;
    AiUInt16 uw_CCMCDestAdd;
    AiUInt16 uw_CCMCFrameCont;
    AiUInt16 uw_CCMCMessTag;
    AiUInt16 uw_CCMCWCount;
    AiUInt16 uw_MCDataW2;
    AiUInt16 uw_MCDataW1;
    AiUInt32 ul_Res1;
}TY_API_EF_RT_DESCRIPTOR;

typedef struct  ty_api_ef_bc_xfer_stat
{
    AiUInt32 ul_TransDescControl;
    AiUInt32 ul_TransStatus;
    AiUInt32 ul_ErrSpec;
    AiUInt32 ul_TransCount;
    AiUInt32 ul_TransErrCount;
    AiUInt16 uw_CCDestAddress;
    AiUInt16 uw_CCFrameControl;
    AiUInt16 uw_CCWCountMCModeCom;
    AiUInt16 uw_MCModeCom;
    AiUInt16 uw_SFrameMessTag;
    AiUInt16 uw_Reserved;
    AiUInt16 uw_SFrameADW;
    AiUInt16 uw_SFrameControl;
    AiUInt16 uw_SDFrameMessTag;
    AiUInt16 uw_SDFrameWCount;
    AiUInt16 uw_SDFrameDestAddr;
    AiUInt16 uw_SDFrameControl;
    AiUInt32 ul_TimeTag;
    AiUInt16 uw_SFrameSatusWExcMask;
    AiUInt16 uw_SDFrameSatusWExcMask;
    AiUInt16 uw_BCTransWaitT;
    AiUInt16 uw_Reserved1;
    AiUInt16 uw_Reserved2;
    AiUInt16 uw_Reserved3;
}TY_API_EF_BC_XFER_STATUS;

typedef struct ty_api_ef_resp_sim_sdframe
{
    AiUInt32 ul_Bus;
    AiUInt32 ul_ErrorWordPos;
    AiUInt32 ul_ErrorType;
    AiUInt32 ul_BitCntOrPos;
    AiUInt32 ul_ResponseTime;
    AiUInt32 ul_FCPA;
    AiUInt32 ul_DestAddr;
    AiUInt32 ul_WordCount;
} TY_API_EF_BC_RESP_SIM_SDFRAME;
typedef struct ty_api_ef_resp_sim_sframe
{
    AiUInt32 ul_Bus;
    AiUInt32 ul_ErrorWordPos;
    AiUInt32 ul_ErrorType;
    AiUInt32 ul_BitCntOrPos;
    AiUInt32 ul_ResponseTime;
    AiUInt32 ul_FCPA;
    AiUInt32 ul_ADW;
} TY_API_EF_BC_RESP_SIM_SFRAME;

typedef struct ty_api_ef_bc_resp_sim
{
  AiUInt32 ul_XferId;
  AiUInt32 ul_HeaderId;
  TY_API_EF_BC_RESP_SIM_SDFRAME x_SDFrame;
  TY_API_EF_BC_RESP_SIM_SFRAME x_SFrame;
} TY_API_EF_BC_RESP_SIM;

typedef struct ty_api_ef_bc_resp_sim_mode
{
  AiUInt32 ul_XferId;
  AiUInt32 ul_ResponseMode;
  AiUInt32 ul_FastMode;
  AiUInt32 ul_ForceResponseOnError;
} TY_API_EF_BC_RESP_SIM_MODE;

typedef struct ty_api_ef_bc_internal_frame
{
    AiUInt32 ul_FCPA;
    AiUInt32 ul_DestAddr;
    AiUInt32 ul_WordCount;
  AiUInt32 ul_ModeCodeWords;
} TY_API_EF_BC_INTERNAL_FRAME;

typedef struct ty_api_ef_bc_internal
{
  AiUInt32 ul_XferId;
  AiUInt32 ul_HeaderId;
  TY_API_EF_BC_INTERNAL_FRAME x_BCInternalValidation;
} TY_API_EF_BC_INTERNAL;

typedef struct ty_api_ef_bc_internal_mode
{
  AiUInt32 ul_XferId;
  AiUInt32 ul_ForceLocalResponseOnError;
  AiUInt32 ul_SuppressLocalCCError;
  AiUInt32 ul_UseBCInternalValidation;
} TY_API_EF_BC_INTERNAL_MODE;


typedef struct ty_api_ef_bc_sync_time_def
{
  AiUInt32 ul_SyncTimeOffset;
} TY_API_EF_BC_SYNC_TIME_DEF;

typedef struct ty_api_ef_bc_sync_time_get
{
  AiUInt32 ul_SyncTimeOffset;
  AiUInt32 ul_LastRcvSyncTime;
  AiUInt32 ul_LastSyncTime;
} TY_API_EF_BC_SYNC_TIME_GET;

typedef struct ty_api_ef_rt_mc_def
{
  AiUInt32 ul_RtAddr;
  AiUInt32 ul_ModeCode;
  AiUInt32 ul_ModeCodeTx;
  AiUInt32 ul_ModeCodeBr;
  AiUInt32 ul_ModeCodeRx;
  AiUInt32 ul_StdIllegalHandling;
} TY_API_EF_RT_MC_DEF;

typedef struct ty_api_hs_rx_select
{
  AiUInt32 ul_RxBusSelect;
} TY_API_HS_RX_SELECT;

typedef struct ty_api_ef_rx_select
{
  AiUInt32 ul_RxBusSelect;
  AiUInt32 ul_FFlt;
} TY_API_EF_RX_SELECT;

typedef struct ty_api_ef_bm_trg_cond
{
    AiUInt32  ul_ErrTrgWord;
    AiUInt32  ul_MonDataWord;
    AiUInt32  ul_MonCmdWord;
    AiUInt32  ul_MonCmdMask;
    AiUInt32  ul_MonHdrWord;
    AiUInt32  ul_MonHdrMask;
} TY_API_EF_BM_TRG_COND;

#endif /* AI3910I_DEF_H */
/* end: ai3910i_def.h */


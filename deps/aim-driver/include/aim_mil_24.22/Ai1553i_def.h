/* SPDX-FileCopyrightText: 2001-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Ai1553i_def.h
 *
 *  Constants and structures for the 1553 Application Interface.
 *
 */

#ifndef AI1553I_DEF_H
#define AI1553I_DEF_H

#include "AiMilErrors.h"
#include "Ai1553Com.h"

/*************************************
* Module Defines
*************************************/
#define MIN_BIU                 API_BIU_1
#define MAX_BIU                 API_BIU_4
#define MAX_API_MODULE  32
#define MIN_API_STREAM          API_STREAM_1
#define MAX_API_STREAM          API_STREAM_8

/* API1553 modules */
#define API_MODULE_1    0
#define API_MODULE_2    1
#define API_MODULE_3    2
#define API_MODULE_4    3
#define API_MODULE_5    4
#define API_MODULE_6    5
#define API_MODULE_7    6
#define API_MODULE_8    7
#define API_MODULE_9    8
#define API_MODULE_10   9
#define API_MODULE_11   10
#define API_MODULE_12   11
#define API_MODULE_13   12
#define API_MODULE_14   13
#define API_MODULE_15   14
#define API_MODULE_16   15
#define API_MODULE_17   16
#define API_MODULE_18   17
#define API_MODULE_19   18
#define API_MODULE_20   19
#define API_MODULE_21   20
#define API_MODULE_22   21
#define API_MODULE_23   22
#define API_MODULE_24   23
#define API_MODULE_25   24
#define API_MODULE_26   25
#define API_MODULE_27   26
#define API_MODULE_28   27
#define API_MODULE_29   28
#define API_MODULE_30   29
#define API_MODULE_31   30
#define API_MODULE_32   31

/* API1553 bius */
#define API_BIU_1    0x01
#define API_BIU_2    0x02
#define API_BIU_3    0x03
#define API_BIU_4    0x04
#define API_BIU_5    0x05
#define API_BIU_6    0x06
#define API_BIU_7    0x07
#define API_BIU_8    0x08

#define API_STREAM_1 1
#define API_STREAM_2 2
#define API_STREAM_3 3
#define API_STREAM_4 4
#define API_STREAM_5 5
#define API_STREAM_6 6
#define API_STREAM_7 7
#define API_STREAM_8 8

#define MAX_API_DLL_INST 32

#define DBG_DISABLED     0x00000000
#define DBG_INT          0x00000001
#define DBG_INIT         0x00000002
#define DBG_OPEN         0x00000004
#define DBG_CLOSE        0x00000008
#define DBG_IO           0x00000010
#define DBG_READREC      0x00000020
#define DBG_WRITEREP     0x00000040
#define DBG_MUTEX        0x00000080
#define DBG_PARAMCHK     0x00000100
#define DBG_SERVER       0x00000400
#define DBG_DQUEUE       0x00000800
#define DBG_TRACE_PBAPRO 0x04000000
#define DBG_TRACE        0x08000000
#define DBG_INFO         0x10000000
#define DBG_ERROR        0x20000000
#define DBG_ERRORMSG     0x40000000
#define DBG_ALL          0xFFFFFFFF
#define DBG_MINIMAL      0x80000000



#define API_HSSA_DEF            26

#define MAX_API_C1760_C02           16

#define MAX_API_BM_MSG_FLT_REC  255/* uncommented due out of range */

/* interrupt event buffer queue size (do not change !!!) */
#define MAX_IR_EVENT_BUF  256

#define MAX_BOARD_NAME_LENGTH 32

/******************************************
***  GENERAL DEFS
******************************************/
#define API_DIS                   0
#define API_ENA                   1
#define API_DIS_ON_ERROR          2

#define API_OFF                   0
#define API_ON                    1

/*  Return Value Defs
* see AiMilErrors.h for error definitiions */
#define API_OK                    0

/*! \def MAX_TG_CMD_SIZE
 * Maximum size of target commands in bytes
 * when using legacy host to target communication
 */
#define MAX_TG_CMD_SIZE  (12*1024)

/* function ApiCmdInit() */
#define API_INIT_MODE_ALL         0
#define API_INIT_MODE_READ        1


/* Obsolete Single Function / Simulator only defines */
#define API_TYPE_SIM_MON          API_DEVICE_MODE_FULL
#define API_TYPE_SIM_ONLY         API_DEVICE_MODE_SIM
#define API_TYPE_MON_ONLY         API_DEVICE_MODE_SF
#define API_TYPE_NOT_PRESENT      API_DEVICE_MODE_NA


#define API_CHN_SINGLE_1553             1
#define API_CHN_SINGLE_3910             1
#define API_CHN_SINGLE_EFEX             1
#define API_CHN_DUAL_1553               2
#define API_CHN_DUAL_3910               2
#define API_CHN_DUAL_EFEX               2
#define API_CHN_SINGLE_3910_SINGLE_1553 2
#define API_CHN_SINGLE_EFEX_SINGLE_1553 2
#define API_CHN_SINGLE_3910_DUAL_1553   3
#define API_CHN_SINGLE_EFEX_DUAL_1553   3
#define API_CHN_QUAD_1553               4
#define API_CHN_SINGLE_3910_QUAD_1553   5
#define API_CHN_SINGLE_EFEX_QUAD_1553   5

#define API_PROTOCOL_1553         1
#define API_PROTOCOL_3910         2
#define API_PROTOCOL_EFEX         3
#define API_PROTOCOL_1553_3910    4
#define API_PROTOCOL_1553_EFEX    5

#define API_LITTLE_ENDIAN_MODE    0
#define API_BIG_ENDIAN_MODE       1

#define API_PBI_SINGLE_1553       1
#define API_PBI_DUAL_1553         2
#define API_PBI_3910              4
#define API_PBI_APX               9
#define API_PBI_DUAL_1553_ONE_BIU 10
#define API_PBI_SINGLE_1553_DBTE  16

#define API_PBI_STANDARD          0
#define API_PBI_PROGRAMMABLE      1
#define API_PBI_ELECTRICAL        0
#define API_PBI_ONBOARD_FOFE      1
#define API_PBI_APX_MILSCOPE      0
#define API_PBI_APX_STANDARD      1


#ifdef _AIM_LVRT
/* function ApiWaitForInterruptsCvi */
#define API_NO_INT_OCCURED            2
#endif

#define API_LLA_INT_TYPE_MASK         0xE0000000
#define API_LLA_UPDATE_MASK           0x10000000
#define API_LLA_STOP_MASK             0x08000000
#define API_LLA_STAT_MASK             0x04000000
#define API_LLA_TRIG_MASK             0x04000000
#define API_LLA_ANY_ERR_MASK          0x02000000
#define API_LLA_BFI_MASK              0x02000000
#define API_LLA_START_MASK            0x01000000

#define API_LLA_PSA_MASK              0x00400000
#define API_LLA_PTRA_MASK             0x00400000
#define API_LLA_RBI_MASK              0x000000FF
#define API_LLA_TCBI_MASK             0x000000FF

#define API_INT_RT                    0
#define API_INT_BC                    1
#define API_INT_BM                    2
#define API_INT_REPLAY                3
#define API_INT_BC_BRANCH             4
#define API_INT_TRG_TT                5

#define API_INT_LS                    1
#define API_INT_HS                    2

#define API_LLC_TRANS_ID              0x00FFFFFF
#define API_LLC_RT_ADDR               0x0000F800
#define API_LLC_TR                    0x00000400
#define API_LLC_RT_SUB                0x000003E0
#define API_LLC_MODECODE              0x0000001F


#define API_DSUB_RS232                0
#define API_DSUB_TRG_IN               1












/* USB device specific memory area definitions */

/*! \def USB_EEPROM
Define for EEProm memory on USB devices */
#define USB_EEPROM 0

/*! \def USB_8051_RAM
Define for 8051 CPU program RAM on USB devices */
#define USB_8051_RAM 1



/* function ApiCmdBCIni() */
#define API_RETRY_1ALT              1
#define API_RETRY_1SAME_1ALT        2
#define API_RETRY_2SAME_1ALT        3

#define API_TBM_TRANSFER            0
#define API_TBM_GLOBAL              1

/* function ApiCmdBCBHDef() */
#define API_QUEUE_SIZE_1            0
#define API_QUEUE_SIZE_2            1
#define API_QUEUE_SIZE_4            2
#define API_QUEUE_SIZE_8            3
#define API_QUEUE_SIZE_16           4
#define API_QUEUE_SIZE_32           5
#define API_QUEUE_SIZE_64           6
#define API_QUEUE_SIZE_128          7
#define API_QUEUE_SIZE_256          8

#define API_BSM_RX_DISCARD          0
#define API_BSM_RX_KEEP_CURRENT     1
#define API_BSM_TX_KEEP_SAME        0
#define API_BSM_TX_GO_ON_NEXT       1

#define API_BQM_CYCLIC              0
#define API_BQM_STAY_LAST           1
#define API_BQM_HOST_CONTROLLED     2

#define API_SQM_ONE_ENTRY_ONLY      1
#define API_SQM_AS_QSIZE            2



#define API_ERR_TYPE_NO_INJECTION     0
#define API_ERR_TYPE_COMMAND_SYNC     1
#define API_ERR_TYPE_DATA_SYNC        2
#define API_ERR_TYPE_PARITY           3
#define API_ERR_TYPE_MANCHESTER_HIGH  4

#define API_ERR_TYPE_MANCHESTER_LOW             5
#define API_ERR_TYPE_GAP                        6
#define API_ERR_TYPE_WORD_CNT_HIGH              7
#define API_ERR_TYPE_WORD_CNT_LOW               8
#define API_ERR_TYPE_BIT_CNT_HIGH               9
#define API_ERR_TYPE_BIT_CNT_LOW                10
#define API_ERR_TYPE_ALTERNATE_BUS              11
#define API_ERR_TYPE_ZERO_CROSS_NEG             12
#define API_ERR_TYPE_ZERO_CROSS_POS             13
#define API_ERR_TYPE_HSSA                       14
#define API_ERR_TYPE_BUS_SWITCH                 15
#define API_ERR_TYPE_PHYSICAL_ERROR_SUPPRESSION 16
#define API_ERR_TYPE_WCLO_EXT                   17
#define API_ERR_TYPE_MSG_LENGTH_HI_IGNORE       18
#define API_ERR_TYPE_MSG_LENGTH_LO_IGNORE       19


#define API_BRW_BUFSTAT_MASK          0x3000
#define API_BRW_RBUS_MASK             0x0010
#define API_BRW_ERR_REPORT_MASK       0x000F

#define API_BUF_NOT_USED              0
#define API_BUF_FULL                  1
#define API_BUF_EMPTY                 2

#define API_RCV_BUS_PRIMARY           1
#define API_RCV_BUS_SECONDARY         0










/***************************************************************************
*  API1553-DS Structures - Typedefs
***************************************************************************/
typedef struct ty_api_lib_info
{
    AiUInt32 ul_AttachedApps;
} TY_API_LIB_INFO;

typedef struct ty_api_intr_regs
{
    AiUInt32 lla;
    AiUInt32 llb;
    AiUInt32 llc;
    AiUInt32 lld;
} TY_API_INTR_REGS[MAX_IR_EVENT_BUF];

typedef union ty_api_intr_loglist_llc_union
{
    AiUInt32 ul_All;
#ifdef HOST_ENDIAN_LITTLE
    struct ty_api_intr_loglist_llc_t
    {
        AiUInt32 ul_Info:24;
        AiUInt32 uc_IntType:8;
    }t;

    struct ty_api_intr_loglist_llc_b
    {
        AiUInt32 ul_Info:24;

        AiUInt32 uc_Biu1:1;
        AiUInt32 uc_Biu2:1;
        AiUInt32 uc_Dma:1;
        AiUInt32 uc_Target:1;
        AiUInt32 uc_Cmd:1;
        AiUInt32 uc_Biu3:1;
        AiUInt32 uc_Biu4:1;
        AiUInt32 res:1;
    }b;
#else
    struct ty_api_intr_loglist_llc_t
    {
        AiUInt32 uc_IntType:8;
        AiUInt32 ul_Info:24;
    }t;

    struct ty_api_intr_loglist_llc_b
    {
        AiUInt32 res:1;
        AiUInt32 uc_Biu4:1;
        AiUInt32 uc_Biu3:1;
        AiUInt32 uc_Cmd:1;
        AiUInt32 uc_Target:1;
        AiUInt32 uc_Dma:1;
        AiUInt32 uc_Biu2:1;
        AiUInt32 uc_Biu1:1;
        AiUInt32 ul_Info:24;
    }b;
#endif
} TY_API_INTR_LOGLIST_LLC;

typedef union ty_api_intr_loglist_lld_union
{
    AiUInt32 ul_All;
#ifdef HOST_ENDIAN_LITTLE
	struct ty_api_intr_loglist_lld_union_t
    {
        AiUInt32 ul_Index:16;
        AiUInt32 uc_Res:8;
        AiUInt32 uc_IntSrc:8;
    }t;
#else
	struct ty_api_intr_loglist_lld_union_t
    {
        AiUInt32 uc_IntSrc:8;
        AiUInt32 uc_Res:8;
        AiUInt32 ul_Index:16;
    }t;
#endif
} TY_API_INTR_LOGLIST_LLD;

typedef struct ty_api_intr_loglist_entry
{
    AiUInt32 ul_Lla;
    AiUInt32 ul_Llb;
    TY_API_INTR_LOGLIST_LLC x_Llc;
    TY_API_INTR_LOGLIST_LLD x_Lld;
} TY_API_INTR_LOGLIST_ENTRY;

typedef struct ty_api_intr_event_list
{
    AiUInt32 put_cnt;
    AiUInt32 get_cnt;
    TY_API_INTR_LOGLIST_ENTRY entry[MAX_IR_EVENT_BUF];
} TY_API_INTR_EVENT_LIST;





/* Typedefinitions special for VME Environment */
typedef void VOID_FUNC_1553(void);
typedef void INTERRUPT_FUNC(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt8 uc_Type, TY_API_INTR_LOGLIST_ENTRY * x_Info );

/* Typedefinitions special for API Environment */
typedef void  (AI_CALL_CONV    *TY_INT_FUNC_PTR)(AiUInt32 ul_Module,
                                                  AiUInt8 uc_Biu,
                                                  AiUInt8 uc_Type,
                                                  TY_API_INTR_LOGLIST_ENTRY * px_Info );

typedef void  (AI_CALL_CONV *TY_PNP_CALLBACK_FUNC_PTR)(AiUInt32 ul_Module, AiUInt32 ul_NotificationType);

#define MAX_API_INT_TYPE         6
#define INT_REQUEST_COUNT_MAX    1000

typedef struct
{
    TY_INT_FUNC_PTR af_IntFunc[MAX_API_INT_TYPE];
    AiUInt32        ul_OpenExUsed;
    AiUInt32        ul_Module;
} TyIntFuncElement;


/*************************************
** Client / Server Support
*************************************/

#define MAX_API_SERVER_PER_SLOT      8
#define MAX_API_SERVER_PER_SYSTEM MAX_API_DLL_INST
#define API_SERVER_MASK              0x000000E0
#define API_MODULE_MASK              0x0000001F
#define API_SERVER_POS               5
#define API_STREAM_MASK              0x00000F00
#define API_STREAM_POS               8

/*! \def IS_REMOTE
This macro checks if a module handle points to a remote device,
by checking if server ID of handle is != 0 */
#define IS_REMOTE(ulModHandle) ((ulModHandle & API_SERVER_MASK) != 0)
#define IS_HS_ACCESS(ulModHandle) ((ulModHandle & API_HS_ACCESS) != 0)

/*! \def GET_SERVER_ID
    Extracts the server ID from a module handle */
#define GET_SERVER_ID( ulModHandle ) ((ulModHandle & API_SERVER_MASK) >> API_SERVER_POS)

/*! \def GET_STREAM_ID
    Extracts the stream ID from a module handle */
#define GET_STREAM_ID( ulModHandle ) ((ulModHandle & API_STREAM_MASK) >> API_STREAM_POS)

/*! \def GET_MODULE_ID
    Extracts the module ID from a module handle */
#define GET_MODULE_ID(ulModHandle) (ulModHandle & API_MODULE_MASK)

/*! \def GET_MODULE_ID
    Extracts the module ID from a module handle */
#define GET_LOCAL_MODULE_HANDLE(ulModHandle) (ulModHandle & ~(API_SERVER_MASK | API_HS_ACCESS))
#define GET_LOCAL_MODULE_HANDLE_KEEP_HS_FLAG(ulModHandle) (ulModHandle & ~(API_SERVER_MASK))

#define CREATE_MODULE_HANDLE( dev, stream, srv )  ((((stream==0xFFFFFFFF)?0:stream) << API_STREAM_POS) | (srv << API_SERVER_POS) | dev)
#define CREATE_MODULE_HANDLE_FROM_LOCAL( local_handle, srv )  ((srv << API_SERVER_POS) | local_handle)


#define API_SERVER_REGISTERED        0
#define API_SERVER_NOT_REGISTERED    1
#define API_INVALID_SLOT_NUMBER      2

#define API_NET_IO_ERR  API_ERR

#define API_SRV_OS_VER_WIN98          2
#define API_SRV_OS_VER_WINNT          3
#define API_SRV_OS_VER_WIN2000        4

/*************************************
** Boundary Constants 
*************************************/
#define MAX_RT_ADDR                 31 
#define MAX_QUEUE_SIZE              API_QUEUE_SIZE_256
#define MAX_WCNT                    31
#define MAX_ERR_WPOS                32
#define MAX_GAP_TIME_MODE_01        0x7FFF
#define MAX_GAP_TIME_MODE_2         63
#define MAX_DYTAG_MODE              API_DYTAG_EFA_MODE
#define MAX_DYTAG_FCT               API_DYTAG_FCT_SAWTOOTH_8_HIGH
#define MAX_ACYC_MODE               API_BC_ACYC_SEND_AT_END_OF_FRAME

#define MAX_WPOS                    31
#define MAX_BSM                     API_BSM_RX_KEEP_CURRENT
#define MAX_BM_IMODE                API_BM_MODE_STOP_EVENT_INT
#define MAX_BM_SMODE                API_BM_STROBE_ON_STOP_EVENT
#define MAX_BM_CAPMODE              API_BM_CAPMODE_FILTER
#define MAX_BM_ACT_ENTRY            4095
#define MAX_BM_STACK_ENTRY_READMODE API_BM_READ_ABS
#define MAX_BM_STACK_ENTRY_SIGN     API_BM_SIGN_NEG
#define MAX_BM_STACK_ENTRY_OFFSET   0x40000 

#define MAX_BM_ENTRY_TYPE           API_BM_ENTRY_TIMETAG_HIGH
#define MAX_BM_FTW_CON              API_BM_WRITE_HTM
#define MAX_BM_TIW_CON              API_BM_WRITE_ETI
#define MAX_RT_SA_TYPE              API_RT_TYPE_TRANSMIT_MODECODE
#define MAX_RT_SA                   30
#define MAX_RT_MODECODE             31
#define MAX_RT_RESPONSE             API_RT_RSP_SEC_BUS
#define MAX_TCB_INDEX               255 
#define MAX_TCB_TT                  API_BM_TRG_HS_EVENT
#define MAX_TCB_SOT                 API_BM_PULSE_STROBE_ON_TRG
#define MAX_TCB_TRI                 API_BM_ASSERT_TRG_INT
#define MAX_TCB_INV                 API_BM_INVERT_RESULT
#define MAX_TCB_NEXT                0xFF
#define MAX_TCB_EOM                 0xFF
#define MAX_BM_MSGFLT_MODE          API_BM_FLT_MODE_DEPENDENT
#define MAX_BM_MSGFLT_FORMAT        API_BM_MSG_FLT_FORMAT_5
#define MAX_DATASET_BUF_ID          4095
#define MAX_MSG_BUF                 API_BUF_RT_MSG
#define MAX_DATA_BITPOS             15
#define MAX_BUS                     API_CAL_BUS_SECONDARY
#define MAX_CPL_MODE                API_CAL_CPL_WRAP_AROUND_LOOP
#define MAX_TRANS_MODE              API_CAL_TRANS_500K
#define MAX_BQM                     API_BQM_HOST_CONTROLLED
#define MAX_INIT_MODE               API_INIT_MODE_READ
#define MAX_RESET_CONTROL           API_RESET_USE_COUNTER_MODE_2
#define MAX_BITE_CONTROL            API_BITE_TIMING
#define MAX_TIMER                   API_TIMER_4
#define MAX_TIMER_MODE              API_STOP_TIMER
#define MAX_SREC_MODE               API_SREC_START_ONCE
#define MAX_EXECSYS_MODE            API_MODE_BMINI_CLEAR_BUFFER

#define MAX_PROTOCOL                API_PROTOCOL_EFEX
#define MAX_PROTOCOL_MODE           API_MODE_SINGLE_RT
#define MAX_SYSTAG_MODE             API_RT_MODE
#define MAX_SYSTAG_CON              API_SYSTAG_RESUME
#define MAX_SYSTAG_FCT              API_SYSTAG_FCT_CHECKSUM
#define MAX_SYSTAG_STEP_FCT5        API_SYSTAG_STEP_KEEP_LAST
#define MAX_SYSTAG_STEP_FCT6        API_SYSTAG_STEP_KEEP_LAST
#define MAX_SYSTAG_WPOS             31

#define MAX_SW_MASK_CONTROL         API_RT_SWM_NXW_AND
#define MAX_SA_CON                  API_RT_ENABLE_SA_INT_ERR
#define MAX_ERR_TYPE                API_ERR_TYPE_BUS_SWITCH
#define MAX_RETRY                   API_RETRY_2SAME_1ALT
#define MAX_TBM                     API_TBM_GLOBAL
#define MAX_CLR                     (API_RESET_STATUS_BITS | API_RESET_ERROR_BITS | API_RESET_SRVREQ_CNT)
#define API_REP_CET                 1
#define MAX_TRACK_ID                255
#define MAX_TRACK_READ_MODE         API_TRACK_CLR_UDF
#define MAX_TRACK_CHUNKSIZE         256
#define MAX_TRACK_PREALLOC_STATES   256



#define MAX_API_SQM_MODE  API_SQM_AS_QSIZE
#define MAX_PXICON_MODE             API_PXI_SET_TTSRC_INTERNAL
#define MAX_PXICON_TRGSOURCE        API_TRG_BM_HS
#define MAX_PXICON_TRGDEST          API_TRG_BM_HS

#define MIN_HEADER_ID           1
#define MAX_HEADER_ID           8191

#define MIN_BUFFER_ID           0
#define MAX_BUFFER_ID           65535

#define MIN_XFER_ID             1
#define MAX_XFER_ID             8191

#define MIN_ERR_BPOS            4
#define MAX_ERR_BPOS            20

#define MIN_ERR_BCBITS          1
#define MAX_ERR_BCBITS          3

#define MIN_INSTR_NR            1
#define MAX_INSTR_NR            API_BC_INSTR_STROBE

#define MIN_SKIP_CNT            1
#define MAX_SKIP_CNT            127


#define MIN_BC_START_MODE       1
#define MAX_BC_START_MODE       API_BC_START_INSTR_TABLE_DBA

#define MIN_BC_FRAME_TIME       0.001
#define MAX_BC_FRAME_TIME       1048.576

#define MIN_DATA_WORDPOS        1
#define MAX_DATA_WORDPOS        32

#define MIN_DATA_BITLEN         1
#define MAX_DATA_BITLEN         16

#define MIN_DATA_WORDS          1
#define MAX_DATA_WORDS          32

#define MIN_FIFO_ID             1
#define MAX_FIFO_ID             32


#define MIN_BUF_NB_IN_FIFO      2
#define MAX_BUF_NB_IN_FIFO      128

#define MIN_RESP_TIME           4.0
#define MAX_RESP_TIME           63.75

#define MIN_SYSTAG_ID           1
#define MAX_SYSTAG_ID           255


#define MIN_CONTIG_GAPERROR         1
#define MAX_CONTIG_GAPERROR         15

#define MIN_CONTIG_ZEROERROR        0
#define MAX_CONTIG_ZEROERROR        1

#define MIN_SYSTAG_STEP_FCT7        API_SYSTAG_STEP_1S_COMP
#define MAX_SYSTAG_STEP_FCT7        API_SYSTAG_STEP_2S_COMP


#define MIN_TRACK_LEN           1
#define MAX_TRACK_LEN           32

#define MIN_MULTI_TRACK_NB      2
#define MAX_MULTI_TRACK_NB      65536

#define MIN_CONT_TRACK_NB       1
#define MAX_CONT_TRACK_NB       32

#define MIN_BH_BUF_SIZE          1
#define MAX_BH_BUF_SIZE         31

#define MIN_DYTAG_MON_ID  1
#define MAX_DYTAG_MON_ID  64

#define MIN_BC_MODE_CTRL  API_BC_MODE_INSERT_MC17_SYNC_CNT
#define MAX_BC_MODE_CTRL  API_BC_MODE_CONFIGURED_DYTAGS

#define MIN_RT_MODE_CTRL  API_RT_MODE_CONFIGURED_DYTAGS
#define MAX_RT_MODE_CTRL  API_RT_MODE_OPERATION_CTRL
#define ALL_RT_ADDR       0xFFFFFFFF

#define MAX_RT_DEFINED_WORD_CNT 32

#ifdef _CMPL4HATS
#define MAX_API_HATS_DIN_CHN      16 
#define MAX_API_HATS_DOUT_CHN      8 
#define MAX_API_HATS_DAC_CHN      16 
#define MAX_API_HATS_ADC_CHN      32
#define MAX_API_HATS_LVDT_OUT_CHN  8
#define MAX_API_HATS_LVDT_IN_CHN   8
#endif



#endif


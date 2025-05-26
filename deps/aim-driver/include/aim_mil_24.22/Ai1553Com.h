/* SPDX-FileCopyrightText: 2015-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai1553Com.h
 *
 *  Shared 1553 struct definitions for API, Library, Driver and Target Software
 *
 */

#ifndef __AI1553COM_H__
#define __AI1553COM_H__

#include "Ai_cdef.h"
#include "Ai_def.h"

typedef TY_DRIVER_INFO TY_API_DRIVER_INFO;

/* -- generic defines used in host and target -- */
#define API_MAX_LOGICAL_BIU  8
#define API_MAX_PHYSICAL_BIU 4
#define API_MAX_MEMORY_BARS  2


enum API_DEVICE_MODE
{
    API_DEVICE_MODE_FULL     = 0,
    API_DEVICE_MODE_SIM      = 1,
    API_DEVICE_MODE_SF       = 2,
    API_DEVICE_MODE_EMBEDDED = 4,
    API_DEVICE_MODE_NOREPLAY_NOERROR = 8,
    API_DEVICE_MODE_DISABLE_BC = 16,
    API_DEVICE_MODE_NA       = 0xFF,
};


#define API_DEVICE_MODE_IS_SIMULATOR(m)        (((m) & API_DEVICE_MODE_SIM) == API_DEVICE_MODE_SIM)
#define API_DEVICE_MODE_IS_SINGLEFUNCTION(m)   (((m) & API_DEVICE_MODE_SF) == API_DEVICE_MODE_SF)
#define API_DEVICE_MODE_IS_EMBEDDED(m)         (((m) & API_DEVICE_MODE_EMBEDDED) == API_DEVICE_MODE_EMBEDDED)
#define API_DEVICE_MODE_IS_NOREPLAY_NOERROR(m) (((m) & API_DEVICE_MODE_NOREPLAY_NOERROR) == API_DEVICE_MODE_NOREPLAY_NOERROR)
#define API_DEVICE_MODE_IS_DISABLE_BC(m)       (((m) & API_DEVICE_MODE_DISABLE_BC) == API_DEVICE_MODE_DISABLE_BC)



/* -- ApiGetBoardInfo -- */

#define TY_BOARD_INFO_DEVICE_TYPE      0
#define TY_BOARD_INFO_CHANNEL_COUNT    1
#define TY_BOARD_INFO_BIU_COUNT        2
#define TY_BOARD_INFO_BOARD_TYPE       3
#define TY_BOARD_INFO_BOARD_CONFIG     4
#define TY_BOARD_INFO_SERIAL           5
#define TY_BOARD_INFO_PARTNO           6
#define TY_BOARD_INFO_SIZE_GLOBAL      7
#define TY_BOARD_INFO_SIZE_SHARED      8
#define TY_BOARD_INFO_OFFS_GLOBAL      9
#define TY_BOARD_INFO_CHANGE_AMPL      10
#define TY_BOARD_INFO_COUPLING         11
#define TY_BOARD_INFO_IRIG             12
#define TY_BOARD_INFO_DISCRETE_CNT     13
#define TY_BOARD_INFO_DISCRETE_CONFIG  14
#define TY_BOARD_INFO_IR               15
#define TY_BOARD_INFO_IS_MULTI_CHANNEL 16
#define TY_BOARD_INFO_IS_HS_REDUNDANT  17
#define TY_BOARD_INFO_CAN_HIGH_RES_ZERO_CROSSING 18
#define TY_BOARD_INFO_HAS_ELECTRICAL_INTERFACE   19
#define TY_BOARD_INFO_MILSCOPE_TYPE              20
#define TY_BOARD_INFO_IS_DBTE                    21
#define TY_BOARD_INFO_HAS_EXTERNAL_IRIG          22
#define TY_BOARD_INFO_HAS_EXTERNAL_TRIGGER       23
#define TY_BOARD_INFO_PROTOCOL                   24
#define TY_BOARD_INFO_APPLICATION_TYPE           25
#define TY_BOARD_INFO_CHANGE_AMPL_HIGH_RES       26
#define TY_BOARD_INFO_CHANGE_AMPL_PACKED         27
#define TY_BOARD_INFO_CAN_OVERRIDE_TRAFFIC       28
#define TY_BOARD_INFO_HAS_FAST_GAP               29
#define TY_BOARD_INFO_HAS_LOW_PRIORITY_LIST      30
#define TY_BOARD_INFO_HAS_BC_INSTR_RETURN_ON_XFER_ERROR 31
#define TY_BOARD_INFO_USB_AIM_POWER_CLASS        32
#define TY_BOARD_INFO_HAS_DIGITAL_TRIGGER_LOOP   33

#define TY_BOARD_INFO_MAX              34

#define API_MILSCOPE_TYPE_NONE    0
#define API_MILSCOPE_TYPE_AYX     1
#define API_MILSCOPE_TYPE_AYE     2


#define TY_AIM_POWER_CLASS_FULL 0
#define TY_AIM_POWER_CLASS_MED  1
#define TY_AIM_POWER_CLASS_LOW  2
static const char * const __ty_aim_power_class_strings[] = {"100% bus load each channel", "max. 50% bus load each channel", "max. 10% bus load each channel"};
#define TY_AIM_POWER_CLASS_STRING(x) __ty_aim_power_class_strings[x]

/* ApiCmdBCXferDescGet / ApiCmdBCXferDescMod */

#define XFER_DESC_FIELD_CHN      1
#define XFER_DESC_FIELD_CW1      2
#define XFER_DESC_FIELD_CW2      3
#define XFER_DESC_FIELD_TRTYPE   4
#define XFER_DESC_FIELD_CW1_CW2  5



/*****************************************************************************/
/*                                                                           */
/*                     Host to Target communication                          */
/*                      input  / ouput definitions                           */
/*                                                                           */
/*****************************************************************************/

typedef struct ty_mil_com
{
    AiUInt32   ulMagic;
    AiUInt32   ulStream;
    AiUInt32   ulCommand;
    AiUInt32   ulSize;
    AiUInt32   ulExpectedAckSize;
    AiUInt32   ulSwapControl; /* 31..16=words; 15..0=bytes (data is lwords,words,bytes) */

    AiUInt32   ulReserved_3;
    AiUInt32   ulReserved_4;
} TY_MIL_COM; /* communication */


typedef struct ty_mil_com_ack
{
   /* This value is set when the command is received  */
    AiUInt32   ulMagic;
    /* This value is set when the command is received  */
    AiUInt32   ulCommand;
    /* This value indicates an error during the execution if not equal to zero */
    AiUInt32   ulError;
    AiUInt32   ulSize;
    AiUInt32   ulSwapControl; /* 31..16=words; 15..0=bytes (data is lwords,words,bytes) */

    /* Reserved */
    AiUInt32   ulReserved_2;
    AiUInt32   ulReserved_3;
    AiUInt32   ulReserved_4;
} TY_MIL_COM_ACK; /* acknowlege */


typedef struct ty_mil_com_with_value
{
    TY_MIL_COM cmd;
    AiUInt32   value;
} TY_MIL_COM_WITH_VALUE;


typedef struct ty_mil_com_ack_with_value
{
    TY_MIL_COM_ACK  xAck;
    AiUInt32        value;
} TY_MIL_COM_ACK_WITH_VALUE;


#define MIL_COM_INIT( p_Struct, _ulStream, _ucBiu, _ulCommand, _ulInSize, _ulOutSize ) \
{   \
    (p_Struct)->ulMagic           = MIL_COM_MAGIC;  \
    if( _ulStream != 0 )                            \
        (p_Struct)->ulStream      = _ulStream;      \
    else                                            \
        (p_Struct)->ulStream      = _ucBiu;         \
    (p_Struct)->ulCommand         = _ulCommand;     \
    (p_Struct)->ulSize            = _ulInSize;      \
    (p_Struct)->ulExpectedAckSize = _ulOutSize;     \
    (p_Struct)->ulSwapControl     = 0;              \
    (p_Struct)->ulReserved_3      = 0;              \
    (p_Struct)->ulReserved_4      = 0;              \
}



#ifdef HOST_ENDIAN_BIG
typedef union ty_coupling_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_coupling_capabilities_b
    {
        AiUInt32 b_Isolated    : 1;
        AiUInt32 b_Direct      : 1;
        AiUInt32 b_Transformer : 1;
        AiUInt32 b_Network     : 1;
        AiUInt32 b_Packed      : 1;
        AiUInt32 b_DigitalWrap : 1;
        AiUInt32 ul_Res        : 26;
    } b;
} TY_COUPLING_CAPABILITIES;
#else
typedef union ty_coupling_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_coupling_capabilities_b
    {
        AiUInt32 ul_Res        : 26;
        AiUInt32 b_DigitalWrap : 1;
        AiUInt32 b_Packed      : 1;
        AiUInt32 b_Network     : 1;
        AiUInt32 b_Transformer : 1;
        AiUInt32 b_Direct      : 1;
        AiUInt32 b_Isolated    : 1;
    } b;
} TY_COUPLING_CAPABILITIES;
#endif

#ifdef HOST_ENDIAN_BIG
typedef union ty_irig_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_irig_capabilities_b
    {
        AiUInt32 b_IrigSwitch : 1;
        AiUInt32 b_FreeWheeling : 1;
        AiUInt32 b_Sinusoidal : 1;
        AiUInt32 b_InstantIrigSetOnInternalMode : 1;
        AiUInt32 b_NoExternalSignalOnInternalMode : 1;
        AiUInt32 ul_Res : 27;
    } b;
} TY_IRIG_CAPABILITIES;
#else
typedef union ty_irig_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_irig_capabilities_b
    {
        AiUInt32 ul_Res : 27;
        AiUInt32 b_NoExternalSignalOnInternalMode : 1;
        AiUInt32 b_InstantIrigSetOnInternalMode : 1;
        AiUInt32 b_Sinusoidal : 1;
        AiUInt32 b_FreeWheeling : 1;
        AiUInt32 b_IrigSwitch : 1;
    } b;
} TY_IRIG_CAPABILITIES;
#endif


#ifdef HOST_ENDIAN_BIG
typedef union ty_ir_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_ir_capabilities_b
    {
        AiUInt32 b_IrOnDiscretes : 1;
        AiUInt32 ul_Res : 31;
    } b;
} TY_IR_CAPABILITIES;
#else
typedef union ty_ir_capabilities_union
{
    AiUInt32 ul_All;
    struct ty_ir_capabilities_b
    {
        AiUInt32 ul_Res : 31;
        AiUInt32 b_IrOnDiscretes : 1;
    } b;
} TY_IR_CAPABILITIES;
#endif


/*! Parameters for API function ApiCmdScopeSetup.
Contains all settings availabe for digital scope functionality */
typedef struct ty_api_scope_setup
{
    AiUInt32 ul_CouplingPri;     /*!< coupling mode of primary channel. See \ref TY_API_SCOPE_COUPLING for possible values */
    AiUInt32 ul_CouplingSec;     /*!< coupling mode of secondary channel See \ref TY_API_SCOPE_COUPLING for possible values */
    AiUInt32 ul_SamplingRate;    /*!< sampling rate of scope (every n+1-th sample will be stored according to this value */
    AiUInt32 ul_CaptureMode;     /*!< capture mode (single-shot or continuous recording) */
    AiUInt32 ul_OperationMode;   /*!< operation mode of scope. See \ref TY_API_SCOPE_OPERATION_MODE for possible values */
    AiUInt32 ul_SingleShotBuf;   /*!< reserved for I don't know what */
} TY_API_SCOPE_SETUP;


/*! Parameters for API function ApiCmdScopeTriggerDef.
Contains all settings availabe for digital scope trigger functionality */
typedef struct ty_api_scope_trg
{
    AiUInt32 ul_TrgMode;         /*!< trigger mode on which condition the trigger is activated */
    AiUInt32 ul_TrgSrc;          /*!< trigger source: primary or secondary bus on which the trigger is activated */
    AiUInt32 ul_TrgValue;        /*!< trigger value that is used to compare against */
    AiUInt32 ul_TrgNbSamples;    /*!< number of samples */
    AiUInt32 ul_TrgFlags;        /*!< trigger flags */
    AiUInt32 ul_TrgDelay;        /*!< trigger delay time */
    AiUInt32 ul_TrgTbt;          /*!< trace before trigger */
} TY_API_SCOPE_TRG;


/*! \enum API_SCOPE_BUFFER_TYPE
enum that signifies for which channel
a MilScope buffer is used for */
typedef enum API_SCOPE_BUFFER_TYPE
{
    SCOPE_BUFFER_PRIMARY = 0,  /*!< Buffer is used for collecting data on primary channel */
    SCOPE_BUFFER_SECONDARY     /*!< Buffer is used for collecting data on secondary channel */
}TY_API_SCOPE_BUFFER_TYPE;


/*! enum that holds all possible scope states */
typedef enum API_SCOPE_STATUS
{
    SCOPE_STATUS_WAIT_FOR_TRIGGER = 0,     /*!< scope is running and waiting for trigger */
    SCOPE_STATUS_TRIGGERED,                /*!< scope is running and has triggered */
    SCOPE_STATUS_STOPPED,                  /*!< scope is stopped */
    SCOPE_STATUS_OVERFLOW                  /*!< a overflow of scope data occured */
}TY_API_SCOPE_STATUS;


/*! Parameters for API function ApiCmdScopeTriggerDefEx.
Contains all settings availabe for digital scope trigger functionality on AYE cards */
typedef struct api_scope_trg_ex
{
    AiUInt32 ul_TrgMode;         /*!< trigger mode on which condition the trigger is activated */
    AiUInt32 ul_TrgSrc;          /*!< trigger source: primary or secondary bus on which the trigger is activated */
    AiUInt32 ul_TrgValue;        /*!< trigger value that is used to compare against as upper boundary */
    AiUInt32 ul_TrgValue2;       /*!< trigger value that is compared against as lesser boundary */
    AiUInt32 ul_TrgNbSamples;    /*!< number of samples */
    AiUInt32 ul_TrgNbSamples2;   /*!< Used for specifying gap length when triggering on command word */
    AiUInt32 ul_TrgFlags;        /*!< trigger flags */
    AiUInt32 ul_TrgDelay;        /*!< trigger delay time */
    AiUInt32 ul_TrgTbt;          /*!< trace before trigger */
    AiUInt32 ul_TrgDiscrete;     /*!< input discrete channel that will be triggered on */
} TY_API_SCOPE_TRG_EX;


/*! enumeration for all possible trigger strobe
types that are supported */
typedef enum api_scope_trigger_strobe_type
{
    API_SCOPE_TRG_STROBE_MON      = 0,   /*!< on trigger, send strobe to monitor */
    API_SCOPE_TRG_STROBE_BC       = 1,   /*!< on trigger, send strobe to bus controller */
    API_SCOPE_TRG_STROBE_DISCRETE = 2    /*!< on trigger, send strobe to discrete channel */

}TY_API_SCOPE_TRIGGER_STROBE_TYPE;


/*! Structure that represents the scope trigger strobe

This structure contains type and destination of strobe
*/
typedef struct api_scope_trg_strobe
{
    AiUInt32 ul_TrgStrobeType;           /*!< Destination type the Trigger Strobe will be issued to */
    AiUInt32 ul_TrgStrobeDest;           /*!< value used to specify destination of the strobe */
} TY_API_SCOPE_TRG_STROBE;


/*! This structure contains the values used
for scope offset compensation */
typedef struct api_scope_offsets
{
    AiInt32 lOffsetA_3V_stub;    /*!< Offset for channel A in 3V stub mode */
    AiInt32 lOffsetB_3V_stub;    /*!< Offset for channel B in 3V stub mode */
    AiInt32 lOffsetA_30V_stub;   /*!< Offset for channel A in 30V stub mode */
    AiInt32 lOffsetB_30V_stub;   /*!< Offset for channel B in 30V stub mode */
    AiInt32 lOffsetA_3V_ext;     /*!< Offset for channel A in 3V extern mode */
    AiInt32 lOffsetB_3V_ext;     /*!< Offset for channel B in 3V extern mode */
    AiInt32 lOffsetA_30V_ext;    /*!< Offset for channel A in 30V extern mode */
    AiInt32 lOffsetB_30V_ext;    /*!< Offset for channel B in 30V extern mode */
} TY_API_SCOPE_OFFSETS;





/* function ApiCmdScopeTriggerDef() */
#define API_SCOPE_MODE_GREATER_THAN         0
#define API_SCOPE_MODE_LESS_THAN            1
#define API_SCOPE_MODE_GREATER_OR_LESS_THAN 2
#define API_SCOPE_MODE_GREATER_THAN_SAMPLES 4
#define API_SCOPE_MODE_LESS_THAN_SAMPLES    5
#define API_SCOPE_MODE_BIU                  7

/* Additional trigger settings for function ApiCmdScopeTriggerDefEx() */

/*! \def API_SCOPE_MODE_COMMAND_SYNC
Trigger on command word sync */
#define API_SCOPE_MODE_COMMAND_SYNC         3

/*! \def API_SCOPE_MODE_BIU_BM
Trigger on monitor trigger of first stream */
#define API_SCOPE_MODE_BIU_BM               7

/*! \def API_SCOPE_MODE_BIU_BC
Trigger on BC strobe instruction of first stream */
#define API_SCOPE_MODE_BIU_BC               8

/*! \def API_SCOPE_MODE_BIU2_BM
Trigger on monitor trigger of second stream */
#define API_SCOPE_MODE_BIU2_BM              9

/*! \def API_SCOPE_MODE_BIU2_BC
Trigger on BC strobe instruction of second stream */
#define API_SCOPE_MODE_BIU2_BC              10

/*! \def API_SCOPE_MODE_DISCRETES
Trigger on discrete input channel */
#define API_SCOPE_MODE_DISCRETES            11


#define API_SCOPE_SRC_PRIMARY               0
#define API_SCOPE_SRC_SECONDARY             1








/* -- ApiCmdBCXferDefErr -- */



typedef struct ty_api_bc_error
{
    AiUInt32 ul_Type;
    AiUInt32 ul_Sync;
    AiUInt32 ul_Contig;
    AiUInt32 ul_Padding1;
    AiUInt32 ul_ErrSpec;
} TY_API_BC_ERROR;


/* Api3910CmdHsBCXferDefErr */
typedef struct ty_api_hs_bc_error
{
    AiUInt32 ul_Type;
    AiUInt32 ul_Padding1;
    AiUInt32 ul_Padding2;
    AiUInt32 ul_ErrSpec;
} TY_API_HS_BC_ERROR;


/* -- ApiCmdSysGetMemLocation -- */
#define TY_GETMEM_RT_DESCR              1





/* -- MIL_COM_GEN_IO_TEST_SEQ -- */
#define MAX_GENIO_TEST_SEQ_ARRAY_SIZE 32

typedef struct ty_api_gen_io_test_seq_array
{
    /* start mode */
    AiUInt32 mode;

    /* start condition */
    AiUInt32 cond;

    /* Reserved */
    AiUInt32 res1;
    AiUInt32 res2;

    /* Timeline 0 .. 10.000 in steps of ms */
    AiUInt32 tm_ms[MAX_GENIO_TEST_SEQ_ARRAY_SIZE];

    /* Identify IP slot number and Board Type, define required operation */
    AiUInt32 genio_type_op[MAX_GENIO_TEST_SEQ_ARRAY_SIZE];

    /* Define the toggle Bits (set to 1) */
    AiUInt32 chn_val[MAX_GENIO_TEST_SEQ_ARRAY_SIZE];

} TY_API_GENIO_TEST_SEQ_ARRAY;


typedef struct ty_api_gen_io_test_seq_tbl
{
    TY_API_GENIO_TEST_SEQ_ARRAY ax_Seq[4];
} TY_API_GENIO_TEST_SEQ_TBL;


/*! \struct TY_API_VER_INFO_VALUES
    Protocol independent version structure. \n
    Describes the version of one specific component. \n
    Used in protocol dependent ReadSWVersion API calls. */
typedef struct ty_api_ver_info_values{
    AiUInt32 ul_VersionType;                           /*!< see \ref TY_E_VERSION_ID. ID of the component the version is for */
    AiUInt32 ul_MajorVer;                              /*!< Major version. Must be increased when component interface gets incompatible (e.g. API call signature change) */
    AiUInt32 ul_MinorVer;                              /*!< Minor version. Must be increased when component interface offers new functionality (e.g. new API call) */
    AiUInt32 ul_PatchVersion;                          /*!< Patch version. Must be increased when componnet interface is stable, but e.g. bugs were fixed */
    AiUInt32 ul_BuildNr;                               /*!< Unique number that describes a specific state of the component */
} TY_API_VER_INFO_VALUES;

/*! \struct TY_API_VER_INFO_STRINGS
    Protocol independent version structure. \n
    Describes the version of one specific component. \n
    Used in protocol dependent ReadSWVersion API calls. */
typedef struct ty_api_ver_info_strings{
    AiChar   ac_Description[AI_DESCRIPTION_STRINGL];   /*!< zero-terminated ASCII string that describes the component the version is for */
    AiChar   ac_FullVersion[AI_VERSION_STRINGL];       /*!< zero-terminated ASCII version string. Should hold main version in dot-separated major.minor.patch.build format.\n
                                                            Version extension should be hyphen separated from main version. For example,Git based projects could add commit SHA1 (e.g. -g324FD) */
} TY_API_VER_INFO_STRINGS;






typedef struct ty_api_mem_biu_addr
{
    AiUInt32 ul_Cb;
    AiUInt32 ul_IrLog;
    AiUInt32 ul_RtDesc;
    AiUInt32 ul_BmTcb;
    AiUInt32 ul_BmAct;
    AiUInt32 ul_RtSaDesc;
    AiUInt32 ul_RtBhArea;
    AiUInt32 ul_RtSqArea;
    AiUInt32 ul_RtEqArea;
    AiUInt32 ul_BcBhArea;
    AiUInt32 ul_BcSqArea;
    AiUInt32 ul_BcEqArea;
    AiUInt32 ul_BcXferDesc;
    AiUInt32 ul_BcHipInstr;
    AiUInt32 ul_BcLipInstr;
    AiUInt32 ul_BcAcycInstr;
    AiUInt32 ul_RepBuf;
    AiUInt32 ul_BmBuf;
} TY_API_MEM_BIU_ADDR;

typedef struct ty_api_mem_sim_buf
{
    AiUInt32 ul_BufBaseAddr;
    AiUInt32 ul_BufSize;
    AiUInt32 ul_BufCount;
    AiUInt32 ul_HsBufBaseAddr;
    AiUInt32 ul_HsBufSize;
    AiUInt32 ul_HsRes;    
} TY_API_MEM_SIM_BUF;

typedef struct ty_api_mem_biu_size
{
    AiUInt32 ul_BcHipInstr;
    AiUInt32 ul_BcLipInstr;
    AiUInt32 ul_BcAcycInstr;
    AiUInt32 ul_RepBuf;
    AiUInt32 ul_BmBuf;
} TY_API_MEM_BIU_SIZE;

typedef struct ty_api_mem_biu_count
{
    AiUInt32 ul_RtBhArea;
    AiUInt32 ul_RtSqArea;
    AiUInt32 ul_RtEqArea;
    AiUInt32 ul_BcBhArea;
    AiUInt32 ul_BcSqArea;
    AiUInt32 ul_BcEqArea;
    AiUInt32 ul_BcXferDesc;
} TY_API_MEM_BIU_COUNT;

typedef struct ty_api_mem_biu_info
{
    AiUInt32 ul_Protocol;
    AiUInt32 ul_StreamNb;
    AiUInt32 ul_MemoryBank;
} TY_API_MEM_BIU_INFO;

typedef struct ty_api_get_mem_layout
{
    TY_API_MEM_BIU_ADDR  ax_BiuAddr[API_MAX_LOGICAL_BIU];
    TY_API_MEM_SIM_BUF   x_Sim[API_MAX_MEMORY_BARS];
    TY_API_MEM_BIU_SIZE  ax_BiuSize[API_MAX_LOGICAL_BIU];
    TY_API_MEM_BIU_COUNT ax_BiuCnt[API_MAX_LOGICAL_BIU];
    TY_API_MEM_BIU_INFO  x_BiuInfo[API_MAX_LOGICAL_BIU];
    AiUInt32             aul_GlobalMemSize[API_MAX_MEMORY_BARS];
} TY_API_GET_MEM_INFO;

typedef struct ty_api_set_mem_layout
{
    AiUInt32             aul_SimBufSize[API_MAX_MEMORY_BARS];
    TY_API_MEM_BIU_SIZE  ax_BiuSize[API_MAX_LOGICAL_BIU];
    TY_API_MEM_BIU_COUNT ax_BiuCnt[API_MAX_LOGICAL_BIU];
} TY_API_SET_MEM_INFO;


#define API_BC_MODIFY_TYPE_MODIFY  0
#define API_BC_MODIFY_TYPE_MOVE    1
#define API_BC_MODIFY_TYPE_JUMP    2

#define API_BC_MODIFY_COMPC__EQ        0
#define API_BC_MODIFY_COMPC__LT        1
#define API_BC_MODIFY_COMPC__GT        2
#define API_BC_MODIFY_COMPC__LE        3
#define API_BC_MODIFY_COMPC__GE        4
#define API_BC_MODIFY_COMPC__TRIGGER   5

#define API_BC_MODIFY_WRTYP_16BIT   0
#define API_BC_MODIFY_WRTYP_26BIT   1
#define API_BC_MODIFY_WRTYP_32BIT   2

#define API_BC_MODIFY_OPC_NONE        0
#define API_BC_MODIFY_OPC_ADD_OPERAND 1
#define API_BC_MODIFY_OPC_SUB_OPERAND 2

typedef struct ty_api_bc_modify_instruction
{
    AiUInt32 type;
    AiUInt32 compare_control;              /* compc */
    AiUInt32 write_type;                   /* wrtyp */
    AiUInt32 arithmetic_operation;         /* OPC   */
    AiUInt32 re_load;                      /* RL 0=disabled / 1=enabled   */
    AiUInt32 operand;                      /* OPR   */
    AiUInt32 compare_word_source_offset;
    AiUInt32 destination_offset;
    AiUInt32 mask_word;
    AiUInt32 compare_word;
    AiUInt32 destination_argument;
} TY_API_BC_MODIFY_INSTRUCTION;



#endif /* __AI1553COM_H__ */


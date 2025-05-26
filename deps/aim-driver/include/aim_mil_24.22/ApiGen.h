/* SPDX-FileCopyrightText: 2016-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiGen.h
 *
 *  This header file contains declarations for
 *  replay related API functionality
 *
 */

#ifndef AI1553_GEN_H
#define AI1553_GEN_H 



/************/
/* defines  */
/************/


/* function ApiGetBoardInfo() */
#define TYPE_APX1553_1            22
#define TYPE_APX1553_2            23
#define TYPE_APX1553_4            24
#define TYPE_APX3910              25
#define TYPE_APX3910Xp            26

#define TYPE_ACX1553_1            27
#define TYPE_ACX1553_2            28
#define TYPE_ACX1553_2_TWO_PBIS   29
#define TYPE_ACX1553_4            30
#define TYPE_ACX1553_4_TWO_PBIS   31
#define TYPE_ACX1553_8            32
#define TYPE_ACX3910              33
#define TYPE_ACX3910Xp            34
#define TYPE_ACX3910_2            35
#define TYPE_ACX3910Xp_2          36

#define TYPE_AVX1553_1            37
#define TYPE_AVX1553_2            38
#define TYPE_AVX1553_2_TWO_PBIS   39
#define TYPE_AVX1553_4            40
#define TYPE_AVX1553_4_TWO_PBIS   41
#define TYPE_AVX1553_8            42
#define TYPE_AVX3910              43
#define TYPE_AVX3910Xp            44
#define TYPE_AVX3910_2            45
#define TYPE_AVX3910Xp_2          46

#define TYPE_ACX1553_4_DS_TWO_PBIS     47
#define TYPE_ACX_EFA_1_TWO_PBIS        48
#define TYPE_ACX_EFAXp_1_TWO_PBIS      49
#define TYPE_ACX_EFA_1_DS_TWO_PBIS     50
#define TYPE_ACX_EFAXp_1_DS_TWO_PBIS   51
#define TYPE_ACX_EFA_2_TWO_PBIS        52
#define TYPE_ACX_EFAXp_2_TWO_PBIS      53
#define TYPE_ACX_EFA_2_DS_TWO_PBIS     54
#define TYPE_ACX_EFAXp_2_DS_TWO_PBIS   55
#define TYPE_ACX_EFA_4_TWO_PBIS        56
#define TYPE_ACX_EFAXp_4_TWO_PBIS      57

#define TYPE_AVX1553_4_DS_TWO_PBIS     58
#define TYPE_AVX_EFA_1_TWO_PBIS        59
#define TYPE_AVX_EFAXp_1_TWO_PBIS      60
#define TYPE_AVX_EFA_1_DS_TWO_PBIS     61
#define TYPE_AVX_EFAXp_1_DS_TWO_PBIS   62
#define TYPE_AVX_EFA_2_TWO_PBIS        63
#define TYPE_AVX_EFAXp_2_TWO_PBIS      64
#define TYPE_AVX_EFA_2_DS_TWO_PBIS     65
#define TYPE_AVX_EFAXp_2_DS_TWO_PBIS   66
#define TYPE_AVX_EFA_4_TWO_PBIS        67
#define TYPE_AVX_EFAXp_4_TWO_PBIS      68

#define TYPE_APU1553_1                 72
#define TYPE_APU1553_2                 73

#define TYPE_AEC1553_1                 74
#define TYPE_AEC1553_2                 75

#define TYPE_AXC1553_1                 76
#define TYPE_AXC1553_2                 77
#define TYPE_AXC1553_4                 78

#define TYPE_AMCX1553_1                79
#define TYPE_AMCX1553_2                80
#define TYPE_AMCX1553_4                81

#define TYPE_APE1553_1                 82
#define TYPE_APE1553_2                 83
#define TYPE_APE1553_4                 84

#define TYPE_ANET1553_1                85
#define TYPE_ANET1553_2                86

#define TYPE_ANET3910                  87
#define TYPE_ANET3910Xp                88

#define TYPE_ASC1553_1                 89

#define TYPE_APXX3910                  90
#define TYPE_APXX3910Xp                91
#define TYPE_APEX3910                  92
#define TYPE_APEX3910Xp                93

#define TYPE_AME1553_1                 94

#define TYPE_ACE3910                   95
#define TYPE_ACE3910Xp                 96

#define TYPE_AXE1553_1                 97
#define TYPE_AXE1553_2                 98
#define TYPE_AXE1553_4                 99

#define TYPE_AMCE1553_1                100
#define TYPE_AMCE1553_2                101
#define TYPE_AMCE1553_4                102

#define TYPE_ASE1553_1                 103
#define TYPE_ASE1553_2                 104
#define TYPE_ASE1553_4                 105

#define TYPE_AXC3910                   106
#define TYPE_AXC3910Xp                 107

#define TYPE_AMEE1553_1                108
#define TYPE_AMEE1553_2                109

#define TYPE_ASC1553_2                 110

#define TYPE_AXCX1553_1                111
#define TYPE_AXCX1553_2                112

#define TYPE_AM2E1553_1                113
#define TYPE_AM2E1553_2                114

#define TYPE_AXCX1553_4                115

/**************************/
/* structs and typedefs   */
/**************************/
typedef struct ty_api_open
{
    AiUInt32 ul_Module;
    AiUInt32 ul_Stream;
    AiChar   ac_SrvName[28];
} TY_API_OPEN;





typedef struct ty_api_board_capabilities
{
    AiUInt32                 ul_CanChangeAmplitude;
    TY_COUPLING_CAPABILITIES x_CouplingCapabilities;
    TY_IRIG_CAPABILITIES     x_IrigCapabilities;
} TY_API_BOARD_CAPABILITIES;



typedef struct ty_api_board_info
{
    AiUInt32 ul_DeviceType;
    AiUInt32 ul_NumberOfChannels;
    AiUInt32 ul_NumberOfBiu;
    AiUInt32 ul_NovRamBoardType;
    AiUInt32 ul_NovRamBoardConfig;
    AiUInt32 ul_SerialNumber;
    AiUInt32 ul_PartNumber;
    AiUInt32 ul_GlobalRamSize;
    AiUInt32 ul_SharedRamSize;
    AiUInt32 ul_GlobalRamStartOffset[MAX_BIU];
    TY_API_BOARD_CAPABILITIES x_BoardCapabilities;
    AiUInt32 ul_Reserved;
} TY_API_BOARD_INFO;


typedef struct api_device_config {
    AiUInt8  uc_DmaEnabled;
    AiUInt8  uc_DataQueueMemoryType;
    AiUInt8  uc_DataQueueMode;
    AiUInt8  uc_ReservedB4;
    AiUInt16 uw_ReservedW1;
    AiUInt16 uw_ReservedW2;
    AiUInt32 ul_DmaMinimumSize;
    AiUInt32 ul_IntRequestCount;
    AiUInt32 ul_DriverFlags;
    AiUInt32 ul_ReservedLW4;
    AiUInt32 ul_ReservedLW5;
    AiUInt32 ul_ReservedLW6;
    AiUInt32 ul_ReservedLW7;
    AiUInt32 ul_ReservedLW8;
} TY_API_DEVICE_CONFIG;

typedef enum hw_type {
    HW_TYPE_UNKNOWN = 0,
    HW_TYPE_PCI = 1
} TY_HW_TYPE;


typedef struct ty_hw_info{

    TY_HW_TYPE e_type;   // UNKNOWN, PCI
    AiUInt32 ul_ModuleNo;


    union {
        struct {
            AiUInt32 ul_Res1;
            AiUInt32 ul_Res2;
            AiUInt32 ul_Res3;
            AiUInt32 ul_Res4;
        } unknown;

        struct {
            AiUInt32 ul_Bus;
            AiUInt32 ul_Slot;
            AiUInt32 ul_Func;
            AiUInt32 ul_Res;
        } pci;
    } hw_types;

} TY_HW_INFO;

/***********************/
/* interface functions */
/***********************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiGetLibraryInfo       ( TY_API_LIB_INFO *px_LibInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiGetDriverInfo        ( AiUInt32 ulModule, TY_API_DRIVER_INFO * px_DriverInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiGetBoardInfo         ( AiUInt32 ulModule, TY_API_BOARD_INFO  * px_BoardInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiSetDllDbgLevel(AiUInt32 ul_DllDbgLevel);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiSetGlobalTestValue(AiUInt32 value);
AI_LIB_FUNC AiUInt32 AI_CALL_CONV ApiGetGlobalTestValue();
AI_LIB_FUNC void    AI_CALL_CONV ApiSetPnpCallback(void * /*TY_PNP_CALLBACK_FUNC_PTR*/ PnPCallbackFunction);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiInit                 (void);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiExit                 (void);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiHwQueryInformation(TY_HW_INFO ax_HardwareInfos[], AiUInt32 *ul_Count);
#if defined(_AIM_WINDOWS) || defined(_AIM_RTX64) || defined(_AIM_VXI) || defined (_AIM_LINUX)
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiOpen(AiUInt32 bModule, AiChar *ac_SrvName, AiUInt32 *ul_ModuleHandle));
#else
  AI_LIB_FUNC AiReturn AI_CALL_CONV ApiOpen               (AiUInt32 bModule);
#endif
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiOpenEx(TY_API_OPEN *px_ApiOpen, AiUInt32 *pul_ModuleHandle);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiClose(AiUInt32 bModule);
AI_LIB_FUNC const char *  AI_CALL_CONV ApiGetErrorDescription(AiInt16  wErrorCode);
AI_LIB_FUNC const char *  AI_CALL_CONV ApiGetBoardName(AiUInt32 ulModHandle);
AI_LIB_FUNC const char *  AI_CALL_CONV ApiGetDevicePath(AiUInt32 ulModHandle);
AI_LIB_FUNC const char *  AI_CALL_CONV ApiGetErrorMessage(AiReturn error_code);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiSetBoardName(AiUInt32 ulModHandle);

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiInitDeviceConfig(AiUInt32 ulModHandle);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiGetDeviceConfig(AiUInt32 ulModHandle, TY_API_DEVICE_CONFIG * pxConfig);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiSetDeviceConfig(AiUInt32 ulModHandle, TY_API_DEVICE_CONFIG * pxConfig);

#if defined(_AIM_WINDOWS) || defined(_AIM_RTX64) || defined(_AIM_VXI) || defined(_AIM_LINUX) || defined(_AIM_VME)
  #ifndef _NI_i386_        /* LabWindows CVI (National Instruments) */
AI_LIB_FUNC AiHandle AI_CALL_CONV ApiGetDrvHandle(AiUInt32 module);
  #endif
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiInstIntHandler       (AiUInt32 bModule, AiUInt8 biu, AiUInt8 uc_Type, TY_INT_FUNC_PTR pf_IntFunc );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiDelIntHandler        (AiUInt32 bModule, AiUInt8 biu, AiUInt8 uc_Type );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiFlushIntHandler      (AiUInt32 bModule );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiSetSysDbgLevel       (AiUInt32 bModule, AiUInt32 ul_DbgLevel );
#endif


AI_LIB_FUNC AiReturn AI_CALL_CONV ApiIoStruct( AiUInt32 ulModulem, TY_MIL_COM * com_p, TY_MIL_COM_ACK * ack_p );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiIo(AiUInt32 bModule, AiUInt8 biu, AiUInt32 cmd, AiInt16 expect, AiUInt8 *out_byte_p, AiInt16
                     out_byte_size, AiUInt16*out_word_p, AiInt16 out_word_size, AiUInt8 *in_byte_p, AiInt16
                     *in_byte_size, AiUInt16 *in_word_p, AiInt16*in_word_size, AiUInt32 *in_lword_p,
                     AiInt16*in_lword_size);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiExtIo(AiUInt32 bModule, AiUInt8 biu, AiUInt32 cmd, AiInt16 expect, AiUInt8 *out_byte_p, AiInt16
                        out_byte_size, AiUInt16 *out_word_p, AiInt16 out_word_size, AiUInt8*in_byte_p, AiInt16
                        *in_byte_size, AiUInt16 *in_word_p, AiInt16 *in_word_size, AiUInt32 *in_lword_p,
                        AiInt16*in_lword_size);





#if !defined(_AIM_LINUX) && !defined(_AIM_RTX64) && !defined(_AIM_VME) && !defined(_AIM_LVRT) 
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiExtInit(AiUInt32 bModule, AiUInt32 par1, AiUInt32 par2));
/*AI_LIB_FUNC AiReturn AI_CALL_CONV ApiExtIo(AiUInt32 bModule,AiUInt8 *cmd,AiUInt8 *ack);*/
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiExtCmdIni(AiUInt32 bModule, AiUInt32 *pini));
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiExtCmdRamWrite(AiUInt32 bModule, AiUInt8 access, AiUInt32 addr, AiUInt32 val));
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiExtCmdRamRead(AiUInt32 bModule, AiUInt8 access, AiUInt32 addr, AiUInt32 *val));
AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiExtCmdAllocMem(AiUInt32 bModule, AiUInt8 bar_no, AiUInt8 kbyte, AiUInt8 *st));
AI_LIB_FUNC AiReturn AI_CALL_CONV AiEnumerateDevices( AiUInt32 bModule, const GUID* guid, AiUInt32 par1, AiUInt32 par2, AiUInt32* pini);
#endif


/**********************/
/* internal functions */
/**********************/

AiInt16 ApiGetLibraryInfo_(TY_API_LIB_INFO *px_LibInfo);
AiInt16 ApiGetDriverInfo_(AiUInt32 ulModule, TY_API_DRIVER_INFO * px_DriverInfo);
AiInt16 ApiGetBoardInfo_(AiUInt32 bModule, TY_API_BOARD_INFO *px_BoardInfo);
AiInt16 ApiInit_( void);
AiInt16 ApiExit_( void);
AiInt16 ApiOpenEx_(TY_API_OPEN *px_ApiOpen, AiUInt32 *pul_ModuleHandle);
AiInt16 ApiClose_(AiUInt32 bModule);
const char * ApiGetErrorDescription_(AiReturn error_code);
const char * ApiGetBoardName_( AiUInt32 ulModHandle );
const char * ApiGetDevicePath_( AiUInt32 ulModHandle );
AiInt16 ApiGetDeviceConfig_( AiUInt32 ulModHandle, AiUInt8 * pucAnsUsed, TY_API_DEVICE_CONFIG * pxConfig );
AiInt16 ApiSetDeviceConfig_( AiUInt32 ulModHandle, AiUInt8 * pucAnsUsed, TY_API_DEVICE_CONFIG * pxConfig );

AI_LIB_FUNC AiReturn AI_CALL_CONV _ApiIo(AiUInt32 bModule, AiUInt8 *cmd, AiUInt8 *ack);

#endif

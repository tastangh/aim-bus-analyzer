/* SPDX-FileCopyrightText: 2016-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsSys.h
 *
 *  This header file contains declarations for
 *  system related API functionality
 *
 */


#ifndef API_LS_SYS_H
#define API_LS_SYS_H


/************/
/* defines  */
/************/
/* function ApiCmdReset() */
#define API_RESET_ALL                 0
#define API_RESET_WITHOUT_MONITOR     1
#define API_RESET_WITHOUT_SIMBUF      2
#define API_RESET_WITHOUT_SIMBUF_MON  3
#define API_RESET_ONLY_IF_NOT_ALREADY_RESETTED  4
#define API_RESET_USE_COUNTER_MODE_2  0x40
#define API_RESET_ENABLE_1760         0x80

/* function ApiCmdBite() */
#define API_BITE_ALL                  0
#define API_BITE_BOARD_ENABLE         1
#define API_BITE_INTERN_SELFTEST      2
#define API_BITE_GLOBAL_RAM_BIU       3
#define API_BITE_GLOBAL_RAM_BCRT      4
#define API_BITE_GLOBAL_RAM_BM        5
#define API_BITE_SHARED_RAM           6
#define API_BITE_SHARED_RAM_CMD       7
#define API_BITE_SHARED_RAM_ACK       8
#define API_BITE_TRANSFER             9
#define API_BITE_INTERRUPT            10
#define API_BITE_DA_CONV_BUSA         11
#define API_BITE_DA_CONV_BUSB         12
#define API_BITE_TIMING               13
#define API_BITE_DISABLE_RESET        0x80

/* first status word */
#define API_BITE_PASSED               0
#define API_BITE_ERR_BOARD_ENABLE     1
#define API_BITE_ERR_INTERN_SELFTEST  2
#define API_BITE_ERR_GLOBAL_RAM_BIU   3
#define API_BITE_ERR_GLOBAL_RAM_BCRT  4
#define API_BITE_ERR_GLOBAL_RAM_BM    5
#define API_BITE_ERR_SHARED_RAM       6
#define API_BITE_ERR_SHARED_RAM_CMD   7
#define API_BITE_ERR_SHARED_RAM_ACK   8
#define API_BITE_ERR_TRANSFER         9
#define API_BITE_ERR_INTERRUPT        10
#define API_BITE_ERR_DA_CONV_BUSA     11
#define API_BITE_ERR_DA_CONV_BUSB     12
#define API_BITE_ERR_TIMING           13

/* second status word */
#define API_BITE_ERR_INT_ADDR             16
#define API_BITE_ERR_INT_PAT_55           17
#define API_BITE_ERR_INT_PAT_AA           18
#define API_BITE_ERR_INT_PAT_FF           19
#define API_BITE_ERR_INT_PAT_00           20
#define API_BITE_ERR_INT_WALK_0           21
#define API_BITE_ERR_INT_WALK_1           22
#define API_BITE_ERR_INT_WRONG_PBI        23
#define API_BITE_ERR_INT_MILBUS           24



#define API_BITE_ERR_DATA_PATTERN         1
#define API_BITE_ERR_WALKING_BIT          2
#define API_BITE_ERR_ADDRESSING           3
#define API_BITE_ERR_BUS                  4

#define API_BITE_ERR_PAGING               1

#define API_BITE_ERR_BC_BROAD_ELECT_A     1
#define API_BITE_ERR_BC_BROAD_ELECT_B     2
#define API_BITE_ERR_BC_BROAD_ELECT_A_BM  3
#define API_BITE_ERR_BC_BROAD_ELECT_B_BM  4
#define API_BITE_ERR_BC_BROAD_ISOL_A_BM   5
#define API_BITE_ERR_BC_BROAD_ISOL_B_BM   6
#define API_BITE_ERR_BCRT_ISOL_AB_BM      7
#define API_BITE_ERR_RTRT_ISOL_A_BM       8
#define API_BITE_ERR_RTRT_ISOL_B_BM       9
#define API_BITE_ERR_MODE_ISOL_A_BM       10
#define API_BITE_ERR_MODE_ISOL_B_BM       11
#define API_BITE_ERR_BC_TIMEOUT_ISOL_A_BM 12
#define API_BITE_ERR_BC_TIMEOUT_ISOL_B_BM 13

#define API_BITE_ERR_TIMING_MFRM_50       1
#define API_BITE_ERR_TIMING_MFRM_10       2
#define API_BITE_ERR_TIMING_MFRM_1        3
#define API_BITE_ERR_TIMING_RT_RESPONSE   10
#define API_BITE_ERR_TIMING_IMG           20


/* function ApiCmdLoadSRec() */
#define API_SREC_INIT                 0
#define API_SREC_DOWNLOAD             1
#define API_SREC_START                2
#define API_SREC_START_ONCE           3

#define API_SREC_CHKS_OK              0
#define API_SREC_ERR_NO_STRING        1
#define API_SREC_ERR_WRONG_TYPE       2
#define API_SREC_ERR_CHKS             3
#define API_SREC_ERR_ADDR             4

/* function ApiCmdProgFlash() */
#define API_FLASH_SECTOR_LCA          6
#define API_FLASH_SECTOR_BIU1         14
#define API_FLASH_SECTOR_EF_BIU1      15
#define API_FLASH_SECTOR_BIU2         16
#define API_FLASH_SECTOR_EF_BIU2      17
#define API_FLASH_SECTOR_TARGET_SW    18
#define API_FLASH_SECTOR_EF_LCA       26

#define API_FLASH_SECTOR_APX_TARGET_SW 6
#define API_FLASH_SECTOR_APX_BIU1     40
#define API_FLASH_SECTOR_APX_BIU2     42
#define API_FLASH_SECTOR_APX_LCA      44
#define API_FLASH_SECTOR_APX_EF_LCA   52

#define API_FLASH_OK                  0
#define API_FLASH_ERR_ERASE           1
#define API_FLASH_ERR_PROG            2


/* function ApiCmdExecSys() */
#define API_MODE_TIMETAG_SOURCE       1
#define API_MODE_DSUB_CONNECT         2
#define API_MODE_HS_ADDR              8
#define API_MODE_DDL                  9
#define API_MODE_RESTART_TARGET_SW   10
#define API_MODE_SET_TRANSP_INTR     13 
#define API_MODE_RES_EXEC_SYS        14 
#define API_MODE_SHOW_PARAM_TABLE    15
#define API_MODE_BMINI_CLEAR_BUFFER  16

#define API_IRIG_ON_BOARD             0


/* function ApiCmdSysSetMemPartition() */
#define API_MEM_PART_OK             0
#define API_MEM_PART_ERR            1
#define API_MEM_PART_PARAM_ERR      2

/* function APiCmdUtility() */
#define API_UTIL_CMD_WRITE_NOVRAM     0
#define API_UTIL_CMD_READ_NOVRAM      1
#define API_UTIL_CMD_READ_SW_VERSIONS 2
#define API_UTIL_CMD_SWITCH_EFA_EFEX  4
#define API_UTIL_CMD_EF_TIME_OUT_DEF  5


/* function ApiCmdSysPXICon() */
#define API_PXI_SET_TRG               0
#define API_PXI_CLR_TRG               1
#define API_PXI_SET_TTSRC_EXTERNAL    2
#define API_PXI_SET_TTSRC_INTERNAL    3

#define API_TRG_PXI0                  0
#define API_TRG_PXI1                  1
#define API_TRG_PXI2                  2
#define API_TRG_PXI3                  3
#define API_TRG_PXI4                  4
#define API_TRG_PXI5                  5
#define API_TRG_PXI6                  6
#define API_TRG_PXI7                  7
#define API_TRG_BC_CHN1               8
#define API_TRG_BC_CHN2               9
#define API_TRG_BC_CHN3               10
#define API_TRG_BC_CHN4               11
#define API_TRG_RT_CHN1               12
#define API_TRG_RT_CHN2               13
#define API_TRG_RT_CHN3               14
#define API_TRG_RT_CHN4               15
#define API_TRG_BM_CHN1               16
#define API_TRG_BM_CHN2               17
#define API_TRG_BM_CHN3               18
#define API_TRG_BM_CHN4               19
#define API_TRG_BM_HS                 20

/* function ApiCmdDefMilbusProtocol() */
#define API_PROTOCOL_1553_A           0
#define API_PROTOCOL_1553_B           1
#define API_MODE_ALL_RT               0
#define API_MODE_SINGLE_RT            1
/* alt */
#define API_MILBUS_PROT_A 0
#define API_MILBUS_PROT_B 1
#define API_RT_ALL  0
#define API_RT_SING 1


/* function ApiCmdSystagDef() */
#define API_ENA_INIT                  3

#define API_BC_MODE                   1
#define API_RT_MODE                   2

#define API_SYSTAG_FCT_DISABLE        0
#define API_SYSTAG_FCT_POS_RAMP       1
#define API_SYSTAG_FCT_NEG_RAMP       2
#define API_SYSTAG_FCT_POS_TRIANGLE   3
#define API_SYSTAG_FCT_NEG_TRIANGLE   4
#define API_SYSTAG_FCT_DATASET        5
#define API_SYSTAG_FCT_STATES         6
#define API_SYSTAG_FCT_COMP           7
#define API_SYSTAG_FCT_CHECKSUM       8


#define API_SYSTAG_STEP_CYCLIC        0
#define API_SYSTAG_STEP_KEEP_LAST     1

#define API_SYSTAG_STEP_1S_COMP       1
#define API_SYSTAG_STEP_2S_COMP       2

#define API_SYSTAG_STEP_CHECKSUM_PLUS 0
#define API_SYSTAG_STEP_CHECKSUM_XOR  1
#define API_SYSTAG_STEP_CHECKSUM_1760 2

#define API_SYSTAG_XFERID_MASK        0xFFFF
#define API_SYSTAG_RTADDR_MASK        0xFF00
#define API_SYSTAG_SA_MID_MASK        0x00FF
#define API_SYSTAG_XFERID_POS         0
#define API_SYSTAG_RTADDR_POS         8
#define API_SYSTAG_SA_MID_POS         0

#define API_SYSTAG_BITPOS_MASK        0xFF00
#define API_SYSTAG_BITNB_MASK         0x00FF
#define API_SYSTAG_BITPOS_POS         8
#define API_SYSTAG_BITNB_POS          0

/* function ApiCmdSystagCon() */
#define API_SYSTAG_SUSPEND            0
#define API_SYSTAG_RESUME             1



#define API_IRIG_NOT_PRESENT      0
#define API_IRIG_PRESENT          1



/**************************/
/* structs and typedefs   */
/**************************/
typedef struct ty_api_ini_info
{
    AiUInt8 bt[4];
    AiUInt8 chns;
    AiUInt8 prot;
    AiUInt8 emod;
    AiUInt8 irg;
    AiUInt8 res1;
    AiUInt8 padd1;
    AiUInt16 padd2;
    AiUInt16 pbi_id_biu1;
    AiUInt16 asp_mon_id;
    AiUInt16 asp_bite_id;
    AiUInt16 pbi_id_biu2;
    AiUInt32 board_config;
    AiUInt32 glb_mem_size;
    AiUInt32 glb_mem_addr;
    AiUInt32 loc_dram_size;
    AiUInt32 loc_dram_addr;
    AiUInt32 shared_dram_size;
    AiUInt32 shared_dram_addr;
    AiUInt32 flash_ram_size;
    AiUInt32 flash_ram_addr;
    AiUInt32 pci[16];
    AiUInt32 board_type;
    AiUInt32 board_sub_type;
    AiUInt32 hardware_variant;
} TY_API_INI_INFO;


typedef struct ty_api_reset_info
{
    AiUInt8 mbufs;
    AiUInt8 sbufs;
    AiUInt16 padding1;
    AiUInt32 mon_addr;
    AiUInt32 sim_addr;
} TY_API_RESET_INFO;


typedef enum api_irig_source
{
    API_IRIG_INTERN = 0,
    API_IRIG_EXTERN = 1
} TY_API_IRIG_SOURCE;


typedef struct ty_api_irig_time
{
    AiUInt32 day; 
    AiUInt32 hour; 
    AiUInt32 minute; 
    AiUInt32 second; 
    AiUInt32 microsecond;
} TY_API_IRIG_TIME;



typedef struct ty_api_pxi_con
{
    AiUInt32 ul_Mode;
    AiUInt32 ul_TrgSource;
    AiUInt32 ul_TrgDest;
    AiUInt32 ul_TTClear;
} TY_API_PXI_CON;




typedef struct ty_api_sync_cnt_set
{
    AiUInt32 ul_SyncCntVal;
} TY_API_SYNC_CNT_SET;

typedef struct ty_api_sync_cnt_get
{
    AiUInt32 ul_SyncCntVal;
    AiUInt32 ul_SyncCntInit;
    AiUInt32 ul_SyncCntInitLow;
    AiUInt32 ul_SyncCntInitHigh;
} TY_API_SYNC_CNT_GET;


typedef struct ty_api_systag
{
    AiUInt16 xid_rtsa;
    AiUInt16 fct;
    AiUInt16 min;
    AiUInt16 max;
    AiUInt16 step;
    AiUInt16 wpos;
    AiUInt16 bpos;
} TY_API_SYSTAG;

typedef struct ty_api_discr_info
{
    AiUInt32 channels;
    AiUInt32 canIn;
    AiUInt32 canOut;
} TY_API_DISCR_INFO;

/***********************/
/* interface functions */
/***********************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdIni               (AiUInt32 bModule, AiUInt8 mode, TY_API_INI_INFO *pini);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReset             (AiUInt32 bModule, AiUInt8 biu, AiUInt8 rc, TY_API_RESET_INFO *pres);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdBite              (AiUInt32 bModule, AiUInt8 biu, AiUInt8 sc, AiUInt8 bite_status[]);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDefRespTout       (AiUInt32 bModule, AiUInt8 biu, AiFloat resp_tout);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdLoadSRec          (AiUInt32 bModule, AiUInt8 mode, AiUInt8 cnt, AiUInt32 offset, AiUInt8 *srec, AiUInt8 *st, AiUInt32 *fsize);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdProgFlash         (AiUInt32 bModule, AiUInt32 src_addr, AiUInt32 sector, AiUInt32 size, AiUInt8 *st);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdExecSys           (AiUInt32 bModule, AiUInt8 mode, AiUInt8 con);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGetIrigStatus     (AiUInt32 ul_ModuleHandle, TY_API_IRIG_SOURCE *source, AiBoolean * in_sync);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGetIrigTime       (AiUInt32 ul_ModuleHandle, TY_API_IRIG_TIME * time);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSetIrigStatus     (AiUInt32 ul_ModuleHandle, TY_API_IRIG_SOURCE new_source);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSetIrigTime       (AiUInt32 ul_ModuleHandle, TY_API_IRIG_TIME *time);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdDefMilbusProtocol (AiUInt32 bModule, AiUInt8 biu, AiUInt8 mil_prot, AiUInt8 mode, AiUInt8 rt);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdUtility           (AiUInt32 bModule, AiUInt8 uc_InCnt, AiUInt8 *puc_OutCnt, AiUInt32 *aul_InLongs, AiUInt32 *aul_OutLongs );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSystagDef         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 id, AiUInt8 con, AiUInt8 mode, TY_API_SYSTAG *psystag );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSystagCon         (AiUInt32 bModule, AiUInt8 biu, AiUInt8 id, AiUInt8 con );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysGetMemPartition(AiUInt32 bModule, AiUInt8 uc_Mode, TY_API_GET_MEM_INFO *px_MemInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysSetMemPartition(AiUInt32 bModule, AiUInt8 uc_Mode, TY_API_SET_MEM_INFO *px_MemInfo, AiUInt32 *pul_Status, AiUInt32 aul_MemUsed[2]);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysPingVariable   (AiUInt32 ulModule, AiUInt32 ulInputCount,  AiUInt32 aulInput[], AiUInt32 aulOutput[] );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysPing           (AiUInt32 ulModule, AiUInt32 ulInput, AiUInt32 * pulOutput );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysMalloc         (AiUInt32 ulModule, AiUInt8 ucMemtype, AiUInt32 ulSize,   AiUInt32 ulTag, AiUInt32 * pulOffset );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysFree           (AiUInt32 ulModule, AiUInt8 ucMemtype, AiUInt32 ulOffset, AiUInt32 ulTag );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysGetBoardInfo   (AiUInt32 ulModule, AiUInt32 ulDataStart, AiUInt32 ulDataCount, AiUInt32 *aulOutput, AiUInt32 *ulOutputCount);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReadDiscretes     (AiUInt32 ul_Module, AiUInt32 *pul_Value);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReadDiscretesConfig(AiUInt32 ul_Module, AiUInt32 *ul_DiscreteSetup);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReadDiscretesInfo(AiUInt32 ul_Module, TY_API_DISCR_INFO *px_DiscrInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdWriteDiscretes(AiUInt32 ul_Module, AiUInt32 ul_Mask, AiUInt32 ul_Value);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdInitDiscretes     (AiUInt32 ul_Module, AiUInt32 ul_DiscreteSetup);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdInitDiscretesRecording(AiUInt32 ul_Module, AiUInt32 ul_Config);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSyncCounterSet    (AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_SYNC_CNT_SET *px_SyncCnt);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSyncCounterGet    (AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_SYNC_CNT_GET *px_SyncCnt);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysPXICon         (AiUInt32 ul_Module, TY_API_PXI_CON *px_PxiCon);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysPXIGeographicalAddressGet(AiUInt32 ul_Module, AiUInt32 *pxiGeographicalAddress);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysGetMemLocation (AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 ul_Mode, AiUInt32 ul_Id, AiUInt32 *pul_MemType, AiUInt32 *pul_Offset);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysTriggerEdgeInputSet(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32 edge_setup);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysTriggerEdgeInputGet(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32 * edge_setup);

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysTriggerDigitalLoopCon(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32 config);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSysTriggerDigitalLoopGet(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32* config);


/**********************/
/* internal functions */
/**********************/
AiInt16 ApiCmdIniLegacy_(AiUInt32 bModule, AiUInt8 mode, TY_API_INI_INFO *pini);
AiInt16 ApiCmdIniStruct_(AiUInt32 bModule, AiUInt8 mode, TY_API_INI_INFO *pini);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdResetLegacy_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rc, TY_API_RESET_INFO *pres);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReset_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 rc, TY_API_RESET_INFO *pres);
AiInt16 ApiCmdBite_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 sc, AiUInt8 bite_status[]);
AiInt16 ApiCmdDefRespTout_(AiUInt32 bModule, AiUInt8 biu, AiFloat resp_tout);
AiInt16 ApiCmdLoadSRec_(AiUInt32 bModule, AiUInt8 mode, AiUInt8 cnt, AiUInt32 offset, AiUInt8 *srec, AiUInt8 *st, AiUInt32 *fsize);
AiInt16 ApiCmdProgFlash_(AiUInt32 bModule, AiUInt32 src_addr, AiUInt32 sector, AiUInt32 size, AiUInt8 *st);
AiInt16 ApiCmdExecSys_(AiUInt32 bModule, AiUInt8 mode, AiUInt8 con);
AiReturn ApiCmdGetIrigStatus_(AiUInt32 ul_ModuleHandle, TY_API_IRIG_SOURCE *source, AiBoolean * in_sync);
AiReturn ApiCmdGetIrigTime_(AiUInt32 ul_ModuleHandle, TY_API_IRIG_TIME * time);
AiReturn ApiCmdSetIrigStatus_(AiUInt32 ul_ModuleHandle, TY_API_IRIG_SOURCE new_source);
AiReturn ApiCmdSetIrigTime_(AiUInt32 ul_ModuleHandle, TY_API_IRIG_TIME *time);
AiInt16 ApiCmdDefMilbusProtocol_(AiUInt32 bModule, AiUInt8 biu, AiUInt8 mil_prot, AiUInt8 mode, AiUInt8 rt);
AiInt16 ApiCmdUtility_(AiUInt32 bModule, AiUInt8 uc_InCnt, AiUInt8 *puc_OutCnt, AiUInt32 *aul_InLongs, AiUInt32 *aul_OutLongs );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdSystagDef(AiUInt32 bModule, AiUInt8 biu, AiUInt8 id, AiUInt8 con, AiUInt8 mode, TY_API_SYSTAG *psystag);
AiInt16 ApiCmdSystagCon_( AiUInt32 bModule, AiUInt8 biu, AiUInt8 id, AiUInt8 con );
AiInt16 ApiCmdSysGetMemPartition_(AiUInt32 bModule, AiUInt8 mode, TY_API_GET_MEM_INFO *mem_info);
AiInt16 ApiCmdSysSetMemPartition_(AiUInt32 bModule, AiUInt8 uc_Mode, TY_API_SET_MEM_INFO *px_MemInfo, AiUInt32 *pul_Status, AiUInt32 aul_MemUsed[2]);
AiInt16 ApiCmdReadDiscretes_(AiUInt32 ul_Module, AiUInt32 *pul_Value);
AiInt16 ApiCmdReadDiscretesConfig_(AiUInt32 ul_Module, AiUInt32 *pul_Value);
AiInt16 ApiCmdWriteDiscretes_(AiUInt32 ul_Module, AiUInt32 ul_Mask, AiUInt32 ul_Value);
AiInt16 ApiCmdInitDiscretes_(AiUInt32 ul_Module, AiUInt32 ul_DiscreteSetup);
AiInt16 ApiCmdInitDiscretesRecording_(AiUInt32 ul_Module, AiUInt32 ul_Config);
AiInt16 ApiCmdSyncCounterSet_(AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_SYNC_CNT_SET *px_SyncCntSet);
AiInt16 ApiCmdSyncCounterGet_(AiUInt32 ul_Module, AiUInt8 uc_Biu, TY_API_SYNC_CNT_GET *px_SyncCntGet);
AiInt16 ApiCmdSysPXICon_(AiUInt32 ul_Module, TY_API_PXI_CON *px_PxiCon);
AiReturn ApiCmdSysPXIGeographicalAddressGet_(AiUInt32 ul_Module, AiUInt32 *pxiGeographicalAddress);
AiInt16 ApiCmdSysGetMemLocation_(AiUInt32 ul_Module, AiUInt8 uc_Biu, AiUInt32 ul_Mode, AiUInt32 ul_Id, AiUInt32 *pul_MemType, AiUInt32 *pul_Offset);
AiReturn ApiCmdSysTriggerEdgeInputSet_(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32 edge_setup);
AiReturn ApiCmdSysTriggerEdgeInputGet_(AiUInt32 ul_Module, AiUInt8 biu, AiUInt32 * edge_setup);

#endif


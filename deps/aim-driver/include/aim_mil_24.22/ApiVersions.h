/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiVersions.h
 *
 *  This header file contains declarations for
 *  version related API functionality
 *
 */

#ifndef AI1553_VERSIONS_H
#define AI1553_VERSIONS_H 



/************/
/* defines  */
/************/

#define API_SPECIAL_VER_MASK          0xFFFF0000
#define API_VER_MASK                  0x0000FFFF
#define API_FIRMWARE_BIU2_VER_MASK    0xFFFF0000
#define API_FIRMWARE_BIU1_VER_MASK    0x0000FFFF
#define API_LCA_BIU2_VER_MASK         0xFFFF0000
#define API_LCA_BIU1_VER_MASK         0x0000FFFF

#define API_BSP_COMPATIBLE            0x00
#define API_BSP_NOT_COMPATIBLE        0xFF


#define API_BSP_WRONG_SYS_W95_SW       0x01
#define API_BSP_WRONG_SYS_WNT_SW       0x02
#define API_BSP_WRONG_TARGET_SW        0x03
#define API_BSP_WRONG_BIU1_SW          0x04
#define API_BSP_WRONG_BIU2_SW          0x05
#define API_BSP_WRONG_LCA1_SW          0x06
#define API_BSP_WRONG_LCA2_SW          0x07
#define API_BSP_WRONG_SYS_W98_W2000_SW 0x08
#define API_BSP_WRONG_BIU3_SW          0x09
#define API_BSP_WRONG_BIU4_SW          0x0A
#define API_BSP_WRONG_SYS_DRV          0x0B





/**************************/
/* structs and typedefs   */
/**************************/


typedef struct ty_api_version
{
    AiUInt32 ul_MajorVer;
    AiUInt32 ul_MinorVer;
    AiUInt32 ul_BuildNo;
    AiUInt32 ul_MajorSpecialVer;
    AiUInt32 ul_MinorSpecialVer;
} TY_API_VERSION;

typedef struct ty_api_version_info
{
    TY_API_VERSION x_TcpVer;
    TY_API_VERSION x_AslLcaVer;
    TY_API_VERSION x_PciLcaVer;
    TY_API_VERSION x_IoLcaBiu1Ver;
    TY_API_VERSION x_CoreLcaBiu1Ver;
    TY_API_VERSION x_IoLcaBiu2Ver;
    TY_API_VERSION x_EndDecLcaBiu2Ver;
    TY_API_VERSION x_IoLcaBiu3Ver;
    TY_API_VERSION x_CoreLcaBiu3Ver;
    TY_API_VERSION x_IoLcaBiu4Ver;
    TY_API_VERSION x_EndDecLcaBiu4Ver;
    TY_API_VERSION x_FirmwareBiu1Ver;
    TY_API_VERSION x_FirmwareBiu2Ver;
    TY_API_VERSION x_FirmwareBiu3Ver;
    TY_API_VERSION x_FirmwareBiu4Ver;
    TY_API_VERSION x_TargetSWVer;
    TY_API_VERSION x_SysDrvVer;
    TY_API_VERSION x_DllVer;
    TY_API_VERSION x_MonitorVer;
    AiUInt32 ul_BoardSerialNo;
} TY_API_VERSION_INFO;



/***********************/
/* interface functions */
/***********************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadBSPVersion       (AiUInt32 bModule, AiUInt32 *pul_FirmwareVer, AiUInt32 *pul_TargetVer,
                                                           AiUInt32 *pul_TargetBuild, AiUInt32 *pul_LcaVer1,
                                                           AiUInt32 *pul_LcaVer2, AiUInt32 *pul_LcaCheckSum,
                                                           AiUInt32 *pul_SysDrvVer, AiUInt32 *pul_SysDrvBuild,
                                                           AiUInt32 *pul_DllVer, AiUInt32 *pul_DllBuild,
                                                           AiUInt32 *pul_BoardSerialNr, AiUInt8  *puc_BspCompatibility );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadBSPVersionEx     (AiUInt32 bModule, TY_API_VERSION_INFO *px_VersionInfo, AiUInt8  *puc_BspCompatibility );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdReadSWVersion     (AiUInt32 bModule, AiUInt8 biu, AiUInt16 *fw_id, AiUInt16 *sw_id, AiUInt16 *lca_id,  AiUInt32 *lca_chks );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadTswCompatVersion(AiUInt32 ulModHandle, TY_API_VERSION * version);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadAllVersions      (AiUInt32           ul_ModuleHandle,
															AiUInt32          ul_Count,
															TY_VER_INFO       ax_Versions[],
															TY_VERSION_OUT*   px_VersionInfoOut);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadVersion          (AiUInt32 ul_Handle, TY_E_VERSION_ID eId, TY_VER_INFO * pxVersion );


/*! \Reads the system driver version of a board
 *
 * Format of the version is major * 100 + minor.
 * So version / 100 gives major version and version % 100 gives minor version.
 * \param ul_ModuleHandle handle of the board to read driver version of
 * \param pul_SysDrvVer pointer to value that receives system driver version.
 * \param pul_SysDrvBuild pointer to value that receives driver build number
 * \return returns API_OK on success, an appropriate error code otherwise
 */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGetSysDrvVersion(AiUInt32 ul_ModuleHandle, AiUInt32* pul_SysDrvVer, AiUInt32* pul_SysDrvBuild);



/**********************/
/* internal functions */
/**********************/

AiInt16 ApiReadBSPVersionEx_(AiUInt32 bModule, TY_API_VERSION_INFO *px_VersionInfo, AiUInt8  *puc_BspCompatibility );
AiInt16 ApiCmdReadSWVersion_(AiUInt32 bModule, AiUInt8 biu, AiUInt16 *fw_id, AiUInt16 *sw_id, AiUInt16 *lca_id,  AiUInt32 *lca_chks );


#endif

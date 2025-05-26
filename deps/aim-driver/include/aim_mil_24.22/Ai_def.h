/* SPDX-FileCopyrightText: 2011-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*
    Description:

    Interface Header file for the generic data types shared between different
    boards (e.g. AFDX, GNET, FC).
*/
#ifndef AI_DEF_H
#define AI_DEF_H

#include "Ai_types.h"




/*! \def AI_DESCRIPTION_STRING
    Maximum length of version description including trailing zero */
#define AI_DESCRIPTION_STRINGL 256


/*! \def AI_VERSION_STRINGL
    Maximum length of full version string including trailing zero */
#define AI_VERSION_STRINGL 256


/*! \struct TY_VER_INFO
    Protocol independent version structure. \n
    Describes the version of one specific component. \n
    Used in protocol dependent ReadSWVersion API calls. */
typedef struct ty_ver_info{
    AiUInt32 ul_VersionType;                           /*!< see \ref TY_E_VERSION_ID. ID of the component the version is for */
    AiChar   ac_Description[AI_DESCRIPTION_STRINGL];   /*!< zero-terminated ASCII string that describes the component the version is for */
    AiUInt32 ul_MajorVer;                              /*!< Major version. Must be increased when component interface gets incompatible (e.g. API call signature change) */
    AiUInt32 ul_MinorVer;                              /*!< Minor version. Must be increased when component interface offers new functionality (e.g. new API call) */
    AiUInt32 ul_PatchVersion;                          /*!< Patch version. Must be increased when componnet interface is stable, but e.g. bugs were fixed */
    AiUInt32 ul_BuildNr;                               /*!< Unique number that describes a specific state of the component */
    AiChar   ac_FullVersion[AI_VERSION_STRINGL];       /*!< zero-terminated ASCII version string. Should hold main version in dot-separated major.minor.patch.build format.\n
                                                            Version extension should be hyphen separated from main version. For example,Git based projects could add commit SHA1 (e.g. -g324FD) */
} TY_VER_INFO;



/* Structure for read board software versions*/
typedef struct ty_ver_no{
   AiUInt32 ul_MajorVer;        /* Major Version Number*/
   AiUInt32 ul_MinorVer;        /* Minor Version Number*/
   AiUInt32 ul_BuildNr;         /* Build Number*/
   AiUInt32 ul_MajorSpecialVer; /* Major Version Number*/
   AiUInt32 ul_MinorSpecialVer; /* Minor Version Number*/
} TY_VER_NO;



typedef struct ty_version_out{
		AiUInt32 ul_Count;
		AiUInt32 ul_MaxCount;
} TY_VERSION_OUT;


/*! \enum TY_E_VERSION_ID
    Enumeration for all components
    that can be versioned */
typedef enum{
   AI_SYS_DRV_VER          = 0,
   AI_ANS_VER              = 1,
   AI_DLL_VER              = 2,
   AI_REMOTE_DLL_VER       = 3,
   AI_TARGET_VER           = 4,
   AI_FIRMWARE_VER_BIU1    = 5,
   AI_MAIN_LCA_VER         = 6, /*!< MLCA/NOVRAM (PL+Microblaze SW)*/
   AI_IO_LCA_VER_BIU1      = 7,
   AI_PCI_LCA_VER          = 8,
   AI_TCP_VER              = 9,
   AI_BOOTSTRAP_VER        = 10,
   AI_MONITOR_VER          = 11,
   AI_TARGET_OS_VER        = 12,
   AI_FPGA_VER             = 13, /*!< AyE=FPGA overall, AyS=PL */
   AI_FIRMWARE_VER_BIU2    = 14,
   AI_FIRMWARE_VER_BIU3    = 15,
   AI_FIRMWARE_VER_BIU4    = 16,
   AI_IO_LCA_VER_BIU2      = 17,
   AI_IO_LCA_VER_BIU3      = 18,
   AI_IO_LCA_VER_BIU4      = 19,
   AI_SUBDLL1_VER          = 20,
   AI_SUBDLL2_VER          = 21,
   AI_SUBDLL3_VER          = 22,
   AI_FIRMWARE_PACKAGE_VER = 23,
   __AI_MAX_VERSIONS       /*! Indicates last entry. All new version IDs must be put before this one!!! */
} TY_E_VERSION_ID;


/*! \def AI_MAX_VERSIONS

    Defines maximum number of known version IDs */
#define AI_MAX_VERSIONS __AI_MAX_VERSIONS




typedef enum ty_e_mem_type{
    AI_MEMTYPE_GLOBAL            = 0,
    AI_MEMTYPE_SHARED            = 1,
    AI_MEMTYPE_LOCAL             = 2,
    AI_MEMTYPE_IO                = 3,
    AI_MEMTYPE_GLOBAL_DIRECT     = 4,
    AI_MEMTYPE_GLOBAL_EXTENSION  = 5,
    AI_MEMTYPE_RX_MEM            = 6,
    AI_MEMTYPE_TX_MEM            = 7,
	AI_MEMTYPE_REF_MEM           = 8

}TY_E_MEM_TYPE;

#define AI_MMAP_OFFSET(m) (m<<24)

/***********************************************************************
 *                                                                     *
 *                  Common NOVRAM values                               *
 *                                                                     *
 **********************************************************************/


/*
  Plattform Type Constants (NOVRAM BoardConfig & 0xFF )
*/

#define AI_PLATFORM_PCI_LONG                0x10 /* API3910           */
#define AI_PLATFORM_PCI_SHORT               0x12 /* API1553-1         */
#define AI_PLATFORM_PCIX_3_3V               0x13 /* APX-GNET / APX-FC */
#define AI_PLATFORM_PCI_SHORT_ZYNQMP        0x14 /* Ultra Scale PCI   */
#define AI_PLATFORM_PCIE_4L_PCIX_3_3V_BASED 0x18 /* APE-GNET / APE-FC */
#define AI_PLATFORM_PCIE_4L_AYS             0x19 /* AYS-FC            */
#define AI_PLATFORM_PCI_E_1L                0x1A /* APS-GNET / APE-FDX */
#define AI_PLATFORM_PCI_E_1L_ZYNQMP         0x1B /* Ultra Scale PCIe  */
#define AI_PLATFORM_VME_B                   0x20
#define AI_PLATFORM_VMEX_B                  0x21
#define AI_PLATFORM_VME_A                   0x22
#define AI_PLATFORM_VMEX_A                  0x23
#define AI_PLATFORM_VXI_C                   0x30
#define AI_PLATFORM_VXI_B                   0x32
#define AI_PLATFORM_VXI_D                   0x34
#define AI_PLATFORM_CPCI_3U                 0x40
#define AI_PLATFORM_CPCIX_3U                0x41 /* ACX1553-3U         */
#define AI_PLATFORM_CPCI_6U                 0x42
#define AI_PLATFORM_CPCIX_6U                0x43 /* ACX3910-2          */
#define AI_PLATFORM_CPCIE_3U                0x44 /* ACE1553-3U-PXI     */
#define AI_PLATFORM_CPCIX_3U_PCIE_BASED     0x45 /* ACXX1553-3U-PXI    */
#define AI_PLATFORM_CPCIE_3U_AYS            0x46 /* e.g. ACE-FDX       */
#define AI_PLATFORM_CPCIX_3U_AYS            0x47 /* e.g. ACXX-FDX      */
#define AI_PLATFORM_CPCIE_3U_ZYNQMP         0x48 /* e.g. ACE3910       */
#define AI_PLATFORM_PMC_32                  0x50 /* AMC1553-ML <  3    */
#define AI_PLATFORM_PMC_32_QUAD             0x51 /* AMC1553-ML >= 3    */
#define AI_PLATFORM_PMC_32_ASP              0x52
#define AI_PLATFORM_PMC_AYS                 0x53 /* AMCX-FDX           */
#define AI_PLATFORM_PMC_64                  0x54
#define AI_PLATFORM_PMC_64_ASP              0x56 /* AMC1553-T          */
#define AI_PLATFORM_PMC_XMC_BASED           0x58 /* AMCX (AyE based)   */
#define AI_PLATFORM_PMC_BASED_EBD           0x59 /* AMCE (Embedded) (AyE based) */
#define AI_PLATFORM_PCI_64_ASP              0x60 /* API-FDX            */
#define AI_PLATFORM_PC_CARD                 0x70 /* APM1553            */
#define AI_PLATFORM_PCIE_CARD               0x75 /* AEC1553            */
#define AI_PLATFORM_MINI_PCIE_CARD          0x76 /* AME1553            */
#define AI_PLATFORM_MINI_PCIE_CARD_AP       0x77 /* AME1553-1-AP       */
#define AI_PLATFORM_MINI_PCIE_ARTIX7        0x78 /* AMEE1553           */
#define AI_PLATFORM_M2_B_M_ARTIX7           0x79 /* AM2E1553 (M.2 2260-D2-B-M) */
#define AI_PLATFORM_PC104                   0x80 /* PC104-1553         */
#define AI_PLATFORM_PCIX                    0x90 /* APX1553            */
#define AI_PLATFORM_PCIE_PCIX_BASED         0x91 /* APE3910 (AyX based)*/
#define AI_PLATFORM_PCIE                    0x98 /* APE1553            */
#define AI_PLATFORM_PCIX_PCIE_BASED         0x99 /* APXE1553           */
#define AI_PLATFORM_USB                     0xA0 /* APU1553            */
#define AI_PLATFORM_ASC                     0xA8 /* ASC                */
#define AI_PLATFORM_USB3                    0xA9 /* ASC1553-2 Ultra Scale+ USB3 */
#define AI_PLATFORM_XMC                     0xB0 /* AXC (AyE-based)    */
#define AI_PLATFORM_XMC_AYS                 0xB1 /* AXC-FDX            */
#define AI_PLATFORM_XMC_EBD                 0xB2 /* AXE (Embedded) (AyE based)   */
#define AI_PLATFORM_XMC_NOREPLAY_NOERROR    0xB3 /* ASE: Single Function Type XMC- Module on Carrier module, No Replay, no Error Injection (AyE based)   */
#define AI_PLATFORM_XMC_ZYNQMP              0xB4 /* XMC- Module with ASP based on USC+ Architecture */
#define AI_PLATFORM_XMC_ARTIXUS             0xB5 /* XMC- Module without ASP based on Artix USC+ Architecture */
#define AI_PLATFORM_ANET                    0xC0 /* ANET Ethernet based boards */
#define AI_PLATFORM_ANET_AYS                0xC1 /* ANET Ethernet based boards with Zynq architecture */
#define AI_PLATFORM_ANET_AYS_MA             0xC2 /* ANET-MxAy with Zynq architecture supporting multi-protocol with MIL-STD1553-and Arinc 429 */




/***********************************************************************
 *                                                                     *
 *         ApiGetDriverInfo struct definition and defines              *
 *                                                                     *
 **********************************************************************/

typedef struct ty_driver_info
{
    AiUInt8  uc_DeviceGroup;
    AiUInt8  uc_ReservedB2;
    AiUInt8  uc_ReservedB3;
    AiUInt8  uc_ReservedB4;
    AiUInt16 uw_ReservedW1;
    AiUInt16 uw_ReservedW2;
    AiUInt32 ul_DriverFlags;
    AiUInt32 ul_SN;
    AiUInt32 ul_BoardConfig;
    AiUInt32 ul_BoardType; 
    AiUInt32 ul_OpenConnections;
    AiUInt32 ul_ReservedLW6;
    AiUInt32 ul_ReservedLW7;
    AiUInt32 ul_ReservedLW8;
} TY_DRIVER_INFO;



/* -- Types / Defines -- */

#define AI_DEVICE_GROUP_COUNT 15

/* The device names are currently only used in AimPciContext.c
   If this names are required by the LIB the names can be 
   defined here and be populated in a file Ai_def.c
*/
/* extern const char * AI_DEVICE_GROUP_NAMES[]; */

typedef enum __ty_e_device_group {
            AI_DEVICE_UNKNOWN  = 0,
            AI_DEVICE_AYI      = 1,
            AI_DEVICE_AYX      = 2,
            AI_DEVICE_AMC      = 3,
            AI_DEVICE_AMC_ASP  = 4,
            AI_DEVICE_AYE      = 5,
            AI_DEVICE_AYE_ASP  = 6,
            AI_DEVICE_AYXGNET  = 7,
            AI_DEVICE_USB      = 8,
            AI_DEVICE_AYS_ASP  = 9,
            AI_DEVICE_AYS_ASP_MA = 10,
            AI_DEVICE_ZYNQMP_ASP = 11,
            AI_DEVICE_AYS        = 12,
            AI_DEVICE_ARTIX7     = 13,
            AI_DEVICE_ARTIXUS    = 14
} TY_E_DEVICE_GROUP, *PTY_E_DEVICE_GROUP;


#endif /* AI_DEF_H */

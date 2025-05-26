/* SPDX-FileCopyrightText: 2015-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file AiMilError.h
 *
 *  This module contains the definitions of the error codes and a human readable error description
 *
 */

#ifndef __AIMILERRORS_H__
#define __AIMILERRORS_H__





/*********************************************************************************/
/*                                                                               */
/*                             Error defines                                     */
/*                                                                               */
/*********************************************************************************/


/* -- host to target communication --- */
#ifndef API_OK
#define API_OK 0
#endif

#define API_ERR                                 0x0001
#define API_ERR_ACK                             0x0002
#define API_ERR_ACK2                            0x0003
#define API_ERR_NAK                             0x0004
#define API_ERR_HOST_TO_TARGET                  0x0005
#define API_ERR_INVALID_DRIVER_HANDLE           0x0006
#define API_ERR_IOCTL_TIMEOUT                   0x0007
#define API_ERR_IOCTL_ERROR                     0x0008
#define API_ERR_DEVICE_NOT_FOUND                0x0009
#define API_ERR_WRONG_TSW_VERSION               0x000A
#define API_ERR_WRONG_DRV_VERSION               0x000B
#define API_ERR_UNKNOWN_COMMAND                 0x000C



/* -- API --- */

#define API_ERR_WRONG_MODULE                      0x0010
#define API_ERR_WRONG_DEVICE                      0x0011
#define API_ERR_CNT_IS_ZERO                       0x0012
#define API_ERR_PARAM1_IS_NULL                    0x0013
#define API_ERR_PARAM2_IS_NULL                    0x0014
#define API_ERR_PARAM3_IS_NULL                    0x0015
#define API_ERR_PARAM4_IS_NULL                    0x0016
#define API_ERR_PARAM5_IS_NULL                    0x0017
#define API_ERR_DEV_IS_NOT_CTX                    0x0018
#define API_ERR_PARAM1_NOT_IN_RANGE               0x0019
#define API_ERR_PARAM2_NOT_IN_RANGE               0x001A
#define API_ERR_PARAM3_NOT_IN_RANGE               0x001B
#define API_ERR_PARAM4_NOT_IN_RANGE               0x001C
#define API_ERR_PARAM5_NOT_IN_RANGE               0x001D
#define API_ERR_PARAM6_NOT_IN_RANGE               0x001E
#define API_ERR_NO_CTX_FOUND                      0x001F
#define API_ERR_WRONG_BIU                         0x0020
#define API_ERR_PARAM6_IS_NULL                    0x0021
#define API_ERR_PARAM7_IS_NULL                    0x0022
#define API_ERR_PARAM8_IS_NULL                    0x0023
#define API_ERR_PARAM9_IS_NULL                    0x0024
#define API_ERR_PARAM10_IS_NULL                   0x0025
#define API_ERR_PARAM11_IS_NULL                   0x0026
#define API_ERR_PARAM12_IS_NULL                   0x0027
#define API_ERR_PARAM13_IS_NULL                   0x0028
#define API_ERR_PARAM14_IS_NULL                   0x0029
#define API_ERR_PARAM7_NOT_IN_RANGE               0x002A
#define API_ERR_PARAM8_NOT_IN_RANGE               0x002B
#define API_ERR_PARAM9_NOT_IN_RANGE               0x002C
#define API_ERR_PARAM10_NOT_IN_RANGE              0x002D
#define API_ERR_PARAM11_NOT_IN_RANGE              0x002E
#define API_ERR_PARAM12_NOT_IN_RANGE              0x002F
#define API_ERR_PARAM13_NOT_IN_RANGE              0x0030
#define API_ERR_PARAM14_NOT_IN_RANGE              0x0031
#define API_ERR_XID_NOT_IN_RANGE                  0x0032
#define API_ERR_HID_NOT_IN_RANGE                  0x0033
#define API_ERR_TYPE_NOT_IN_RANGE                 0x0034
#define API_ERR_CHN_NOT_IN_RANGE                  0x0035
#define API_ERR_XMT_RT_NOT_IN_RANGE               0x0036
#define API_ERR_RCV_RT_NOT_IN_RANGE               0x0037
#define API_ERR_XMT_SA_NOT_IN_RANGE               0x0038
#define API_ERR_RCV_SA_NOT_IN_RANGE               0x0039
#define API_ERR_WCNT_NOT_IN_RANGE                 0x003A
#define API_ERR_TIC_NOT_IN_RANGE                  0x003B
#define API_ERR_HLT_NOT_IN_RANGE                  0x003C
#define API_ERR_SXH_NOT_IN_RANGE                  0x003D
#define API_ERR_RSP_NOT_IN_RANGE                  0x003E
#define API_ERR_GAP_MODE_NOT_IN_RANGE             0x003F
#define API_ERR_ERRTYPE_NOT_IN_RANGE              0x0040
#define API_ERR_ERRSPEC_WPOS_NOT_IN_RANGE         0x0041
#define API_ERR_ERRSPEC_BPOS_NOT_IN_RANGE         0x0042
#define API_ERR_ERRSPEC_BCBITS_NOT_IN_RANGE       0x0043
#define API_ERR_ERRSPEC_CONTIG_NOT_IN_RANGE       0x0044
#define API_ERR_GAP_NOT_IN_RANGE                  0x0045
#define API_ERR_BCDYTAG_TAGFCT_NOT_IN_RANGE       0x0046
#define API_ERR_BCDYTAG_MIN_NOT_IN_RANGE          0x0047
#define API_ERR_BCDYTAG_MAX_NOT_IN_RANGE          0x0048
#define API_ERR_BCDYTAG_STEP_NOT_IN_RANGE         0x0049
#define API_ERR_BCDYTAG_WPOS_NOT_IN_RANGE         0x004A
#define API_ERR_FRAME_ID_NOT_IN_RANGE             0x004B
#define API_ERR_FRAME_CNT_NOT_IN_RANGE            0x004C
#define API_ERR_FRAME_INSTR_NOT_IN_RANGE          0x004D
#define API_ERR_FRAME_XID_NOT_IN_RANGE            0x004E
#define API_ERR_FRAME_FID_NOT_IN_RANGE            0x004F
#define API_ERR_CAPMODE_NOT_IN_RANGE              0x0050
#define API_ERR_CAPFSIZE_NOT_IN_RANGE             0x0051
#define API_ERR_TCBTT_NOT_IN_RANGE                0x0052
#define API_ERR_TCBSOT_NOT_IN_RANGE               0x0053
#define API_ERR_TCBTRI_NOT_IN_RANGE               0x0054
#define API_ERR_TCBINV_NOT_IN_RANGE               0x0055
#define API_ERR_TCBNEXT_NOT_IN_RANGE              0x0056
#define API_ERR_TCBEOM_NOT_IN_RANGE               0x0057
#define API_ERR_POS1_NOT_IN_RANGE                 0x0058
#define API_ERR_POS2_NOT_IN_RANGE                 0x0059
#define API_ERR_POS1_GREATER_THAN_POS2            0x005A
#define API_ERR_ADDR_NOT_VALID                    0x005B
#define API_ERR_RTDYTAG_TAGFCT_NOT_IN_RANGE       0x005C
#define API_ERR_RTDYTAG_MIN_NOT_IN_RANGE          0x005D
#define API_ERR_RTDYTAG_MAX_NOT_IN_RANGE          0x005E
#define API_ERR_RTDYTAG_STEP_NOT_IN_RANGE         0x005F
#define API_ERR_RTDYTAG_WPOS_NOT_IN_RANGE         0x0060
#define API_ERR_IRIG_HOUR_NOT_IN_RANGE            0x0061
#define API_ERR_IRIG_MIN_NOT_IN_RANGE             0x0062
#define API_ERR_IRIG_SEC_NOT_IN_RANGE             0x0063

#define API_ERR_SYSTAG_XIDRTMID_NOT_IN_RANGE      0x0064
#define API_ERR_SYSTAG_FCT_NOT_IN_RANGE           0x0065
#define API_ERR_SYSTAG_MIN_NOT_IN_RANGE           0x0066
#define API_ERR_SYSTAG_MAX_NOT_IN_RANGE           0x0067
#define API_ERR_SYSTAG_STEP_NOT_IN_RANGE          0x0068
#define API_ERR_SYSTAG_WPOS_NOT_IN_RANGE          0x0069
#define API_ERR_SYSTAG_BPOS_NOT_IN_RANGE          0x006A
#define API_ERR_QUEUE_EMPTY                       0x006B
#define API_ERR_QUEUE_OVERFLOW                    0x006C
#define API_ERR_1760_MODE_NOT_IN_RANGE            0x006D

#define API_ERR_1760_BUFID_NOT_IN_RANGE           0x006E
#define API_ERR_1760_C01_NOT_IN_RANGE             0x006F
#define API_ERR_1760_C02_NOT_IN_RANGE             0x0070

#define API_ERR_DQUEUE_ID_NOT_IN_RANGE            0x0071
#define API_ERR_DQUEUE_LOC_BUFTYPE_NOT_IN_RANGE   0x0072
#define API_ERR_DQUEUE_REM_BUFTYPE_NOT_IN_RANGE   0x0073
#define API_ERR_DQUEUE_LOC_MEMSIZE_NOT_IN_RANGE   0x0074
#define API_ERR_DQUEUE_REM_MEMSIZE_NOT_IN_RANGE   0x0075
#define API_ERR_DQUEUE_LOC_HANDLE_NOT_IN_RANGE    0x0076
#define API_ERR_DQUEUE_REM_HANDLE_NOT_IN_RANGE    0x0077
#define API_ERR_DQUEUE_ASP_SIZE_NOT_IN_RANGE      0x0078
#define API_ERR_DQUEUE_CONTROL_MODE_NOT_IN_RANGE  0x0079
#define API_ERR_DQUEUE_READ_NOT_BUFFERED          0x007A
#define API_ERR_DQUEUE_ASP_SIZE_TOO_BIG           0x007B
#define API_ERR_DQUEUE_ASP_OPEN                   0x007C
#define API_ERR_PARA_ID_NOT_IN_RANGE              0x007D
#define API_ERR_PARA_MODE_NOT_IN_RANGE            0x007E
#define API_ERR_PARA_BUF_NOT_IN_RANGE             0x007F
#define API_ERR_PARA_TYPE_NOT_IN_RANGE            0x0080
#define API_ERR_PARA_SAMID_NOT_IN_RANGE           0x0081
#define API_ERR_PARA_WPOS_NOT_IN_RANGE            0x0082
#define API_ERR_PARA_RANGE_NOT_IN_RANGE           0x0083
#define API_ERR_PARA_RTXFER_NOT_IN_RANGE          0x0084
#define API_ERR_WRONG_STREAM                      0x0085
#define API_ERR_IRIG_DAY_NOT_IN_RANGE             0x0086

#define API_ERR_SUBPARAM1_NOT_IN_RANGE            0x0100
#define API_ERR_SUBPARAM2_NOT_IN_RANGE            0x0101
#define API_ERR_SUBPARAM3_NOT_IN_RANGE            0x0102
#define API_ERR_SUBPARAM4_NOT_IN_RANGE            0x0103
#define API_ERR_SUBPARAM5_NOT_IN_RANGE            0x0104
#define API_ERR_SUBPARAM6_NOT_IN_RANGE            0x0105
#define API_ERR_SUBPARAM7_NOT_IN_RANGE            0x0106
#define API_ERR_SUBPARAM8_NOT_IN_RANGE            0x0107
#define API_ERR_SUBPARAM9_NOT_IN_RANGE            0x0108
#define API_ERR_SUBPARAM10_NOT_IN_RANGE           0x0109
#define API_ERR_SUBPARAM1_IS_NULL                 0x0180
#define API_ERR_SUBPARAM2_IS_NULL                 0x0181
#define API_ERR_SUBPARAM3_IS_NULL                 0x0182
#define API_ERR_SUBPARAM4_IS_NULL                 0x0183
#define API_ERR_SUBPARAM5_IS_NULL                 0x0184
#define API_ERR_SUBPARAM6_IS_NULL                 0x0185
#define API_ERR_SUBPARAM7_IS_NULL                 0x0186
#define API_ERR_SUBPARAM8_IS_NULL                 0x0187
#define API_ERR_SUBPARAM9_IS_NULL                 0x0188
#define API_ERR_SUBPARAM10_IS_NULL                0x0189
#define API_ERR_NO_MODULE_EXTENSION               0x018A
#define API_ERR_MALLOC_FAILED                     0x018B
#define API_ERR_OS_NOT_SUPPORTED                  0x018C
#define API_ERR_PLATTFORM_NOT_SUPPORTED           0x018D
#define API_ERR_FAILED_TO_CREATE_MUTEX            0x018E
#define API_ERR_INVALID_HANDLE                    0x018F
#define API_ERR_INVALID_TSW_VERSION               0x0190
#define API_ERR_NO_SCOPE_EXECUTED                 0x0191
#define API_ERR_CREATE_EVENT                      0x0192
#define API_ERR_NUMBUFFERSLEFT_INVALID            0x0193
#define API_ERR_DMA_NOT_SUPPORTED                 0x0194
#define API_ERR_TIMEOUT                           0x0195
#define API_ERR_OVERFLOW                          0x0196
#define API_ERR_BUFFER_NOT_ALLOCATED              0x0197
#define API_ERR_CON_NOT_ALLOWED                   0x0198
#define API_ERR_BUFFER_TOO_SMALL                  0x0199

/* -- ANS20 --- */
#define API_ERR_SERVER                            0x0200
#define API_ERR_SERVER_INVALID_VERSION            0x0201
#define API_ERR_SERVER_CONNECT                    0x0202



/* -- HS Error Codes (Started at 0x100 before ) --- */
#define API_ERR_HS_DYTAG_WMODE_NOT_IN_RANGE           0x0300
#define API_ERR_HS_DYTAG_MODE_NOT_IN_RANGE            0x0301
#define API_ERR_HS_DYTAG_WPOS_NOT_IN_RANGE            0x0302
#define API_ERR_HS_XFER_XFERID_NOT_IN_RANGE           0x0303
#define API_ERR_HS_XFER_HEADER_NOT_IN_RANGE           0x0304
#define API_ERR_HS_XFER_XFERTYPE_NOT_IN_RANGE         0x0305
#define API_ERR_HS_XFER_XFERHALT_NOT_IN_RANGE         0x0306
#define API_ERR_HS_XFER_MSGSIZE_NOT_IN_RANGE          0x0307
#define API_ERR_HS_XFER_RT_NOT_IN_RANGE               0x0308
#define API_ERR_HS_XFER_MID_NOT_IN_RANGE              0x0309
#define API_ERR_HS_XFER_SW_EXCEPT_NOT_IN_RANGE        0x030A
#define API_ERR_HS_XFER_FIRST_GAPMODE_NOT_IN_RANGE    0x030B
#define API_ERR_HS_XFER_FIRST_GAP_NOT_IN_RANGE        0x030C
#define API_ERR_HS_XFER_SECOND_GAPMODE_NOT_IN_RANGE   0x030D
#define API_ERR_HS_XFER_SECOND_GAP_NOT_IN_RANGE       0x030E
#define API_ERR_HS_LSCHN_NOT_IN_RANGE                 0x030F
#define API_ERR_HS_LSIR_NOT_IN_RANGE                  0x0310
#define API_ERR_HS_LSERR_TYPE_NOT_IN_RANGE            0x0311
#define API_ERR_HS_LSERR_CONTIG_NOT_IN_RANGE          0x0312
#define API_ERR_HS_LSERR_ERRSPEC_WPOS_NOT_IN_RANGE    0x0313
#define API_ERR_HS_LSERR_ERRSPEC_BPOS_NOT_IN_RANGE    0x0314
#define API_ERR_HS_LSERR_ERRSPEC_BCBITS_NOT_IN_RANGE  0x0315
#define API_ERR_HS_HSCHN_NOT_IN_RANGE                 0x0316
#define API_ERR_HS_HSIR_NOT_IN_RANGE                  0x0317
#define API_ERR_HS_HSXMT_NOPRE_NOT_IN_RANGE           0x0318
#define API_ERR_HS_HSXMT_STARTDEL_NOT_IN_RANGE        0x0319
#define API_ERR_HS_HSERR_TYPE_NOT_IN_RANGE            0x031A
#define API_ERR_HS_HSERR_ERRSPEC_WPOS_NOT_IN_RANGE    0x031B
#define API_ERR_HS_HSERR_ERRSPEC_BPOS_NOT_IN_RANGE    0x031C
#define API_ERR_HS_HSERR_ERRSPEC_BCBITS_NOT_IN_RANGE  0x031D
#define API_ERR_EF_WORDCOUNT_NOT_IN_RANGE             0x031E
#define API_ERR_HS_NO_SUCH_LIBRARY                    0x031F


/* -- Target --- */

#define API_TARGET_ERROR_OFFSET                   0x1000
#define API_ERR_CMD_NOT_FOUND                     0x1001
#define API_ERR_WRONG_CMD_SIZE                    0x1002
#define API_ERR_WRONG_ACK_SIZE                    0x1003
#define API_ERR_BIU_OUT_OF_RANGE                  0x1004
#define API_ERR_DMA_ALREADY_RUNNING               0x1005
#define API_ERR_DMA_WRITE_NOT_SUPPORTED           0x1006
#define API_ERR_DMA_INVALID_MEMORY                0x1007
#define API_ERR_DMA_TOO_MANY_BLOCKS               0x1008
#define API_ERR_DMA_DEST_INVALID                  0x1009
#define API_ERR_MEMORY_INVALID                    0x100A
#define API_ERR_NO_SPACE_LEFT                     0x100B
#define API_ERR_BIU_NOT_ACTIVE                    0x100C
#define API_ERR_LS_CMD_ON_HS_BIU                  0x100D
#define API_ERR_HS_CMD_ON_LS_BIU                  0x100E
#define API_ERR_INVALID_MODE                      0x100F
#define API_ERR_ADDRESS_OUT_OF_RANGE              0x1010
#define API_ERR_INVALID_DEVICE_STATE              0x1011
#define API_ERR_PARAMETER_OUT_OF_RANGE            0x1012
#define API_ERR_IOFPGA_BOOT_FAILED                0x1013
#define API_ERR_BIP1_BOOT_FAILED                  0x1014
#define API_ERR_BIP2_BOOT_FAILED                  0x1015
#define API_ERR_BIP1_START_FAILED                 0x1016
#define API_ERR_BIP2_START_FAILED                 0x1017
#define API_ERR_FUNCTION_NOT_SUPPORTED            0x1018
#define API_ERR_ELEMENT_NOT_ENABLED               0x1019
#define API_ERR_BC_SIMULATION_ACTIVE              0x101A
#define API_ERR_RT_SIMULATION_ACTIVE              0x101B
#define API_ERR_PRIMARY_COUPLING_NOT_INTERNAL     0x101C
#define API_ERR_SECONDARY_COUPLING_NOT_INTERNAL   0x101D
#define API_ERR_MYMON_ERROR                       0x101E
#define API_ERR_VALUE_NOT_IN_RANGE                0x101F
#define API_ERR_SCOPE_ALREADY_RUNNING             0x1020
#define API_ERR_SCOPE_DMA_ENABLED                 0x1021
#define API_ERR_SCOPE_DMA_STATUS_BUSY             0x1022
#define API_ERR_INVALID_KEY                       0x1023
#define API_ERR_TARGET_RESPONSE_TIMEOUT           0x1024
#define API_ERR_INVALID_DISCRETE                  0x1025
#define API_ERR_NOVRAM_ERROR                      0x1026
#define API_ERR_FOFE_OT_SHUTDOWN                  0x1027
#define API_ERR_INVALID_CON                       0x1028
#define API_ERR_INVALID_RT                        0x1029
#define API_ERR_INVALID_HID                       0x102A
#define API_ERR_INVALID_RT_SA                     0x102B
#define API_ERR_INVALID_RT_SA_TYPE                0x102C
#define API_ERR_INVALID_BUS                       0x102D
#define API_ERR_INVALID_ID                        0x102E
#define API_ERR_INVALID_SIZE                      0x102F
#define API_ERR_INVALID_TIME                      0x1030
#define API_ERR_CMD_NOT_SUPPORTED_BY_FW           0x1031
#define API_ERR_ACK_SIZE_UNDEFINED                0x1032
#define API_ERR_AGAIN                             0x1033
#define API_ERR_BC_NOT_AVAILABLE                  0x1034
#define API_ERR_RT_NOT_AVAILABLE                  0x1035
#define API_ERR_BM_NOT_AVAILABLE                  0x1036
#define API_ERR_REPLAY_NOT_AVAILABLE              0x1037
#define API_ERR_ERROR_INJECTION_NOT_AVAILABLE     0x1038
#define API_ERR_DWC_NOT_AVAILABLE                 0x1039




/* -- End Of Table */

#define API_ERR_MAXIMUM                           0xFFFF /* The return value of the API is AiInt16 */






/*********************************************************************************/
/*                                                                               */
/*                           Error message lookup table                          */
/*                                                                               */
/*********************************************************************************/


typedef struct {
   AiUInt32   ulNumber;
   const char * sZFileName;
} TY_MIL_ERR_TRANSLATE_TABLE;


#define DEFINE_AIMILERRORS_TABLE                                                                                                \
static const TY_MIL_ERR_TRANSLATE_TABLE axMilErrorDescription[] =                                                               \
{                                                                                                                               \
                                                                                                                                \
            {API_ERR                                    ,"General error"},                                                      \
            {API_ERR_ACK                                ,"Target incorrectly returned ACK without data"},                       \
            {API_ERR_ACK2                               ,"Target incorrectly returned ACK2 with data"},                         \
            {API_ERR_NAK                                ,"Target reports generic error"},                                       \
            {API_ERR_HOST_TO_TARGET                     ,"Host to target communication error"},                                 \
            {API_ERR_INVALID_DRIVER_HANDLE              ,"IOCTL error invalid driver handle"},                                  \
            {API_ERR_IOCTL_TIMEOUT                      ,"IOCTL timeout"},                                                      \
            {API_ERR_IOCTL_ERROR                        ,"IOCTL driver error"},                                                 \
            {API_ERR_DEVICE_NOT_FOUND                   ,"No device extension could be found for the specified module handle"}, \
            {API_ERR_WRONG_TSW_VERSION                  ,"Target software is to old to support this command"},                  \
            {API_ERR_WRONG_DRV_VERSION                  ,"System driver is to old to support this command "},                   \
            {API_ERR_UNKNOWN_COMMAND                    ,"Unknown command"},                                                    \
                                                                                                                                \
                                                                                                                                \
                                                                                                                                \
            {API_ERR_WRONG_MODULE                       ,"API_ERR_WRONG_MODULE" },                                              \
            {API_ERR_WRONG_DEVICE                       ,"API_ERR_WRONG_DEVICE"},                                               \
            {API_ERR_CNT_IS_ZERO                        ,"API_ERR_CNT_IS_ZERO"},                                                \
            {API_ERR_PARAM1_IS_NULL                     ,"API_ERR_PARAM1_IS_NULL"},                                             \
            {API_ERR_PARAM2_IS_NULL                     ,"API_ERR_PARAM2_IS_NULL"},                                             \
            {API_ERR_PARAM3_IS_NULL                     ,"API_ERR_PARAM3_IS_NULL"},                                             \
            {API_ERR_PARAM4_IS_NULL                     ,"API_ERR_PARAM4_IS_NULL"},                                             \
            {API_ERR_PARAM5_IS_NULL                     ,"API_ERR_PARAM5_IS_NULL"},                                             \
            {API_ERR_DEV_IS_NOT_CTX                     ,"API_ERR_DEV_IS_NOT_CTX"},                                             \
            {API_ERR_PARAM1_NOT_IN_RANGE                ,"API_ERR_PARAM1_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM2_NOT_IN_RANGE                ,"API_ERR_PARAM2_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM3_NOT_IN_RANGE                ,"API_ERR_PARAM3_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM4_NOT_IN_RANGE                ,"API_ERR_PARAM4_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM5_NOT_IN_RANGE                ,"API_ERR_PARAM5_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM6_NOT_IN_RANGE                ,"API_ERR_PARAM6_NOT_IN_RANGE"},                                        \
            {API_ERR_NO_CTX_FOUND                       ,"API_ERR_NO_CTX_FOUND"},                                               \
            {API_ERR_WRONG_BIU                          ,"API_ERR_WRONG_BIU"},                                                  \
            {API_ERR_PARAM6_IS_NULL                     ,"API_ERR_PARAM6_IS_NULL"},                                             \
            {API_ERR_PARAM7_IS_NULL                     ,"API_ERR_PARAM7_IS_NULL"},                                             \
            {API_ERR_PARAM8_IS_NULL                     ,"API_ERR_PARAM8_IS_NULL"},                                             \
            {API_ERR_PARAM9_IS_NULL                     ,"API_ERR_PARAM9_IS_NULL"},                                             \
            {API_ERR_PARAM10_IS_NULL                    ,"API_ERR_PARAM10_IS_NULL"},                                            \
            {API_ERR_PARAM11_IS_NULL                    ,"API_ERR_PARAM11_IS_NULL"},                                            \
            {API_ERR_PARAM12_IS_NULL                    ,"API_ERR_PARAM12_IS_NULL"},                                            \
            {API_ERR_PARAM13_IS_NULL                    ,"API_ERR_PARAM13_IS_NULL"},                                            \
            {API_ERR_PARAM14_IS_NULL                    ,"API_ERR_PARAM14_IS_NULL"},                                            \
            {API_ERR_PARAM7_NOT_IN_RANGE                ,"API_ERR_PARAM7_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM8_NOT_IN_RANGE                ,"API_ERR_PARAM8_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM9_NOT_IN_RANGE                ,"API_ERR_PARAM9_NOT_IN_RANGE"},                                        \
            {API_ERR_PARAM10_NOT_IN_RANGE               ,"API_ERR_PARAM10_NOT_IN_RANGE"},                                       \
            {API_ERR_PARAM11_NOT_IN_RANGE               ,"API_ERR_PARAM11_NOT_IN_RANGE"},                                       \
            {API_ERR_PARAM12_NOT_IN_RANGE               ,"API_ERR_PARAM12_NOT_IN_RANGE"},                                       \
            {API_ERR_PARAM13_NOT_IN_RANGE               ,"API_ERR_PARAM13_NOT_IN_RANGE"},                                       \
            {API_ERR_PARAM14_NOT_IN_RANGE               ,"API_ERR_PARAM14_NOT_IN_RANGE"},                                       \
            {API_ERR_XID_NOT_IN_RANGE                   ,"API_ERR_XID_NOT_IN_RANGE"},                                           \
            {API_ERR_HID_NOT_IN_RANGE                   ,"API_ERR_HID_NOT_IN_RANGE"},                                           \
            {API_ERR_TYPE_NOT_IN_RANGE                  ,"API_ERR_TYPE_NOT_IN_RANGE"},                                          \
            {API_ERR_CHN_NOT_IN_RANGE                   ,"API_ERR_CHN_NOT_IN_RANGE"},                                           \
            {API_ERR_XMT_RT_NOT_IN_RANGE                ,"API_ERR_XMT_RT_NOT_IN_RANGE"},                                        \
            {API_ERR_RCV_RT_NOT_IN_RANGE                ,"API_ERR_RCV_RT_NOT_IN_RANGE"},                                        \
            {API_ERR_XMT_SA_NOT_IN_RANGE                ,"API_ERR_XMT_SA_NOT_IN_RANGE"},                                        \
            {API_ERR_RCV_SA_NOT_IN_RANGE                ,"API_ERR_RCV_SA_NOT_IN_RANGE"},                                        \
            {API_ERR_WCNT_NOT_IN_RANGE                  ,"API_ERR_WCNT_NOT_IN_RANGE"},                                          \
            {API_ERR_TIC_NOT_IN_RANGE                   ,"API_ERR_TIC_NOT_IN_RANGE"},                                           \
            {API_ERR_HLT_NOT_IN_RANGE                   ,"API_ERR_HLT_NOT_IN_RANGE"},                                           \
            {API_ERR_SXH_NOT_IN_RANGE                   ,"API_ERR_SXH_NOT_IN_RANGE"},                                           \
            {API_ERR_RSP_NOT_IN_RANGE                   ,"API_ERR_RSP_NOT_IN_RANGE"},                                           \
            {API_ERR_GAP_MODE_NOT_IN_RANGE              ,"API_ERR_GAP_MODE_NOT_IN_RANGE"},                                      \
            {API_ERR_ERRTYPE_NOT_IN_RANGE               ,"API_ERR_ERRTYPE_NOT_IN_RANGE"},                                       \
            {API_ERR_ERRSPEC_WPOS_NOT_IN_RANGE          ,"API_ERR_ERRSPEC_WPOS_NOT_IN_RANGE"},                                  \
            {API_ERR_ERRSPEC_BPOS_NOT_IN_RANGE          ,"API_ERR_ERRSPEC_BPOS_NOT_IN_RANGE"},                                  \
            {API_ERR_ERRSPEC_BCBITS_NOT_IN_RANGE        ,"API_ERR_ERRSPEC_BCBITS_NOT_IN_RANGE"},                                \
            {API_ERR_ERRSPEC_CONTIG_NOT_IN_RANGE        ,"API_ERR_ERRSPEC_CONTIG_NOT_IN_RANGE"},                                \
            {API_ERR_GAP_NOT_IN_RANGE                   ,"API_ERR_GAP_NOT_IN_RANGE"},                                           \
            {API_ERR_BCDYTAG_TAGFCT_NOT_IN_RANGE        ,"API_ERR_BCDYTAG_TAGFCT_NOT_IN_RANGE"},                                \
            {API_ERR_BCDYTAG_MIN_NOT_IN_RANGE           ,"API_ERR_BCDYTAG_MIN_NOT_IN_RANGE"},                                   \
            {API_ERR_BCDYTAG_MAX_NOT_IN_RANGE           ,"API_ERR_BCDYTAG_MAX_NOT_IN_RANGE"},                                   \
            {API_ERR_BCDYTAG_STEP_NOT_IN_RANGE          ,"API_ERR_BCDYTAG_STEP_NOT_IN_RANGE"},                                  \
            {API_ERR_BCDYTAG_WPOS_NOT_IN_RANGE          ,"API_ERR_BCDYTAG_WPOS_NOT_IN_RANGE"},                                  \
            {API_ERR_FRAME_ID_NOT_IN_RANGE              ,"API_ERR_FRAME_ID_NOT_IN_RANGE"},                                      \
            {API_ERR_FRAME_CNT_NOT_IN_RANGE             ,"API_ERR_FRAME_CNT_NOT_IN_RANGE"},                                     \
            {API_ERR_FRAME_INSTR_NOT_IN_RANGE           ,"API_ERR_FRAME_INSTR_NOT_IN_RANGE"},                                   \
            {API_ERR_FRAME_XID_NOT_IN_RANGE             ,"API_ERR_FRAME_XID_NOT_IN_RANGE"},                                     \
            {API_ERR_FRAME_FID_NOT_IN_RANGE             ,"API_ERR_FRAME_FID_NOT_IN_RANGE"},                                     \
            {API_ERR_CAPMODE_NOT_IN_RANGE               ,"API_ERR_CAPMODE_NOT_IN_RANGE"},                                       \
            {API_ERR_CAPFSIZE_NOT_IN_RANGE              ,"API_ERR_CAPFSIZE_NOT_IN_RANGE"},                                      \
            {API_ERR_TCBTT_NOT_IN_RANGE                 ,"API_ERR_TCBTT_NOT_IN_RANGE"},                                         \
            {API_ERR_TCBSOT_NOT_IN_RANGE                ,"API_ERR_TCBSOT_NOT_IN_RANGE"},                                        \
            {API_ERR_TCBTRI_NOT_IN_RANGE                ,"API_ERR_TCBTRI_NOT_IN_RANGE"},                                        \
            {API_ERR_TCBINV_NOT_IN_RANGE                ,"API_ERR_TCBINV_NOT_IN_RANGE"},                                        \
            {API_ERR_TCBNEXT_NOT_IN_RANGE               ,"API_ERR_TCBNEXT_NOT_IN_RANGE"},                                       \
            {API_ERR_TCBEOM_NOT_IN_RANGE                ,"API_ERR_TCBEOM_NOT_IN_RANGE"},                                        \
            {API_ERR_POS1_NOT_IN_RANGE                  ,"API_ERR_POS1_NOT_IN_RANGE"},                                          \
            {API_ERR_POS2_NOT_IN_RANGE                  ,"API_ERR_POS2_NOT_IN_RANGE"},                                          \
            {API_ERR_POS1_GREATER_THAN_POS2             ,"API_ERR_POS1_GREATER_THAN_POS2"},                                     \
            {API_ERR_ADDR_NOT_VALID                     ,"API_ERR_ADDR_NOT_VALID"},                                             \
            {API_ERR_RTDYTAG_TAGFCT_NOT_IN_RANGE        ,"API_ERR_RTDYTAG_TAGFCT_NOT_IN_RANGE"},                                \
            {API_ERR_RTDYTAG_MIN_NOT_IN_RANGE           ,"API_ERR_RTDYTAG_MIN_NOT_IN_RANGE"},                                   \
            {API_ERR_RTDYTAG_MAX_NOT_IN_RANGE           ,"API_ERR_RTDYTAG_MAX_NOT_IN_RANGE"},                                   \
            {API_ERR_RTDYTAG_STEP_NOT_IN_RANGE          ,"API_ERR_RTDYTAG_STEP_NOT_IN_RANGE"},                                  \
            {API_ERR_RTDYTAG_WPOS_NOT_IN_RANGE          ,"API_ERR_RTDYTAG_WPOS_NOT_IN_RANGE"},                                  \
            {API_ERR_IRIG_HOUR_NOT_IN_RANGE             ,"API_ERR_IRIG_HOUR_NOT_IN_RANGE"},                                     \
            {API_ERR_IRIG_MIN_NOT_IN_RANGE              ,"API_ERR_IRIG_MIN_NOT_IN_RANGE"},                                      \
            {API_ERR_IRIG_SEC_NOT_IN_RANGE              ,"API_ERR_IRIG_SEC_NOT_IN_RANGE"},                                      \
            {API_ERR_SYSTAG_XIDRTMID_NOT_IN_RANGE       ,"API_ERR_SYSTAG_XIDRTMID_NOT_IN_RANGE"},                               \
            {API_ERR_SYSTAG_FCT_NOT_IN_RANGE            ,"API_ERR_SYSTAG_FCT_NOT_IN_RANGE"},                                    \
            {API_ERR_SYSTAG_MIN_NOT_IN_RANGE            ,"API_ERR_SYSTAG_MIN_NOT_IN_RANGE"},                                    \
            {API_ERR_SYSTAG_MAX_NOT_IN_RANGE            ,"API_ERR_SYSTAG_MAX_NOT_IN_RANGE"},                                    \
            {API_ERR_SYSTAG_STEP_NOT_IN_RANGE           ,"API_ERR_SYSTAG_STEP_NOT_IN_RANGE"},                                   \
            {API_ERR_SYSTAG_WPOS_NOT_IN_RANGE           ,"API_ERR_SYSTAG_WPOS_NOT_IN_RANGE"},                                   \
            {API_ERR_SYSTAG_BPOS_NOT_IN_RANGE           ,"API_ERR_SYSTAG_BPOS_NOT_IN_RANGE"},                                   \
            {API_ERR_QUEUE_EMPTY                        ,"API_ERR_QUEUE_EMPTY"},                                                \
            {API_ERR_QUEUE_OVERFLOW                     ,"API_ERR_QUEUE_OVERFLOW"},                                             \
            {API_ERR_1760_MODE_NOT_IN_RANGE             ,"API_ERR_1760_MODE_NOT_IN_RANGE"},                                     \
            {API_ERR_1760_BUFID_NOT_IN_RANGE            ,"API_ERR_1760_BUFID_NOT_IN_RANGE"},                                    \
            {API_ERR_1760_C01_NOT_IN_RANGE              ,"API_ERR_1760_C01_NOT_IN_RANGE"},                                      \
            {API_ERR_1760_C02_NOT_IN_RANGE              ,"API_ERR_1760_C02_NOT_IN_RANGE"},                                      \
            {API_ERR_DQUEUE_ID_NOT_IN_RANGE             ,"API_ERR_DQUEUE_ID_NOT_IN_RANGE"},                                     \
            {API_ERR_DQUEUE_LOC_BUFTYPE_NOT_IN_RANGE    ,"API_ERR_DQUEUE_LOC_BUFTYPE_NOT_IN_RANGE"},                            \
            {API_ERR_DQUEUE_REM_BUFTYPE_NOT_IN_RANGE    ,"API_ERR_DQUEUE_REM_BUFTYPE_NOT_IN_RANGE"},                            \
            {API_ERR_DQUEUE_LOC_MEMSIZE_NOT_IN_RANGE    ,"API_ERR_DQUEUE_LOC_MEMSIZE_NOT_IN_RANGE"},                            \
            {API_ERR_DQUEUE_REM_MEMSIZE_NOT_IN_RANGE    ,"API_ERR_DQUEUE_REM_MEMSIZE_NOT_IN_RANGE"},                            \
            {API_ERR_DQUEUE_LOC_HANDLE_NOT_IN_RANGE     ,"API_ERR_DQUEUE_LOC_HANDLE_NOT_IN_RANGE"},                             \
            {API_ERR_DQUEUE_REM_HANDLE_NOT_IN_RANGE     ,"API_ERR_DQUEUE_REM_HANDLE_NOT_IN_RANGE"},                             \
            {API_ERR_DQUEUE_ASP_SIZE_NOT_IN_RANGE       ,"API_ERR_DQUEUE_ASP_SIZE_NOT_IN_RANGE"},                               \
            {API_ERR_DQUEUE_CONTROL_MODE_NOT_IN_RANGE   ,"API_ERR_DQUEUE_CONTROL_MODE_NOT_IN_RANGE"},                           \
            {API_ERR_DQUEUE_READ_NOT_BUFFERED           ,"API_ERR_DQUEUE_READ_NOT_BUFFERED"},                                   \
            {API_ERR_DQUEUE_ASP_SIZE_TOO_BIG            ,"API_ERR_DQUEUE_ASP_SIZE_TOO_BIG"},                                    \
            {API_ERR_DQUEUE_ASP_OPEN                    ,"API_ERR_DQUEUE_ASP_OPEN"},                                            \
            {API_ERR_PARA_ID_NOT_IN_RANGE               ,"API_ERR_PARA_ID_NOT_IN_RANGE"},                                       \
            {API_ERR_PARA_MODE_NOT_IN_RANGE             ,"API_ERR_PARA_MODE_NOT_IN_RANGE"},                                     \
            {API_ERR_PARA_BUF_NOT_IN_RANGE              ,"API_ERR_PARA_BUF_NOT_IN_RANGE"},                                      \
            {API_ERR_PARA_TYPE_NOT_IN_RANGE             ,"API_ERR_PARA_TYPE_NOT_IN_RANGE"},                                     \
            {API_ERR_PARA_SAMID_NOT_IN_RANGE            ,"API_ERR_PARA_SAMID_NOT_IN_RANGE"},                                    \
            {API_ERR_PARA_WPOS_NOT_IN_RANGE             ,"API_ERR_PARA_WPOS_NOT_IN_RANGE"},                                     \
            {API_ERR_PARA_RANGE_NOT_IN_RANGE            ,"API_ERR_PARA_RANGE_NOT_IN_RANGE"},                                    \
            {API_ERR_PARA_RTXFER_NOT_IN_RANGE           ,"API_ERR_PARA_RTXFER_NOT_IN_RANGE"},                                   \
            {API_ERR_WRONG_STREAM                       ,"API_ERR_WRONG_STREAM"},                                               \
            {API_ERR_IRIG_DAY_NOT_IN_RANGE              ,"Invalid irig day"},                                                   \
            {API_ERR_SUBPARAM1_NOT_IN_RANGE             ,"API_ERR_SUBPARAM1_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM2_NOT_IN_RANGE             ,"API_ERR_SUBPARAM2_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM3_NOT_IN_RANGE             ,"API_ERR_SUBPARAM3_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM4_NOT_IN_RANGE             ,"API_ERR_SUBPARAM4_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM5_NOT_IN_RANGE             ,"API_ERR_SUBPARAM5_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM6_NOT_IN_RANGE             ,"API_ERR_SUBPARAM6_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM7_NOT_IN_RANGE             ,"API_ERR_SUBPARAM7_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM8_NOT_IN_RANGE             ,"API_ERR_SUBPARAM8_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM9_NOT_IN_RANGE             ,"API_ERR_SUBPARAM9_NOT_IN_RANGE"},                                     \
            {API_ERR_SUBPARAM10_NOT_IN_RANGE            ,"API_ERR_SUBPARAM10_NOT_IN_RANGE"},                                    \
            {API_ERR_SUBPARAM1_IS_NULL                  ,"API_ERR_SUBPARAM1_IS_NULL"},                                          \
            {API_ERR_SUBPARAM2_IS_NULL                  ,"API_ERR_SUBPARAM2_IS_NULL"},                                          \
            {API_ERR_SUBPARAM3_IS_NULL                  ,"API_ERR_SUBPARAM3_IS_NULL"},                                          \
            {API_ERR_SUBPARAM4_IS_NULL                  ,"API_ERR_SUBPARAM4_IS_NULL"},                                          \
            {API_ERR_SUBPARAM5_IS_NULL                  ,"API_ERR_SUBPARAM5_IS_NULL"},                                          \
            {API_ERR_SUBPARAM6_IS_NULL                  ,"API_ERR_SUBPARAM6_IS_NULL"},                                          \
            {API_ERR_SUBPARAM7_IS_NULL                  ,"API_ERR_SUBPARAM7_IS_NULL"},                                          \
            {API_ERR_SUBPARAM8_IS_NULL                  ,"API_ERR_SUBPARAM8_IS_NULL"},                                          \
            {API_ERR_SUBPARAM9_IS_NULL                  ,"API_ERR_SUBPARAM9_IS_NULL"},                                          \
            {API_ERR_SUBPARAM10_IS_NULL                 ,"API_ERR_SUBPARAM10_IS_NULL"},                                         \
            {API_ERR_NO_MODULE_EXTENSION                ,"Failed to get module extension"},                                     \
            {API_ERR_MALLOC_FAILED                      ,"Memory allocation failed"},                                           \
            {API_ERR_OS_NOT_SUPPORTED                   ,"The specified command is not supported on this Operating System"},    \
            {API_ERR_PLATTFORM_NOT_SUPPORTED            ,"The specified command is not supported on this plattform"},           \
            {API_ERR_FAILED_TO_CREATE_MUTEX             ,"Failed to create Mutex"},                                             \
            {API_ERR_INVALID_HANDLE                     ,"Invalid handle"},                                                     \
            {API_ERR_INVALID_TSW_VERSION                ,"The selected mode is not supported in the installed TSW version"},    \
            {API_ERR_NO_SCOPE_EXECUTED                  ,"Scope setup has not been executed on this board"},                    \
            {API_ERR_CREATE_EVENT                       ,"Creation of Event failed"},                                           \
            {API_ERR_NUMBUFFERSLEFT_INVALID             ,"Inavlid number of scope buffers provided"},                           \
            {API_ERR_DMA_NOT_SUPPORTED                  ,"DMA not supported on this device"},                                   \
            {API_ERR_TIMEOUT                            ,"Timeout occurred"},                                                   \
            {API_ERR_OVERFLOW                           ,"Overflow occurred"},                                                  \
            {API_ERR_BUFFER_NOT_ALLOCATED               ,"Buffer pointer is NULL"},                                             \
            {API_ERR_CON_NOT_ALLOWED                    ,"Con parameter value not allowed"},                                    \
            {API_ERR_BUFFER_TOO_SMALL                   ,"Buffer too small"},                                                   \
                                                                                                                                \
            {API_ERR_SERVER                             ,"Error in server communication"},                                      \
            {API_ERR_SERVER_INVALID_VERSION             ,"Error invalid server version"},                                       \
            {API_ERR_SERVER_CONNECT                     ,"Error in connecting to socket"},                                      \
                                                                                                                                \
            {API_ERR_HS_DYTAG_WMODE_NOT_IN_RANGE           ,"HS dyntag wmode not in range"},                                    \
            {API_ERR_HS_DYTAG_MODE_NOT_IN_RANGE            ,"HS dyntag mode not in range"},                                     \
            {API_ERR_HS_DYTAG_WPOS_NOT_IN_RANGE            ,"HS dyntag wpos not in range"},                                     \
            {API_ERR_HS_XFER_XFERID_NOT_IN_RANGE           ,"HS xfer id not in range"},                                         \
            {API_ERR_HS_XFER_HEADER_NOT_IN_RANGE           ,"HS xfer header id not in range"},                                  \
            {API_ERR_HS_XFER_XFERTYPE_NOT_IN_RANGE         ,"HS xfer type not in range"},                                       \
            {API_ERR_HS_XFER_XFERHALT_NOT_IN_RANGE         ,"HS xfer halt not in range"},                                       \
            {API_ERR_HS_XFER_MSGSIZE_NOT_IN_RANGE          ,"HS message size not in range"},                                    \
            {API_ERR_HS_XFER_RT_NOT_IN_RANGE               ,"HS xfer RT not in range"},                                         \
            {API_ERR_HS_XFER_MID_NOT_IN_RANGE              ,"HS xfer MID not in range"},                                        \
            {API_ERR_HS_XFER_SW_EXCEPT_NOT_IN_RANGE        ,"HS xfer sw except not in range"},                                  \
            {API_ERR_HS_XFER_FIRST_GAPMODE_NOT_IN_RANGE    ,"HS xfer first gapmode not in range"},                              \
            {API_ERR_HS_XFER_FIRST_GAP_NOT_IN_RANGE        ,"HS xfer first gap not in range"},                                  \
            {API_ERR_HS_XFER_SECOND_GAPMODE_NOT_IN_RANGE   ,"HS xfer second gapmode not in range"},                             \
            {API_ERR_HS_XFER_SECOND_GAP_NOT_IN_RANGE       ,"HS xfer second gap not in range"},                                 \
            {API_ERR_HS_LSCHN_NOT_IN_RANGE                 ,"HS ls chn not in range"},                                          \
            {API_ERR_HS_LSIR_NOT_IN_RANGE                  ,"HS lsir not in range"},                                            \
            {API_ERR_HS_LSERR_TYPE_NOT_IN_RANGE            ,"HS ls err type not in range"},                                     \
            {API_ERR_HS_LSERR_CONTIG_NOT_IN_RANGE          ,"HS ls err contig not in range"},                                   \
            {API_ERR_HS_LSERR_ERRSPEC_WPOS_NOT_IN_RANGE    ,"HS ls err errsepec wpos not in range"},                            \
            {API_ERR_HS_LSERR_ERRSPEC_BPOS_NOT_IN_RANGE    ,"HS ls err errspec bpos not in range"},                             \
            {API_ERR_HS_LSERR_ERRSPEC_BCBITS_NOT_IN_RANGE  ,"HS ls err errspec bcbits not in range"},                           \
            {API_ERR_HS_HSCHN_NOT_IN_RANGE                 ,"HS hs chn not in range"},                                          \
            {API_ERR_HS_HSIR_NOT_IN_RANGE                  ,"HS ir not in range"},                                              \
            {API_ERR_HS_HSXMT_NOPRE_NOT_IN_RANGE           ,"HS xmt nopre not in range"},                                       \
            {API_ERR_HS_HSXMT_STARTDEL_NOT_IN_RANGE        ,"HS xmt startdel not in range"},                                    \
            {API_ERR_HS_HSERR_TYPE_NOT_IN_RANGE            ,"HS err type not in range"},                                        \
            {API_ERR_HS_HSERR_ERRSPEC_WPOS_NOT_IN_RANGE    ,"HS errspec wpos not in range"},                                    \
            {API_ERR_HS_HSERR_ERRSPEC_BPOS_NOT_IN_RANGE    ,"HS errspec bpos not in range"},                                    \
            {API_ERR_HS_HSERR_ERRSPEC_BCBITS_NOT_IN_RANGE  ,"HS errspec bcbits not in range"},                                  \
            {API_ERR_EF_WORDCOUNT_NOT_IN_RANGE             ,"EF wordcount not in range"},                                       \
            {API_ERR_HS_NO_SUCH_LIBRARY                    ,"HS command on non HS library called"},                             \
                                                                                                                                \
            {API_ERR_CMD_NOT_FOUND                      ,"Target command not found"},                                           \
            {API_ERR_WRONG_CMD_SIZE                     ,"There was not enough data provided for the command"},                 \
            {API_ERR_WRONG_ACK_SIZE                     ,"Output buffer size to small"},                                        \
            {API_ERR_BIU_OUT_OF_RANGE                   ,"The specified biu was not in range"},                                 \
            {API_ERR_DMA_ALREADY_RUNNING                ,"DMA engine is already running"},                                      \
            {API_ERR_DMA_WRITE_NOT_SUPPORTED            ,"DMA direction write not supported"},                                  \
            {API_ERR_DMA_INVALID_MEMORY                 ,"DMA memory is invalid"},                                              \
            {API_ERR_DMA_TOO_MANY_BLOCKS                ,"DMA too many blocks"},                                                \
            {API_ERR_DMA_DEST_INVALID                   ,"DMA invalid destination address"},                                    \
            {API_ERR_MEMORY_INVALID                     ,"Not supported memory type"},                                          \
            {API_ERR_NO_SPACE_LEFT                      ,"No space left on device"},                                            \
            {API_ERR_BIU_NOT_ACTIVE                     ,"The specified BIU is not active"},                                    \
            {API_ERR_LS_CMD_ON_HS_BIU                   ,"Tried to executing LS command on HS BIU"},                            \
            {API_ERR_HS_CMD_ON_LS_BIU                   ,"Tried to executing HS command on LS BIU"},                            \
            {API_ERR_INVALID_MODE                       ,"Invalid mode"},                                                       \
            {API_ERR_ADDRESS_OUT_OF_RANGE               ,"Address out of range"},                                               \
            {API_ERR_INVALID_DEVICE_STATE               ,"Invalid device state"},                                               \
            {API_ERR_PARAMETER_OUT_OF_RANGE             ,"Parameter out of range"},                                             \
            {API_ERR_IOFPGA_BOOT_FAILED                 ,"IO FPGA boot failed"},                                                \
            {API_ERR_BIP1_BOOT_FAILED                   ,"BIP1 boot failed"},                                                   \
            {API_ERR_BIP2_BOOT_FAILED                   ,"BIP2 boot failed"},                                                   \
            {API_ERR_BIP1_START_FAILED                  ,"BIP1 start failed"},                                                  \
            {API_ERR_BIP2_START_FAILED                  ,"BIP2 start failed"},                                                  \
            {API_ERR_FUNCTION_NOT_SUPPORTED             ,"Function not supported on this device"},                              \
            {API_ERR_ELEMENT_NOT_ENABLED                ,"Element not enabled"},                                                \
            {API_ERR_BC_SIMULATION_ACTIVE               ,"BC simulation is active"},                                            \
            {API_ERR_RT_SIMULATION_ACTIVE               ,"RT simulation is active"},                                            \
            {API_ERR_PRIMARY_COUPLING_NOT_INTERNAL      ,"Primary coupling is not internal"},                                   \
            {API_ERR_SECONDARY_COUPLING_NOT_INTERNAL    ,"Secondary coupling is not internal"},                                 \
            {API_ERR_MYMON_ERROR                        ,"Error in MyMON communication"},                                       \
            {API_ERR_VALUE_NOT_IN_RANGE                 ,"Error calibrated value out of range"},                                \
            {API_ERR_SCOPE_ALREADY_RUNNING              ,"The MilScope can not be started because it is already running"},      \
            {API_ERR_SCOPE_DMA_ENABLED                  ,"The MilScope can not be started because the DMA is enabled"},         \
            {API_ERR_SCOPE_DMA_STATUS_BUSY              ,"The MilScope can not be started because the DMA is busy"},            \
            {API_ERR_INVALID_KEY                        ,"The specified key was not valid"},                                    \
            {API_ERR_TARGET_RESPONSE_TIMEOUT            ,"Target did not respond in time"},                                     \
            {API_ERR_INVALID_DISCRETE                   ,"Invalid discrete channel for this device"},                           \
            {API_ERR_NOVRAM_ERROR                       ,"Failed to access NOVRAM"},                                            \
            {API_ERR_FOFE_OT_SHUTDOWN                   ,"The FOFE was turned off due to a over-temperature alert"},            \
            {API_ERR_INVALID_CON                        ,"Invalid con parameter"},                                              \
            {API_ERR_INVALID_RT                         ,"Invalid RT parameter"},                                               \
            {API_ERR_INVALID_HID                        ,"Invalid HID parameter"},                                              \
            {API_ERR_INVALID_RT_SA                      ,"Invalid RT SA parameter"},                                            \
            {API_ERR_INVALID_RT_SA_TYPE                 ,"Invalid RT SA type parameter"},                                       \
            {API_ERR_INVALID_BUS                        ,"Invalid bus parameter"},                                              \
            {API_ERR_INVALID_ID                         ,"Invalid id parameter"},                                               \
            {API_ERR_INVALID_SIZE                       ,"Invalid size"},                                                       \
            {API_ERR_INVALID_TIME                       ,"Invalid time"},                                                       \
            {API_ERR_CMD_NOT_SUPPORTED_BY_FW            ,"The firmware does not support this command"},                         \
            {API_ERR_ACK_SIZE_UNDEFINED                 ,"The function did not set a valid acknowledge size"},                  \
            {API_ERR_AGAIN                              ,"Try again later"},                                                    \
            {API_ERR_BC_NOT_AVAILABLE                   ,"BC not available"},                                                   \
            {API_ERR_RT_NOT_AVAILABLE                   ,"RT not available"},                                                   \
            {API_ERR_BM_NOT_AVAILABLE                   ,"BM not available"},                                                   \
            {API_ERR_REPLAY_NOT_AVAILABLE               ,"Replay not available"},                                               \
            {API_ERR_ERROR_INJECTION_NOT_AVAILABLE      ,"Error injection not available"},                                      \
            {API_ERR_DWC_NOT_AVAILABLE                  ,"Defined Word Count Check not available"},                             \
                                                                                                                                \
                                                                                                                                \
            {API_ERR_MAXIMUM                            ,"No description available for this error code"},                       \
            {0,0}                                                                                                               \
};







#endif /* __AIMILERRORS_H__ */

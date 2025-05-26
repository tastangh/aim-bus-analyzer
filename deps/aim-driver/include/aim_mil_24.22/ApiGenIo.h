/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiGenIo.h
 *
 *  This header file contains declarations for
 *  GenIo API functionality
 *
 */


#ifndef API_GEN_IO_H
#define API_GEN_IO_H


/************/
/* defines  */
/************/





/**************************/
/* structs and typedefs   */
/**************************/

typedef struct ty_api_gen_io_dev_ven
{
    AiUInt32 ul_DeviceVendor;
    AiUInt32 ul_SubSystemSubVendor;
} TY_API_GEN_IO_DEV_VEN;

typedef struct ty_api_gen_io_dev_ven_list
{
    AiUInt32 ul_Cnt;
    TY_API_GEN_IO_DEV_VEN *px_DevVen;
} TY_API_GEN_IO_DEV_VEN_LIST;

typedef struct ty_api_gen_io_board_type
{
    AiUInt32 ul_BoardType;
    AiUInt32 ul_Type;
} TY_API_GEN_IO_BOARD_TYPE;

typedef struct ty_api_gen_io_board_type_list
{
    AiUInt32 ul_Cnt;
    TY_API_GEN_IO_BOARD_TYPE *px_BoardTypes;
} TY_API_GEN_IO_BOARD_TYPE_LIST;

typedef struct ty_api_gen_io_get_boards_in
{
    TY_API_GEN_IO_DEV_VEN_LIST*     px_DevVenList;
    TY_API_GEN_IO_BOARD_TYPE_LIST*  px_BoardTypesList;
} TY_API_GEN_IO_GET_BOARDS_IN;

typedef struct ty_api_gen_io_board
{
    AiUInt32 ul_Board;   /* e.g. APX, DIG, ANA, RS */ 
    AiUInt32 ul_Type;       /* e.g. APX, 408, 470 */
    AiUInt32 ul_StartAddress;
} TY_API_GEN_IO_BOARD;

typedef struct ty_api_gen_io_get_board_out
{
    AiUInt32 ul_Cnt;
    TY_API_GEN_IO_BOARD* px_Board;
} TY_API_GEN_IO_GET_BOARDS_OUT;



/***********************/
/* interface functions */
/***********************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoGetBoards    (AiUInt32 ul_Module, TY_API_GEN_IO_GET_BOARDS_IN *px_InBoards, TY_API_GEN_IO_GET_BOARDS_OUT *px_OutBoards );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoAddrInit     (AiUInt32 ul_Module, AiUInt8 genio_type, AiUInt8 io_type, AiUInt8 chn, AiUInt8 res, AiUInt32 addr);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoOutChnWrite  (AiUInt32 ul_Module, AiUInt8 genio_type, AiUInt8 chn, AiUInt32 res, AiUInt32 val);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoInChnRead    (AiUInt32 ul_Module, AiUInt8 genio_type, AiUInt8 chn, AiUInt32 ctl, AiUInt32 *val);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoSysSTaskCtrl (AiUInt32 ul_Module, AiUInt8 tmr, AiUInt8 con, AiUInt32 rate, AiUInt32 rsync, AiUInt32 ustep);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoRead         (AiUInt32 ul_Module, AiUInt64 ull_Addr, AiUInt32 ul_Size, void * vp_Data, AiInt16 *pw_Read);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoOutChnRead   (AiUInt32 ul_Module, AiUInt8 genio_type, AiUInt8 chn, AiUInt32 ctl, AiUInt32 *val);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdGenIoTestSeq      (AiUInt32 ul_Module, AiUInt32 ul_Con, TY_API_GENIO_TEST_SEQ_TBL *pxSeqTbl, AiUInt8 *auc_SeqStatus);


#endif


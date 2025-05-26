/* SPDX-FileCopyrightText: 2001-2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Ai3910i_fnc.h
 *
 *  Function prototypes for the 3910 Application Interface. 
 *
 */

#ifndef AI3910I_FNC_H
#define AI3910I_FNC_H






/***************************************************************************
 **  API3910 Functions - External References
 ***************************************************************************/
#if !defined(NO_PROTOTYPE_INCLUDE)
/* System Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsReset          (AiUInt32 bModule, AiUInt8 uc_ResCtrl,
                                                              AiUInt8 uc_MBufSize, TY_API_HS_RESET_INFO *px_ResInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsResetInfo      (AiUInt32 bModule, TY_API_HS_RESET_INFO *px_ResInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRxInitTimeOutDef( AiUInt32 bModule, AiUInt16 uw_TimeOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsReadRecData       (AiUInt32 bModule, TY_API_HS_BM_REC *px_ApiRecStat,
                                                              void *pl_Buf, AiUInt32 *pul_BytesRead);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsWriteRepData      (AiUInt32 bModule,  TY_API_HS_REP_STATUS *px_ApiRepStat,
                                                              void *pl_Buf, AiUInt32 *pul_BytesWritten );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsSystagDef      (AiUInt32 bModule, AiUInt8 uc_Id, AiUInt8 uc_Con, 
                                                              AiUInt8 uc_Mode, TY_API_HS_SYSTAG *px_Systag );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsSystagCon      ( AiUInt32 bModule, AiUInt8 uc_Id, AiUInt8 uc_Con );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsSetFofeDiscretes(AiUInt32 bModule, AiUInt8 uc_BitEnable,
                                                               AiUInt8 uc_BitCon1, AiUInt8 uc_BitCon2 );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackDef       (AiUInt32 bModule, AiUInt8 uc_TrackId, AiUInt8 uc_BT,
                                                              AiUInt16 uw_XidRtMid, AiUInt8 uc_Mode, AiUInt16 uw_TrackStartPos,
                                                              AiUInt8 uc_TrackBPos, AiUInt8 uc_TrackBLen, AiUInt16 uw_TrackLen,
                                                              AiUInt16 uw_MultiplexedTrackNb, AiUInt16 uw_ContinousTrackNb,
                                                              AiUInt32 *pul_TrackBufferStartAddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackDefEx     (AiUInt32 bModule, 
                                                              TY_API_HS_TRACK_DEF_IN  *px_TrackDefIn,
                                                              TY_API_HS_TRACK_DEF_OUT *px_TrackDefOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackRead      (AiUInt32 bModule, AiUInt8 uc_TrackId, AiUInt16 uw_MultiplexedTrackIndex,
                                                              AiUInt8 uc_ReadMode, AiUInt8 *puc_DataValid, AiUInt16 *puw_TrackDataWords);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackReadEx    (AiUInt32 bModule, 
                                                              TY_API_HS_TRACK_READ_IN *px_TrackReadIn,
                                                              TY_API_HS_TRACK_READ_OUT *px_TrackReadOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackScan      (AiUInt32 bModule, 
                                                              TY_API_HS_TRACK_SCAN_IN *px_TrackScanIn,
                                                              TY_API_HS_TRACK_SCAN_OUT *px_TrackScanOut );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsTrackPreAlloc  (AiUInt32 bModule, 
                                                              TY_API_HS_TRACK_PREALLOC_IN *px_TrackPreAllocIn,
                                                              TY_API_HS_TRACK_PREALLOC_OUT *px_TrackPreAllocOut );

/* Buffer Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBufDef         (AiUInt32 bModule, AiUInt8 uc_BufType, AiUInt16 uw_HeaderId,
                                                              AiUInt16 uw_BufferId, AiUInt16 uw_BufSize, AiUInt16 uw_WStartIndex, AiUInt16 uw_WriteSize,
                                                              AiUInt16 *puw_Data, AiUInt16 *puw_AllocBufNr, AiUInt16 *puw_AllocInfo, AiUInt32 *pul_BufAddr );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBufHostControl(AiUInt32 bModule,
                                                                AiUInt8 uc_BufType,
                                                                AiUInt16 uw_HeaderId,
                                                                AiUInt32 ul_Con);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBufRead        (AiUInt32 bModule, AiUInt8 uc_BufType, AiUInt16 uw_HeaderId, 
                                                              AiUInt16 uw_BufferId, AiUInt16 uw_RStartIndex, AiUInt16 uw_ReadSize,
                                                              AiUInt16 *puw_BufNr, AiUInt32 *pul_BufAddr, AiUInt16 *puw_Data );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBufWrite       (AiUInt32 bModule, AiUInt8 uc_BufType, AiUInt16 uw_HeaderId,
                                                              AiUInt16 uw_BufferId, AiUInt16 uw_DataPos, AiUInt16 uw_DataWordMask, AiUInt16 uw_DataWord);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBufWriteBlock(AiUInt32 bModule,
                                                            AiUInt8 uc_BufType, AiUInt16 uw_HeaderId,
                                                            AiUInt16 uw_BufferId, AiUInt16 uw_WStartIndex,
                                                            AiUInt16 uw_WriteSize, AiUInt16* puw_Data);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsBHModify          (AiUInt32 bModule, TY_API_HS_BH_MODIFY *px_BHInfo);

/* Bus Controller Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCBHDef        (AiUInt32 bModule, AiUInt16 uw_HeaderId,
                                                                                                      AiUInt16 uw_Res, AiUInt16 uw_SqId,
                                                                                                      AiUInt16 uw_EqId, AiUInt8 uc_QSize,
                                                                                                      AiUInt8 uc_Bqm, AiUInt8 uc_Bsm,
                                                                                                      AiUInt8 uc_Sqm, AiUInt8 uc_Eqm,
                                                                                                      AiUInt8 uc_Dbm, AiUInt8 uc_Wcm );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCBHRead       (AiUInt32 bModule, AiUInt16 uw_HeaderId,
                                                              TY_API_HS_BC_BH_INFO *px_BufHeader );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCXferDef      (AiUInt32 bModule, TY_API_HS_BC_XFER *px_Xfer );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCXferDefErr   (AiUInt32 bModule, AiUInt16 uw_xid, TY_API_HS_BC_ERR * px_Error );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCXferDefEx    (AiUInt32 bModule, TY_API_HS_BC_XFER *px_Xfer, TY_API_HS_BC_XFER_INFO *px_XferInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCDytagDef     (AiUInt32 bModule, AiUInt8 uc_Con,
                                                                                                          AiUInt16 uw_HeaderId,    TY_API_HS_BC_DYTAG ax_Dytag[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCGetDytagDef  (AiUInt32 bModule, AiUInt16 uw_HeaderId,
                                                                                                          TY_API_HS_BC_DYTAG ax_Dytag[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCXferRead     (AiUInt32 bModule, AiUInt16 uw_XferId,
                                                              AiUInt16 uw_Clr, TY_API_HS_BC_XFER_STATUS *px_XferStat );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCXferReadEx   (AiUInt32 bModule, TY_API_HS_BC_XFER_READ_IN *px_XferReadIn,
                                                                                                        TY_API_HS_BC_XFER_STATUS_EX *px_XferStat );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCGetXferDef   (AiUInt32 Module,AiUInt16 uw_XferId, 
                                                              TY_API_HS_BC_XFER* px_BCXfer);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCGetXferDefEx (AiUInt32 Module, AiUInt16 uw_XferId,
                                                                      TY_API_HS_BC_XFER* px_Xfer,
                                                              TY_API_HS_BC_XFER_INFO* px_XferInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCStatusRead   (AiUInt32 Module,    TY_API_HS_BC_STATUS *px_BCStatus);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBCModeCtrl     (AiUInt32 ul_Module,  
                                                              TY_API_HS_BC_MODE_CTRL *px_BcModeCtrl);

/* Remote Terminal Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTLSW          (AiUInt32 bModule, AiUInt8 uc_RtAddr, AiUInt16 uw_StatusWord );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTLAW          (AiUInt32 bModule, AiUInt8 uc_RtAddr, AiUInt16 uw_ActionWord );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTXmtStartDel  (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              AiUInt8 uc_RtMode, AiUInt16 uw_StartDelay );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTXmtPreNb     (AiUInt32 bModule, AiUInt8 uc_RtAddr, AiUInt8 uc_PreambleNb );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTBHDef        (AiUInt32 bModule, AiUInt16 uw_HeaderId, AiUInt16 uw_Rsv, 
                                                              AiUInt16 uw_SqId, AiUInt16 uw_EqId, AiUInt8 uc_QSize, AiUInt8 uc_Bqm, 
                                                              AiUInt8 uc_Bsm, AiUInt8 uc_Sqm, AiUInt8 uc_Eqm, AiUInt8 uc_Dbm, AiUInt8 uc_Wcm );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTBHRead       (AiUInt32 bModule, AiUInt16 uw_HeaderId,
                                                              AiUInt32 *pul_SimMem, TY_API_HS_RT_BH_INFO *px_Bh );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTMIDDef       (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              AiUInt8 uc_MsgId, AiUInt16 uw_HeaderId,
                                                              AiUInt8 uc_MsgIdType, AiUInt8 uc_MsgIdCon,
                                                              AiUInt8 uc_MsgIdInt );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTErrInj       (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                                                                      AiUInt8 uc_MsgId, TY_API_HS_RT_ERR *px_Err );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTGetErrInj    (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                                                                      AiUInt8 uc_MsgId, TY_API_HS_RT_ERR *px_Err );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTDytagDef     (AiUInt32 bModule, AiUInt8 uc_Con,
                                                                                                        AiUInt16 uw_HeaderId,
                                                                                                        TY_API_HS_RT_DYTAG ax_DyData[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTGetDytagDef  (AiUInt32 bModule, AiUInt16 uw_HeaderId,
                                                                                                          TY_API_HS_RT_DYTAG ax_Dytag[4] );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTStatus       (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              TY_API_HS_RT_STATUS_DSP *px_Dsp );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTMIDRead      (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              AiUInt8 uc_MsgId, AiUInt8 uc_MsgIdType,
                                                              AiUInt8 uc_ClrBufStat,
                                                              TY_API_HS_RT_MID_STATUS *px_MsgIdStatus );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTMIDReadEx    (AiUInt32 bModule, TY_API_HS_RT_MID_READ_IN *px_MsgIdReadIn,
                                                                                                        TY_API_HS_RT_MID_STATUS_EX *px_MsgIdStatus );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTGetMidBufferHeaderInfo (AiUInt32 Module, AiUInt8 RT, AiUInt8 Type,
                                                              AiUInt8 MID_MC, AiUInt32 *BufHeaderIndex, AiUInt32 *BufHeaderAddr);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsRTModeCtrl     (AiUInt32 ul_Module,  
                                                              TY_API_HS_RT_MODE_CTRL *px_RtModeCtrl);

/* Replay Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsReplayIni      (AiUInt32 bModule, AiUInt8 uc_Nct,
                                                              AiUInt8 uc_Cet, AiUInt8 uc_RInt, AiUInt32 ul_HsMsgCnt );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsReplayStatus   (AiUInt32 bModule, TY_API_HS_REP_STATUS *px_RepStatus );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsReplayRT       (AiUInt32 bModule, AiUInt8 uc_Con, AiUInt8 uc_Mode, AiUInt8 uc_Rt);

/* Calibration Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsCalSigCon      (AiUInt32 bModule, AiUInt8 uc_Channel, AiUInt8 uc_Con );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsCalCplCon      (AiUInt32 bModule, AiUInt8 uc_Mode );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsCalXmtCon      (AiUInt32 bModule, AiUInt8 uc_Channel, AiUInt8 uc_Power );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsCalRxMode      (AiUInt32 bModule, TY_API_HS_RX_SELECT *px_RxBusSelect );

/* Bus Monitor Commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMIntMode      (AiUInt32 bModule, AiUInt8 uc_IMod, AiUInt8 uc_SMod, AiUInt8 uc_Res );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMCapMode      (AiUInt32 bModule, AiUInt8 uc_CapMode, AiUInt32 ul_Tat );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMFilterIni    (AiUInt32 bModule, AiUInt8 uc_RtAddr, TY_API_HS_BM_MSG_FLT *px_MsgCap );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMStatusRead   (AiUInt32 bModule, TY_API_HS_BM_STATUS *px_Status );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMTrgCon       (AiUInt32 bModule, AiUInt16 uw_WordLocation,
                                                              AiUInt16 uw_TrgCon,
                                                              TY_API_HS_BM_TRG_COND *px_TrgCondition );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMActRead      (AiUInt32 bModule, AiUInt16 uw_EntryNr,
                                                              TY_API_HS_BM_ACT *px_Activity );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMRTActRead    (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              TY_API_HS_BM_RT_ACT *px_Activity );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMRTMIDActRead (AiUInt32 bModule, AiUInt8 uc_RtAddr,
                                                              AiUInt8 uc_Mid, AiUInt8 uc_MidType,
                                                              TY_API_HS_BM_RT_ACT *px_Activity );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMStackpRead   (AiUInt32 bModule, 
                                                              TY_API_HS_BM_STACKP_DSP *px_Stack );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMStackEntryRead(AiUInt32 bModule, AiUInt8 uc_PtrType,
                                                              AiUInt8 uc_OffsetSign, AiUInt32 ul_Offset,
                                                              AiUInt16 uw_ReadStartIndex, AiUInt16 uw_EntryReadSize,
                                                              TY_API_HS_BM_STACK_ENTRY *px_Entry );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMStackEntryFind(AiUInt32 bModule, AiUInt8 uc_PtrType,
                                                              AiUInt8 uc_OffsetSign, AiUInt16 uw_EntryType,
                                                              AiUInt16 uw_Offset, AiUInt32 ul_EntryWord,
                                                              AiUInt32 ul_EntryMask, TY_API_HS_BM_STACK_FND *px_Entry );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMRecStatusRead(AiUInt32 bModule, TY_API_HS_BM_REC *px_Rec );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMCopyRecDat   (AiUInt32 bModule, TY_API_HS_BM_REC *px_Rec );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMDytagMonDef  (AiUInt32 bModule, TY_API_BM_DYTAG_MON_DEF *px_DytagMon);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsBMDytagMonRead (AiUInt32 bModule, TY_API_BM_DYTAG_MON_READ_CTRL *px_DytagMonCtrl,
                                                              TY_API_BM_DYTAG_MON_ACT *px_DytagMonAct);

/* GPIO commands */
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioReset      (AiUInt32 bModule, AiUInt8 *puc_DeviceCount,
                                                              TY_API_HS_GPIO_RES_INF *px_GpioResInf );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioResetInfo  (AiUInt32 bModule, AiUInt8 *puc_DeviceCount,
                                                              TY_API_HS_GPIO_RES_INF *px_GpioResInf );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioReadStatus (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 *puc_Status);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioWriteCmd   (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Command);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioBrWriteCmd (AiUInt32 bModule, AiUInt8 uc_Command);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioReadReg    (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 *puc_Data);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioWriteReg   (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 uc_Data);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioBrWriteReg (AiUInt32 bModule, AiUInt8 uc_Address, AiUInt8 uc_Data);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioReadRegBlock(AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 uc_Count, AiUInt8 *puc_Data );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsGpioWriteRegBlock(AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 uc_Count, AiUInt8 *puc_Data );

AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxReset          (AiUInt32 bModule, AiUInt8 *puc_CtxDevCount,
                                                              TY_API_HS_GPIO_RES_INF *px_CtxResInf );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxBite           (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 *puc_Result, AiUInt8 *puc_FailInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxReadVersion    (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 *puc_Version );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxReadStatus     (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 *puc_Status );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxModeCon        (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Mode );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxDacCon         (AiUInt32 bModule, AiUInt8 uc_Device, AiFloat f_Pulse,
                                                              AiFloat f_Rise, AiFloat f_Fall, AiFloat f_OvrShoot );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxReadReg        (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 *puc_Data );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910HsCtxWriteReg       (AiUInt32 bModule, AiUInt8 uc_Device, AiUInt8 uc_Address,
                                                              AiUInt8 uc_Data );

/****************************************************************
 **    EFEX Functions
 ****************************************************************/
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCXferRead     (AiUInt32 bModule, AiUInt16 uw_XferId, TY_API_EF_BC_XFER_STATUS *px_EfXferStat);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCXferCon      (AiUInt32 bModule, AiUInt16 uw_XferId, AiUInt8  uc_Enable);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCXferDef      (AiUInt32 bModule, TY_API_EF_BC_XFER *px_HsEfXfer);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCXferDefEx    (AiUInt32 bModule, TY_API_EF_BC_XFER *px_HsEfXfer, TY_API_HS_BC_XFER_INFO *px_XferInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCGetXferDef   (AiUInt32 Module, AiUInt16 uw_XferId, TY_API_EF_BC_XFER *px_BCXfer);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCGetXferDefEx (AiUInt32 Module,    AiUInt16 uw_XferId,
                                                                      TY_API_EF_BC_XFER* px_Xfer,
                                                                      TY_API_HS_BC_XFER_INFO* px_XferInfo);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCRespSimDef   (AiUInt32 bModule, TY_API_EF_BC_RESP_SIM *px_BcRespSim);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCRespSimCtrl  (AiUInt32 bModule, TY_API_EF_BC_RESP_SIM_MODE *px_BcRespSimMode);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCInternalCtrl (AiUInt32 bModule, TY_API_EF_BC_INTERNAL_MODE *px_BcInternalMode);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCInternalDef  (AiUInt32 bModule, TY_API_EF_BC_INTERNAL *px_BcInternal);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCSyncTimeDef  (AiUInt32 bModule, TY_API_EF_BC_SYNC_TIME_DEF *px_SyncTime);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBCSyncTimeGet  (AiUInt32 bModule, TY_API_EF_BC_SYNC_TIME_GET *px_SyncTime);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfRTIni          (AiUInt32 bModule, AiUInt8  uc_RtAddr, AiUInt8  uc_RtMode, AiFloat  f_RespEfex, TY_API_EF_RT_DESCRIPTOR *px_rtdesc);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfRTStatusRead   (AiUInt32 bModule, AiUInt8  uc_RtAddr, TY_API_EF_RT_DESCRIPTOR *px_RtDesc);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfRTModeCodeDef  (AiUInt32 bModule, AiUInt8  uc_RtAddr, AiUInt8  uc_ModeCode, AiUInt8  uc_ModeCodeTx, AiUInt8  uc_ModeCodeBrc);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfRTModeCodeDefEx(AiUInt32 bModule, TY_API_EF_RT_MC_DEF *px_MCInfo );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBMStackEntryRead(AiUInt32 bModule, AiUInt8 uc_PtrType, AiUInt8 uc_OffsetSign, AiUInt32 ul_Offset,
                                                                                        AiUInt16 uw_ReadStartIndex, AiUInt16 uw_EntryReadSize, TY_API_EF_BM_STACK_ENTRY *px_Entry );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBMTrigType     (AiUInt32 bModule, AiUInt8 uc_SHdrTriggerSel, AiUInt8 uc_CWTriggerSel);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfBMTrgCon       (AiUInt32 bModule, AiUInt16 uw_WordLocation,
                                                                                                      AiUInt16 uw_TrgCon,
                                                                                                        TY_API_EF_BM_TRG_COND *px_TrgCondition );
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdHsFrameModeSelect(AiUInt32 bModule, AiUInt8 uc_FModeSelect);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfTimeOutDef     (AiUInt32 bModule, AiInt16 sw_Etoof, AiUInt uw_EfexTo);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfSwitchEfaEfex  (AiUInt32 bModule, AiUInt8 uc_SwitchType, AiUInt32 aul_LicenseKey[4], AiUInt8 *puc_SwitchResult);
AI_LIB_FUNC AiReturn AI_CALL_CONV Api3910CmdEfCalRxMode      (AiUInt32 bModule, TY_API_EF_RX_SELECT *px_RxBusSelect );

/****************************************************************/
#endif /* !defined(NO_PROTOTYPE_INCLUDE) */

#endif /* AI3910I_DEF_H */
/* end: ai3910i_def.h */


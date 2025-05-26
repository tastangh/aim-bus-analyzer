/* SPDX-FileCopyrightText: 2001-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Ai1553i_fnc.h
 *
 *  Function prototypes for the 1553 Application Interface. 
 *
 */

#ifndef AI1553I_FNC_H
#define AI1553I_FNC_H

#if defined(_AIM_VME)
#include "AiVmeGeneric.h"
#include "ApiVme.h"
#endif


#include "ApiLsDQue.h"
#include "ApiLsBc.h"
#include "ApiLsBm.h"
#include "ApiLsRt.h"
#include "ApiLsBuf.h"
#include "ApiLsScope.h"
#include "ApiLsTrack.h"
#include "Aim1553_esmart_update.h"
#include "ApiGenIo.h"
#include "ApiLsQue.h"
#include "ApiVersions.h"
#include "ApiLsCal.h"
#include "ApiLsReplay.h"
#include "ApiGen.h"
#include "ApiLsSys.h"
#include "ApiLsFifo.h"
#include "ApiNetwork.h"




/*********** USB device specific memory access functions *******************************************************************************************************/

/*! \brief Function to read USB device specific memory areas

    Provides functionality to read from either EEProm or 8051 program RAM
    on USB based devices.
    \param [in] bModule - stream handle. Target of read is corresponding board
    \param [in] memtype - memory type to read from. Either EEProm or 8051 RAM. Defines are USB_EEPROM and USB_8051_RAM
    \param [in] offset - relative byte offset to start of corresponding memory area to read from
    \param [in] size - number of bytes to read
    \param [out] data - pointer to memory area to read data into
    \param [out] p_BytesRead - optional parameter that will receive number of bytes read
    \return returns API_OK on success */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiReadUsbData(AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt32 size, void* data, AiUInt32* p_BytesRead);


/*! \brief Function to write USB device specific memory areas

    Provides functionality to write to either EEProm or 8051 program RAM
    on USB based devices.
    \param [in] bModule - stream handle. Target of write is corresponding board
    \param [in] memtype - memory type to write to. Either EEProm or 8051 RAM. Defines are USB_EEPROM and USB_8051_RAM
    \param [in] offset - relative byte offset to start of corresponding memory area to write to
    \param [in] size - number of bytes to write
    \param [in] data - pointer to memory area to write data from
    \param [out] p_BytesWritten - optional parameter that will receive number of bytes written
    \return returns API_OK on success */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiWriteUsbData(AiUInt32 bModule, AiUInt8 memtype, AiUInt32 offset, AiUInt32 size, void* data, AiUInt32* p_BytesWritten);





AI_LIB_FUNC AI_DEPRECATED(AiReturn AI_CALL_CONV ApiReadRecData(AiUInt32 bModule, AiUInt8 biu, TY_API_BM_REC *api_rec_stat, void *lpBuf, AiUInt32 *lBytesRead));
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiWriteRepData         (AiUInt32 bModule, AiUInt8 biu,  TY_API_REP_STATUS *api_rep_stat,
                                                           void *lpBuf, AiUInt32 *lBytesWritten);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiStartPerformanceTimer(AiUInt32 bModule);



 

#endif /* AI1553I_FNC_H */

/* end: ai1553i_def.h */

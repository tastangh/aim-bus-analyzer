/* SPDX-FileCopyrightText: 2001-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiLsScope.h
 *
 *  This header file contains declarations for
 *  1553 Digital Scope related API functionality
 *
 */
#ifndef API_LSSCOPE_H
#define API_LSSCOPE_H

#include "Ai_cdef.h"
#include "Ai1553i_def.h"
#include "Ai1553i_fnc.h"


/************/
/* defines  */
/************/




/* function ApiCmdScopeSetup() */
#define API_SCOPE_COUPLING_PRIMARY         0
#define API_SCOPE_COUPLING_SECONDARY       0
#define API_SCOPE_COUPLING_EXTERN          4
#define API_SCOPE_COUPLING_STUB            5
#define API_SCOPE_COUPLING_STUB_3V         6
#define API_SCOPE_COUPLING_EXTERN_3V       7

#define API_SCOPE_COUPLING_NETWORK         6  /* obsolete 27.7.09 mh */

#define API_SCOPE_RATE_EVERY               0

#define API_SCOPE_QUEUE_SIZE              96 /* KB */
#define API_SCOPE_QUEUE_SIZE_BRAM_2K     128 /* KB */

#define API_SCOPE_START_CONTINUOUS         1
#define API_SCOPE_START_SINGLE             2

#define API_SCOPE_OPR_CHANNEL_DISABLED     0
#define API_SCOPE_OPR_CHANNEL_A            1
#define API_SCOPE_OPR_CHANNEL_B            2
#define API_SCOPE_OPR_CHANNEL_AB           3
#define API_SCOPE_OPR_CHANNEL_A100         4
#define API_SCOPE_OPR_CHANNEL_B100         5

/* function ApiCmdScopeTriggerDef() */
#define API_SCOPE_MODE_GREATER_THAN         0
#define API_SCOPE_MODE_LESS_THAN            1
#define API_SCOPE_MODE_GREATER_OR_LESS_THAN 2
#define API_SCOPE_MODE_GREATER_THAN_SAMPLES 4
#define API_SCOPE_MODE_LESS_THAN_SAMPLES    5
#define API_SCOPE_MODE_BIU                  7

#define API_SCOPE_SRC_PRIMARY              0
#define API_SCOPE_SRC_SECONDARY            1

#define API_SCOPE_SIZE_SINGLE_CH_REC       0x10000  // 64KBytes
#define API_SCOPE_SIZE_DUAL_CH_REC         0x8000   // 32KBytes





/*! \def SCOPE_BUFFER_FLAG_FILLED
If set, buffer contains data. If not set, buffer is empty */
#define SCOPE_BUFFER_FLAG_FILLED     (1 << 0)


/*! \def SCOPE_BUFFER_FLAG_CANCELED
If set, buffer has been canceled and has to be provided again */
#define SCOPE_BUFFER_FLAG_CANCELED   (1 << 1)


/*! \def SCOPE_BUFFER_FLAG_TRIGGER 
If set, the buffer contains the trigger sample */
#define SCOPE_BUFFER_FLAG_TRIGGER    (1 << 2)


/*! \def API_SCOPE_OFFSET_COMP_RESET
Used in \ref ApiCmdScopeOffsetCompensation for reseeting scope offsets */
#define API_SCOPE_OFFSET_COMP_RESET     0


/*! \def API_SCOPE_OFFSET_COMP_MEASURE
Used in \ref ApiCmdScopeOffsetCompensation for measuring \n
and setting scope offsets */
#define API_SCOPE_OFFSET_COMP_MEASURE   1


/*! \def API_SCOPE_OFFSET_COMP_GET
Used in \ref ApiCmdScopeOffsetCompensation for getting current scope offsets */
#define API_SCOPE_OFFSET_COMP_GET       2


#define MAX_SCOPE_COUPLING_MODE     API_SCOPE_COUPLING_EXTERN_3V
#define MAX_SCOPE_CAP_MODE          API_SCOPE_START_SINGLE
#define MAX_SCOPE_OPR_MODE          API_SCOPE_OPR_CHANNEL_B100
#define MAX_SCOPE_SAMPLING_RATE     255

#define MAX_SCOPE_TRG_MODE          API_SCOPE_MODE_BIU
#define MAX_SCOPE_TRG_SRC           API_SCOPE_SRC_SECONDARY
#define MAX_SCOPE_TRG_VALUE         1023
#define MAX_SCOPE_TRG_NB_SAMPLES    255
#define MAX_SCOPE_TRG_HOLDOFF       262143
#define MAX_SCOPE_TRG_DELAY         262143
#define MAX_SCOPE_TRG_TBT           262143



/*! Bit field used in ApiWaitForScopeFinished
Signifies if time out occurred or scope has finished
\ref SCOPE_WAIT_OVERFLOW has to be checked in any case */
typedef AiUInt32 TY_API_SCOPE_WAIT_STATE;

/*! \def SCOPE_WAIT_FINISHED
Waiting for scope finished, no more scope buffers are left */
#define SCOPE_WAIT_FINISHED (1 << 0)

/*! \def SCOPE_WAIT_TIMEOUT
Time out while waiting for scope */
#define SCOPE_WAIT_TIMEOUT  (1 << 1)

/*! \def SCOPE_WAIT_OVERFLOW
Overflow occured during scope recording */
#define SCOPE_WAIT_OVERFLOW (1 << 2)


/*! Union that represents a scope sample packet, \n
consisting of three samples in one 32bit value */
typedef union API_SCOPE_SAMPLE_PACKET
{
    AiUInt32 raw;               /*!< The raw 32bit value */

#ifdef HOST_ENDIAN_LITTLE
    struct
    {
        AiUInt32 res:2;        /*!< reserved bits */
        AiUInt32 sample0:10;   /*!< In chronological order, first scope sample */
        AiUInt32 sample1:10;   /*!< In chronological order, second scope sample */
        AiUInt32 sample2:10;   /*!< In chronological order, third scope sample */
    }samples;
#else
    struct ty_api_scope_sample
    {
        AiUInt32 sample2:10;
        AiUInt32 sample1:10;
        AiUInt32 sample0:10;
        AiUInt32 res:2;
    }samples;
#endif

}TY_API_SCOPE_SAMPLE_PACKET;


/*! \def SCOPE_SAMPLE_PACKET_BY_PACKET_ID
Gets the scope packet with a given ID, beginning with 0, from a scope data buffer */
#define SCOPE_SAMPLE_PACKET_BY_PACKET_ID(pBuffer, packet_id) (*(((TY_API_SCOPE_SAMPLE_PACKET*) pBuffer) + (packet_id)))


/*! \def SCOPE_SAMPLE_PACKET_BY_SAMPLE_ID
Gets the scope packet that contains the sample with a given ID, beginning with 0, from a scope data buffer */
#define SCOPE_SAMPLE_PACKET_BY_SAMPLE_ID(pBuffer, sample_id) (*(((TY_API_SCOPE_SAMPLE_PACKET*) pBuffer) + (sample_id / 3)))


/*! \def NUM_SCOPE_SAMPLE_PACKETS
Converts size in bytes of scope data buffer into number of sample packets in this buffer */
#define NUM_SCOPE_SAMPLE_PACKETS(data_size) ((data_size) / 4)


/*! \def NUM_SCOPE_SAMPLES
Converts size in bytes of scope data buffer into number of samples in this buffer */
#define NUM_SCOPE_SAMPLES(data_size) (((data_size) / 4) * 3)


/*! \def SCOPE_SAMPLE_BY_SAMPLE_ID
Gets the scope sample with a given ID, beginning with 0, from a scope data buffer */
#define SCOPE_SAMPLE_BY_SAMPLE_ID(pBuffer, sample_id) (((((AiUInt32*) pBuffer)[sample_id / 3]) >> (((sample_id % 3) * 10) + 2)) & 0x3FF)





/**************************/
/* structs and typedefs   */
/**************************/



typedef struct ty_api_scope_calibration_info
{
    AiUInt32 lOffsetA;
    AiUInt32 lOffsetB;
    AiUInt32 lOffsetA100;
    AiUInt32 lOffsetB100;
    AiUInt32 lOffsetA100Sec;
    AiUInt32 lOffsetB100Sec;
} TY_API_SCOPE_CALIBRATION_INFO;


/* forwared declaration */
struct API_SCOPE_BUFFER;


/*! \brief type definition for MIL scope buffer handler functions

Is called when a scope buffer is complete
\param ul_Module module handle of the device the scope buffer belongs to
\param p_ScopeBuffer pointer to scope buffer structure of the buffer that is complete */
typedef void (AI_CALL_CONV *TY_API_SCOPE_BUFFER_HANDLER)(AiUInt32 ul_Module, struct API_SCOPE_BUFFER* p_ScopeBuffer);


/*! Bitfield that contains flags for MIL scope
buffers. */
typedef AiUInt32 TY_API_SCOPE_BUFFER_FLAGS;




/*! Structure that represents a buffer used for recording scope data 

This structure encapsulates a pointer for the raw scope data
with some additional information */
typedef struct API_SCOPE_BUFFER
{
    AiUInt32 ulID;                             /*!< This is the user-definable ID of the buffer */
    TY_API_SCOPE_BUFFER_TYPE eBufferType;      /*!< Signifies if buffer is for primary, or secondary channel */
    void* pvBuffer;                            /*!< pointer to actual buffer that will receive scope data */
    AiUInt32 ulBufferSize;                     /*!< size of buffer in bytes */
    TY_API_SCOPE_BUFFER_FLAGS ulFlags;         /*!< various flags, e.g. if buffer was filled */
    AiUInt32 ulDataSize;                       /*!< actual size of data in the buffer in bytes */
    AiUInt32 ulTriggerId;                      /*!< ID of the trigger sample in the buffer starting with 0 */
    TY_API_SCOPE_BUFFER_HANDLER BufferHandler; /*!< handler function that is called after the buffer has been filled */
    void* pvUserData;                          /*!< pointer to user definable data */
} TY_API_SCOPE_BUFFER;



/****************************************************************
 **    MILScope Functions
 ****************************************************************/
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeSetup        (AiUInt32 ul_Module, TY_API_SCOPE_SETUP *px_ScopeSetup);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeTriggerDef   (AiUInt32 ul_Module, TY_API_SCOPE_TRG *px_ScopeTrg);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeCalibrate    (AiUInt32 ul_Module, AiUInt8 ucMode, TY_API_SCOPE_CALIBRATION_INFO * pxInfo );


/*! \brief Creates a scope buffer

    This function creates a scope buffer of type \ref TY_API_SCOPE_BUFFER ,
    that can be used for recording scope data. \n
    It has to be freed with \ref ApiFreeScopeBuffer when no more needed. \n
    A user specific ID, a handler function that is called when the buffer is completed or cancelled
    and a user definable data pointer can be specified for the buffer..
    \param ulModuleHandle handle to the device to create scope buffer for
    \param ulID ID of the scope buffer
    \param eBufferType the buffer's type according to \ref TY_API_SCOPE_BUFFER_TYPE enumeration.
    \param BufferHandler pointer to buffer handler function to set for the buffer.
    \param pvUserData pointer to user definable data that can be used in buffer handler
    \return returns API_OK on success,  an appropriate error code otherwise */
AI_LIB_FUNC TY_API_SCOPE_BUFFER* AI_CALL_CONV ApiCreateScopeBuffer(AiUInt32 ulModuleHandle, AiUInt32 ulID, TY_API_SCOPE_BUFFER_TYPE eBufferType,
                                                                    TY_API_SCOPE_BUFFER_HANDLER BufferHandler, void* pvUserData);


AI_LIB_FUNC AiReturn AI_CALL_CONV ApiFreeScopeBuffer      (AiUInt32 ulModuleHandle, TY_API_SCOPE_BUFFER *px_Buffer);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeStart        (AiUInt32 ul_Module);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiProvideScopeBuffers  (AiUInt32 ulModuleHandle, AiUInt32 ulNumBuffers, TY_API_SCOPE_BUFFER* axScopeBuffers[]);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeTriggerStrobeDef(AiUInt32 ulModuleHandle, TY_API_SCOPE_TRG_STROBE *px_ScopeTrgStrobe);


/*! \brief Gets the scope status of a given board

    This function returns the current scope status of a board
    and the number of scope buffers that are currently provided
    for recording scope data
    \param[in] ulModuleHandle handle of board to get status of
    \param[out] p_ScopeStatus receives the scope status
    \param[out] pulNumBuffersLeft receives number of scope buffers available
    \return returns API_OK on success, an appropriate error code on failure */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeStatus(AiUInt32 ulModuleHandle, TY_API_SCOPE_STATUS* p_ScopeStatus, AiUInt32* pulNumBuffersLeft);


/*! \brief Stops the scope of a board

    \param[in] ulModuleHandle handle of the board to stop scope of
    \return returns API_OK on success, an appropriate error code on failure */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeStop(AiUInt32 ulModuleHandle);


/*! \brief Set extended scope trigger properties of a board

    \param ulModuleHandle handle of the board to set scope trigger of.
    \param px_ScopeTrg pointer to \ref TY_API_SCOPE_TRG_EX structure that contains trigger properties to set.
    \return returns API_OK on success, an appropriate error code on failure */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeTriggerDefEx(AiUInt32 ulModuleHandle, TY_API_SCOPE_TRG_EX* px_ScopeTrg);


/*! \brief Waits until scope capturing has finished

    This function waits until all provided scope buffers are filled,
    or canceled. Or until the specified time out is reached. \n
    It returns the result of the wait as a bit field with different possible flags. \n
    The flag \ref SCOPE_WAIT_OVERFLOW has to be examined after every call to ensure no
    overflow in scope capturing occured 
    \param [in] ulModuleHandle handle to the device to wait for
    \param [in] lTimeOut time out in milliseconds to wait
    \param [out] p_WaitResultFlags receives the result of the wait 
    \return returns API_OK on success, an appropriate error code otherwise */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiWaitForScopeFinished(AiUInt32 ulModuleHandle, AiInt32 lTimeOut, 
                                                          TY_API_SCOPE_WAIT_STATE* p_WaitResultFlags);


/*! \brief Resets scope functionality of a board

    This resets all scope related settings to their default state.
    At least \ref ApiCmdScopeSetup has to be called if you want to
    start scope capturing again.
    \param ulModuleHandle handle to the device to reset scope on
    \return returns API_OK on success, an appropriate error code otherwise */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeReset(AiUInt32 ulModuleHandle);


/*! \brief Creates a list of scope buffers

    This function takes an array of pointers to \ref TY_API_SCOPE_BUFFER structures.\n
    Each of these pointers is set to a newly allocated scope buffer structure which
    has to be freed with \ref ApiFreeScopeBuffer when no more needed. \n
    The buffers are created with consecutive IDs, beginning from 0.
    \param ulModuleHandle handle to the device to create scope buffers for
    \param ulNumBuffers number of scope buffers to create
    \param paxBufferList array of pointers to \ref TY_API_SCOPE_BUFFER structures. At least ulNumBuffers entries are needed.
    \param pBufferHandler pointer to buffer handler function to set for the buffers.
    \param pvUserData pointer to user definable data that can be used in buffer handler
    \return returns API_OK on success,  an appropriate error code otherwise */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCreateScopeBufferList(AiUInt32 ulModuleHandle, AiUInt32 ulNumBuffers, TY_API_SCOPE_BUFFER* paxBufferList[], 
                                                           TY_API_SCOPE_BUFFER_HANDLER pBufferHandler, void* pvUserData);


/*! \brief This function is used to compensate the scope offsets from zero-line

    Depending on the mode requested, it will either perform some measurement probes \n
    in order to determine the pitch of the scope data from the zero line of an idle bus, \n
    or it will read the current offset settings, or reset them.
    \param ulModuleHandle handle to the device to compensate scope offsets for.
    \param ucMode signifies what task the function has to perform, e.g. measure, reset
    \param pxOffsets optional pointer to \ref TY_API_SCOPE_OFFSETS structure where current offsets will be returned
    \return returns API_OK on success,  an appropriate error code otherwise */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiCmdScopeOffsetCompensation(AiUInt32 ulModuleHandle, AiUInt8 ucMode, TY_API_SCOPE_OFFSETS* pxOffsets);



#endif
/* end: ApiLsScope.h */

/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Aim1553_esmart_update.h
 *
 *  This header file contains declarations for
 *  the esmart update
 *
 */



/************/
/* defines  */
/************/


/*! \def API_FILE_FLAG_OVERWRITE
 * File can be overwritten
 */
#define API_FILE_FLAG_OVERWRITE    (1 << 0 )


/*! \def API_FILE_FLAG_APPEND
 * File can be appended
 */
#define API_FILE_FLAG_APPEND       (1 << 1 )





/**************************/
/* structs and typedefs   */
/**************************/

/*! \struct api_file_upload_in
 * This structure holds the input parameters for
 * the \ref ApiFileUpload API function
 */
typedef struct api_file_upload_in
{
    const AiChar* pc_DestFilePath;      /*!< path to destination file on host where uploaded file will be stored */
    const AiChar* pc_SourceFileName;    /*!< Name of the file on the embedded device to upload */
    AiUInt32 ul_Flags;                  /*!< File flags for the destination file e.g. \ref API_FILE_FLAG_OVERWRITE to allow overwriting */
} TY_API_FILE_UPLOAD_IN;




/*! \enum api429_e_update_status
 *
 * Enumeration of all possible update states
 * when using ApiUpdateStatus
 */
typedef enum api_e_update_status
{
    API_UPDATE_DEV_CON_FAILURE = -1, /*!< Update related connection to board is broken. No update possible any more */
    API_UPDATE_IDLE,                 /*!< Device is ready for updating */
    API_UPDATE_START_REQUESTED,      /*!< Device update has been requested but has not started yet */
    API_UPDATE_IN_PROGRESS,          /*!< Device update is currently in progress */
    API_UPDATE_FAILED,               /*!< Device update procedure failed */
    API_UPDATE_FINISHED              /*!< Device update procedure successfully finished */
} TY_API_E_UPDATE_STATUS;





/*! \struct api_packet_update_status_out
 *
 * This structure holds the output parameters for
 * the ApiUpdateStatus function
 */
typedef struct api_packet_update_status_out
{
    TY_API_E_UPDATE_STATUS e_Status;            /*!< Current update procedure status. See \ref api_e_update_status */
    AiUInt32               ul_ComponentsDone;   /*!< Number of components that have been updated so far. Only valid when
                                                        update procedure is in progress, finished or failed */
    AiUInt32               ul_TotalComponents;  /*!< Total number of components that have to be updated.
                                                        Only valid when update procedure is in progress, finished or failed */
} TY_API_PACKET_UPDATE_STATUS_OUT;






/***********************/
/* interface functions */
/***********************/


/*! \brief Copies a file from host to target device
 *
 *  This function can be used to copy files from the host onto the embedded device system.
 *  \param ul_BoardHandle handle to board that file will be transferred to.
 *  \param sz_FilePath the path to the file to download. The file will be stored with basename on the embedded device
 *  \return returns 0 on success, an error code on failure.
 */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiFileDownload( AiUInt32 module, const char* sz_FilePath);


/*! \brief Copies a file from target device to host
 *
 *  This function can be used to copy files from the embedded device system to the host.
 *  \param ul_BoardHandle handle to board that file will be transferred from.
 *  \param px_UploadIn pointer to structure with the input parameters
 *  \return returns 0 on success, an error code on failure.
 */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiFileUpload( AiUInt32 module, TY_API_FILE_UPLOAD_IN* px_UploadIn);

/*! \brief Returns current device update process status
 *
 * This function returns the current state of a device update
 * procedure.
 * \param ul_BoardHandle handle to board to get update procedure status from
 * \param px_UpdateStatus pointer to structure that will receive the update process status
 * \return returns 0 on success, an error code on failure.
 */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiUpdateStatus( AiUInt32 module, TY_API_PACKET_UPDATE_STATUS_OUT* px_UpdateStatus);

/*! \brief Start device update procedure
 *
 * This function starts the update procedure with a previously downloaded file.
 * \param ul_BoardHandle handle to board where the update procedure shall be started
 * \param updatePackageName name of the update package, that has been previously downloaded to device
 * \return return 0 on success, an error code otherwise
 */
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiUpdateStart( AiUInt32 module, const AiChar* updatePackageName);


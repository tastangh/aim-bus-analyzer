/* SPDX-FileCopyrightText: 2016-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file ApiNetwork.h
 *
 *  This header file contains declarations for
 *  network related API functionality
 *
 */

#ifndef AI1553_NETWORK_H
#define AI1553_NETWORK_H 



/************/
/* defines  */
/************/


/**************************/
/* structs and typedefs   */
/**************************/
#ifndef _API_RPC_HEADER
typedef struct  ty_tagserverinfo
{
    TY_VER_INFO server_version;
    AiUInt32    protocol_major;
    AiUInt32    protocol_minor;
    AiChar      application_name[128];
    AiChar      description[128];
    AiChar      host_name[128];
    AiChar      os_info[128];
}TY_API_SERVERINFO;
#endif

/***********************/
/* interface functions */
/***********************/

AI_LIB_FUNC AiReturn AI_CALL_CONV ApiConnectToServer      ( AiUChar * pszNetworkAdress, AiInt16 *w_ServerBoards);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiDisconnectFromServer ( AiUChar * pszNetworkAdress );
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiGetServerInfo        ( AiUInt32 bModule, TY_API_SERVERINFO *pServerInfo);



#pragma pack(1)
typedef struct
{
    AiUInt32 magic;
    AiUInt32 protocol;
    AiUInt32 port;
    AiUInt32 board_count;
    AiUInt32 protocol_major;
    AiUInt32 protocol_minor;
    char     host_name[128];
} TY_API_DISCOVER_RESPONSE;
#pragma pack()

typedef void(AI_CALL_CONV *TY_API_DISCOVER_CALLBACK_FUNC_PTR)(const char * ip, TY_API_DISCOVER_RESPONSE * response);

#pragma pack(1)
typedef struct
{
    AiUInt32 port;
    TY_API_DISCOVER_CALLBACK_FUNC_PTR callback;
    AiInt32 threadTableIndex;
    AiBoolean active;
} TY_API_DISCOVER_CALLBACK;
#pragma pack()


AI_LIB_FUNC AiReturn AI_CALL_CONV ApiDiscoverServer(AiUInt32 port);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiDiscoverServerCallback(TY_API_DISCOVER_CALLBACK * config);
AI_LIB_FUNC AiReturn AI_CALL_CONV ApiDiscoverServerUnregister(TY_API_DISCOVER_CALLBACK * config);


/**********************/
/* internal functions */
/**********************/


#endif /* AI1553_NETWORK_H */

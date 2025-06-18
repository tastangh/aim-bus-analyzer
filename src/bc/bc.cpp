#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <chrono>
#include <thread>

#include "AiOs.h"
#include "Api1553.h"

void check_api_return(AiReturn ret, const std::string& function_name, const char* file, int line) {
    if (ret != API_OK) {
        const char* error_message = ApiGetErrorMessage(ret);
        std::string message = "HATA: " + function_name + " basarisiz oldu. Kod: " + std::to_string(ret) +
                              " -> " + (error_message ? error_message : "Bilinmeyen Hata") +
                              " (Dosya: " + file + ", Satir: " + std::to_string(line) + ")";
        throw std::runtime_error(message);
    }
}
#define CHECK_API(ret, func_name) check_api_return(ret, func_name, __FILE__, __LINE__)

namespace MilUtil 
{
    static AiUInt32 g_bc_hid = 1;
    static AiUInt32 g_bid = 1;

    AiUInt32 NextBCHid() {
        return g_bc_hid++;
    }

    AiUInt32 NextBid(AiUInt8 queueSize) {
        AiUInt32 id = g_bid;
        g_bid += (1 << queueSize);
        return id;
    }

    void ConfigureHardware(AiUInt32 modHandle, int biuId) {
        CHECK_API(ApiCmdCalCplCon(modHandle, biuId, API_CAL_BUS_PRIMARY, API_CAL_CPL_TRANSFORM), "ApiCmdCalCplCon (Primary)");
        CHECK_API(ApiCmdCalCplCon(modHandle, biuId, API_CAL_BUS_SECONDARY, API_CAL_CPL_TRANSFORM), "ApiCmdCalCplCon (Secondary)");
    }

    void SetData(AiUInt32 modHandle, int biuId, AiUInt16 headerId, AiUInt16 index, const AiUInt16* data, AiUInt16 wordCount) {
        AiUInt16 outIndex;
        AiUInt32 outAddr;
        CHECK_API(ApiCmdBufDef(modHandle, biuId, API_BUF_BC_MSG, headerId, index, wordCount, const_cast<AiUInt16*>(data), &outIndex, &outAddr), "ApiCmdBufDef");
    }

    void CreateTransfer(AiUInt32 modHandle, int biuId, AiUInt16 transferId, AiUInt8 type, AiUInt16 headerId, AiUInt8 txRt, AiUInt8 txSa, AiUInt8 rxRt, AiUInt8 rxSa, AiUInt8 wcnt) {
        AiUInt16 bufferId = NextBid(API_QUEUE_SIZE_1);

        TY_API_BC_BH_INFO bh_info;
        memset(&bh_info, 0, sizeof(bh_info));
        CHECK_API(ApiCmdBCBHDef(modHandle, biuId, headerId, bufferId, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, API_BSM_TX_KEEP_SAME, API_SQM_AS_QSIZE, 0, 0, &bh_info), "ApiCmdBCBHDef");

        TY_API_BC_XFER bc_xfer;
        memset(&bc_xfer, 0, sizeof(bc_xfer));
        bc_xfer.xid = transferId;
        bc_xfer.hid = headerId;
        bc_xfer.type = type;
        bc_xfer.xmt_rt = txRt;
        bc_xfer.xmt_sa = txSa;
        bc_xfer.rcv_rt = rxRt;
        bc_xfer.rcv_sa = rxSa;
        bc_xfer.wcnt = wcnt;
        
        AiUInt32 desc_addr;
        CHECK_API(ApiCmdBCXferDef(modHandle, biuId, &bc_xfer, &desc_addr), "ApiCmdBCXferDef");
    }
    
    void CreateFraming(AiUInt32 modHandle, int biuId, const AiUInt16* transferIds, int count) {
        TY_API_BC_FRAME minor_frame;
        memset(&minor_frame, 0, sizeof(minor_frame));
        minor_frame.id = 1;
        minor_frame.cnt = count;
        for(int i = 0; i < count; ++i) {
            minor_frame.instr[i] = API_BC_INSTR_TRANSFER;
            minor_frame.xid[i] = transferIds[i];
        }
        CHECK_API(ApiCmdBCFrameDef(modHandle, biuId, &minor_frame), "ApiCmdBCFrameDef");
        
        TY_API_BC_MFRAME_EX major_frame;
        memset(&major_frame, 0, sizeof(major_frame));
        major_frame.cnt = 1;
        major_frame.fid[0] = minor_frame.id;
        CHECK_API(ApiCmdBCMFrameDefEx(modHandle, biuId, &major_frame), "ApiCmdBCMFrameDefEx");
    }

    bool IsBcActive(AiUInt32 modHandle, int biuId) {
        TY_API_BC_STATUS_DSP bc_status;
        memset(&bc_status, 0, sizeof(bc_status));
        CHECK_API(ApiCmdBCStatusRead(modHandle, biuId, &bc_status), "ApiCmdBCStatusRead");
        return (bc_status.status == API_BC_STATUS_BUSY);
    }
}

void PrepareBusController(AiUInt32 board_handle, int biu_id) {
    std::cout << "\nBus Controller hazirlaniyor..." << std::endl;
    CHECK_API(ApiCmdBCIni(board_handle, biu_id, 0, 0, 0, 0), "ApiCmdBCIni");

    const AiUInt16 xferId1 = 1, headerId1 = MilUtil::NextBCHid();
    MilUtil::CreateTransfer(board_handle, biu_id, xferId1, API_BC_TYPE_BCRT, headerId1, 0, 0, 19, 30, 0);
    
    AiUInt16 message_data[32];
    for(int i=0; i<32; ++i) message_data[i] = 0xBC00 + i + 1;
    MilUtil::SetData(board_handle, biu_id, headerId1, 1, message_data, 32);

    const AiUInt16 xferId2 = 2, headerId2 = MilUtil::NextBCHid();
    MilUtil::CreateTransfer(board_handle, biu_id, xferId2, API_BC_TYPE_RTBC, headerId2, 19, 30, 0, 0, 0);

    AiUInt16 transfer_ids[] = { xferId1, xferId2 };
    MilUtil::CreateFraming(board_handle, biu_id, transfer_ids, 2);
    
    std::cout << "OK: BC Hazirlandi." << std::endl;
}

void RunSimulation(AiUInt32 board_handle, int biu_id) {
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n", 
           "Framing :      1000 times  ",
           "-------------------------- ",
           "BC          -> RT19RcvSA30 ",
           "RT19XmtSA30 -> BC          ",
           "-------------------------- ",
           "     total 2000 transfers " );
    
    std::cout << "\nBus Controller baslatiliyor..." << std::endl;
    AiUInt32 major_frame_addr;
    AiUInt32 minor_frame_addrs[64];
    CHECK_API(ApiCmdBCStart(board_handle, biu_id, API_BC_START_IMMEDIATELY, 1000, 10, 0, &major_frame_addr, minor_frame_addrs), "ApiCmdBCStart");

    std::cout << "BC baslatildi. Durum izleniyor..." << std::endl;
    do {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (MilUtil::IsBcActive(board_handle, biu_id));

    std::cout << "BC durdu." << std::endl;
}

int main() {
    AiUInt32 board_handle = 0;
    bool api_initialized = false;
    const int DEVICE_ID = 0, BIU_ID = 0, STREAM_ID = 1;

    try {
        std::cout << "AIM MIL-STD-1553 LsBcSample Tam Esdeger Uygulama" << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;

        AiReturn board_count = ApiInit();
        if (board_count < 1) { throw std::runtime_error("KRITIK HATA: Sistemde hicbir AIM karti bulunamadi."); }
        api_initialized = true;
        std::cout << "OK: ApiInit basariyla calisti." << std::endl;
        
        TY_API_OPEN api_open_params;
        memset(&api_open_params, 0, sizeof(api_open_params));
        api_open_params.ul_Module = DEVICE_ID;
        api_open_params.ul_Stream = STREAM_ID;
        strncpy(api_open_params.ac_SrvName, "local", sizeof(api_open_params.ac_SrvName) - 1);
        CHECK_API(ApiOpenEx(&api_open_params, &board_handle), "ApiOpenEx");
        std::cout << "OK: Kart acildi." << std::endl;

        std::cout << "Akis sifirlaniyor..." << std::endl;
        TY_API_RESET_INFO reset_info;
        memset(&reset_info, 0, sizeof(reset_info));
        CHECK_API(ApiCmdReset(board_handle, BIU_ID, API_RESET_ALL, &reset_info), "ApiCmdReset");

        MilUtil::ConfigureHardware(board_handle, BIU_ID);
        PrepareBusController(board_handle, BIU_ID);
        RunSimulation(board_handle, BIU_ID);
        
    } catch (const std::runtime_error& e) {
        std::cerr << "\nPROGRAMDA KRITIK HATA: " << e.what() << std::endl;
    }

    if (board_handle != 0) {
        std::cout << "\nUygulama kapatiliyor, kaynaklar serbest birakiliyor..." << std::endl;
        ApiCmdBCHalt(board_handle, BIU_ID);
        ApiClose(board_handle);
    }
    if (api_initialized) {
        ApiExit();
    }
    
    std::cout << "Cikis yapildi." << std::endl;
    return 0;
}
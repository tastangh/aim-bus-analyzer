#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <chrono> // Zamanlama için eklendi
#include <thread> // Uyku (sleep) için eklendi

// AIM API başlık dosyası
#include "Api1553.h"

// --- Sabitler ---
const int AIM_DEVICE_ID = 0;
const int BIU_NUMBER = 1;
const AiUInt16 XFER_ID_BC_TO_RT = 1;
const AiUInt16 BUFFER_ID = 1;

// --- Hata kontrolü için yardımcı fonksiyon ---
void check_api_return(AiReturn ret, const std::string& function_name) {
    if (ret != API_OK) {
        const char* error_message = ApiGetErrorMessage(ret);
        std::string message = "HATA: " + function_name + " basarisiz oldu. Kod: " + std::to_string(ret) +
                              " -> " + (error_message ? error_message : "Bilinmeyen Hata");
        throw std::runtime_error(message);
    }
    std::cout << "OK: " << function_name << " basariyla calisti." << std::endl;
}

// --- Ana Fonksiyon ---
int main() {
    AiUInt32 board_handle = 0;
    bool api_initialized = false;

    try {
        std::cout << "AIM MIL-STD-1553 Basit Bus Controller Uygulamasi" << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;

        // 1. API Başlatma
        AiReturn init_result = ApiInit();
        if (init_result < 0) {
            const char* err_msg = ApiGetErrorMessage(init_result);
            std::cerr << "KRITIK HATA: ApiInit basarisiz oldu. Kod: " << init_result << " -> " << (err_msg ? err_msg : "Bilinmeyen Hata") << std::endl;
            return 1;
        }
        api_initialized = true;
        std::cout << "OK: ApiInit basariyla calisti. Bulunan kart sayisi: " << init_result << std::endl;
        
        // 2. Kartı Açma ve Resetleme
        TY_API_OPEN api_open_params;
        memset(&api_open_params, 0, sizeof(api_open_params));
        api_open_params.ul_Module = AIM_DEVICE_ID;
        api_open_params.ul_Stream = BIU_NUMBER;
        strcpy(api_open_params.ac_SrvName, "local");
        
        check_api_return(ApiOpenEx(&api_open_params, &board_handle), "ApiOpenEx");

        TY_API_RESET_INFO reset_info;
        memset(&reset_info, 0, sizeof(reset_info));
        check_api_return(ApiCmdReset(board_handle, BIU_NUMBER, API_RESET_ALL, &reset_info), "ApiCmdReset");

        // 3. Bus Controller Modunu Ayarlama
        std::cout << "\nBus Controller modu ayarlanior..." << std::endl;
        check_api_return(ApiCmdBCIni(board_handle, BIU_NUMBER, 0, 0, 0, 0), "ApiCmdBCIni");

        // 4. Transfer ve Buffer Tanımlamaları
        TY_API_BC_BH_INFO bh_info;
        memset(&bh_info, 0, sizeof(bh_info));
        check_api_return(ApiCmdBCBHDef(board_handle, BIU_NUMBER, BUFFER_ID, BUFFER_ID, 0, 0, API_QUEUE_SIZE_1, 0, 0, 0, 0, 0, &bh_info), "ApiCmdBCBHDef");

        AiUInt16 message_data[32] = { 0 }; // Veri dizisini burada tanımla
        check_api_return(ApiCmdBufWriteBlock(board_handle, BIU_NUMBER, API_BUF_BC_MSG, BUFFER_ID, 0, 0, 3, message_data), "ApiCmdBufWriteBlock");

        TY_API_BC_XFER bc_xfer;
        memset(&bc_xfer, 0, sizeof(bc_xfer));
        bc_xfer.xid = XFER_ID_BC_TO_RT;
        bc_xfer.hid = BUFFER_ID;
        bc_xfer.type = API_BC_TYPE_BCRT;
        bc_xfer.rcv_rt = 1;
        bc_xfer.rcv_sa = 1;
        bc_xfer.wcnt = 3;

        AiUInt32 desc_addr = 0;
        check_api_return(ApiCmdBCXferDef(board_handle, BIU_NUMBER, &bc_xfer, &desc_addr), "ApiCmdBCXferDef");
        
        // 5. Framing
        TY_API_BC_FRAME minor_frame;
        memset(&minor_frame, 0, sizeof(minor_frame));
        minor_frame.id = 1;
        minor_frame.cnt = 1;
        minor_frame.instr[0] = API_BC_INSTR_TRANSFER;
        minor_frame.xid[0] = XFER_ID_BC_TO_RT;
        check_api_return(ApiCmdBCFrameDef(board_handle, BIU_NUMBER, &minor_frame), "ApiCmdBCFrameDef");
        
        TY_API_BC_MFRAME major_frame;
        memset(&major_frame, 0, sizeof(major_frame));
        major_frame.cnt = 1;
        major_frame.fid[0] = minor_frame.id;
        check_api_return(ApiCmdBCMFrameDef(board_handle, BIU_NUMBER, &major_frame), "ApiCmdBCMFrameDef");

        // 6. Bus Controller'ı Başlat
        std::cout << "\nFraming tamamlandi. Bus Controller baslatiliyor..." << std::endl;
        
        AiUInt32 major_frame_addr;
        AiUInt32 minor_frame_addrs[64];
        // DÜZELTME: count=0 (sürekli) ve frame_time=100.0f (100ms) olarak ayarlandı
        check_api_return(ApiCmdBCStart(board_handle, BIU_NUMBER, API_BC_START_IMMEDIATELY, 0, 100.0f, 0, &major_frame_addr, minor_frame_addrs), "ApiCmdBCStart");

        std::cout << "BC basariyla baslatildi ve her 100ms'de bir veri gonderiyor..." << std::endl;
        
        // DÜZELTME: Her 100ms'de bir veri tamponunu güncelleyen döngü
        for (int i = 0; i < 50; ++i) { // Örnek olarak 50 defa (5 saniye) gönderim yap
            // Veriyi güncelle (basit bir sayaç)
            message_data[0]++;
            message_data[1] = 0xAAAA;
            message_data[2] = i;

            // Güncellenmiş veriyi AIM kartındaki mevcut tampona yaz
            check_api_return(ApiCmdBufWriteBlock(board_handle, BIU_NUMBER, API_BUF_BC_MSG, BUFFER_ID, 0, 0, 3, message_data), "ApiCmdBufWriteBlock");
            printf("Gonderilen Veri: %04X %04X %04X\n", message_data[0], message_data[1], message_data[2]);

            // 100 milisaniye bekle
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    } catch (const std::runtime_error& e) {
        std::cerr << "Programda bir hata olustu: " << e.what() << std::endl;
    }

    // 7. Temiz Kapatma
    if (board_handle != 0) {
        std::cout << "\nUygulama kapatiliyor, kaynaklar serbest birakiliyor..." << std::endl;
        ApiCmdBCHalt(board_handle, BIU_NUMBER);
        ApiClose(board_handle);
    }

    if (api_initialized) {
        ApiExit();
    }
    
    std::cout << "Cikis yapildi." << std::endl;
    return 0;
}
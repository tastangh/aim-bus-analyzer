#include "Api1553.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // sleep fonksiyonu için (Linux/macOS)
// #include <windows.h> // Sleep fonksiyonu için (Windows)

// Hata kontrolü için yardımcı fonksiyon
void check_api_status(AiReturn status_code, const char* operation_name) {
    if (status_code != API_OK) {
        char error_message_buffer[256];
        const char* msg_ptr = ApiGetErrorMessage(status_code);
        if (msg_ptr) {
            snprintf(error_message_buffer, sizeof(error_message_buffer), "%s", msg_ptr);
        } else {
            snprintf(error_message_buffer, sizeof(error_message_buffer), "Bilinmeyen hata kodu, mesaj mevcut değil.");
        }
        fprintf(stderr, "BC HATA: %s basarisiz oldu! Kod: %d, Mesaj: %s\n",
                operation_name, status_code, error_message_buffer);
        // Önemli: Hata durumunda açılmışsa module_handle'ı kapatmayı düşünün.
        // Global bir değişkende handle tutuluyorsa:
        // extern AiUInt32 g_bc_module_handle; // Örnek
        // if (g_bc_module_handle != 0) ApiClose(g_bc_module_handle);
        exit(1);
    } else {
        printf("BC BASARILI: %s\n", operation_name);
    }
}

int main() {
    AiReturn api_status;
    AiUInt32 bc_module_handle = 0;
    AiUInt8 bc_biu_selection = API_BIU_1; // ApiOpenEx sonrası genellikle önemsiz, handle içinde stream bilgisi var

    printf("MIL-STD-1553 BC Uygulamasi (Standart Cerceve) Baslatiliyor...\n");

    api_status = ApiInit();
    if (api_status <= 0) {
        fprintf(stderr, "BC HATA: ApiInit basarisiz oldu veya kart bulunamadi. Donus Kodu: %d\n", api_status);
        if (api_status < 0) check_api_status(api_status, "ApiInit (Hata Durumu)");
        return 1;
    }
    printf("BC BASARILI: ApiInit - %d kart bulundu.\n", api_status);

    TY_API_OPEN bc_open_params;
    memset(&bc_open_params, 0, sizeof(TY_API_OPEN));
    bc_open_params.ul_Module = 0;
    bc_open_params.ul_Stream = 1; // BC için Stream 1
    strcpy(bc_open_params.ac_SrvName, "local");
    api_status = ApiOpenEx(&bc_open_params, &bc_module_handle);
    check_api_status(api_status, "ApiOpenEx (BC Stream)");

    TY_API_RESET_INFO bc_reset_info;
    api_status = ApiCmdReset(bc_module_handle, bc_biu_selection, API_RESET_ALL, &bc_reset_info);
    check_api_status(api_status, "ApiCmdReset (BC BIU)");

    api_status = ApiCmdBCIni(bc_module_handle, bc_biu_selection,
                             API_DIS, API_ENA, API_TBM_TRANSFER, API_BC_XFER_BUS_PRIMARY);
    check_api_status(api_status, "ApiCmdBCIni");

    // BC Arabellek Başlığını Tanımla
    TY_API_BC_BH_INFO bc_bh_info_send;
    memset(&bc_bh_info_send, 0, sizeof(TY_API_BC_BH_INFO));
    const AiUInt16 BC_SEND_HID = 1;
    const AiUInt16 BC_SEND_BID = 1; // Bu Buffer ID'ye veri yazılacak
    api_status = ApiCmdBCBHDef(bc_module_handle, bc_biu_selection,
                               BC_SEND_HID, BC_SEND_BID, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC,
                               0, 0, 0, 0, &bc_bh_info_send);
    check_api_status(api_status, "ApiCmdBCBHDef (BC Gönderme Başlığı)");

    // BC Transfer Parametrelerini Tanımla
    TY_API_BC_XFER bc_transfer_params;
    memset(&bc_transfer_params, 0, sizeof(TY_API_BC_XFER));
    const AiUInt16 TRANSFER_ID_1 = 1;

    bc_transfer_params.xid = TRANSFER_ID_1;
    bc_transfer_params.hid = BC_SEND_HID;
    bc_transfer_params.type = API_BC_TYPE_BCRT;
    bc_transfer_params.chn  = API_BC_XFER_BUS_PRIMARY;
    bc_transfer_params.rcv_rt = 5;
    bc_transfer_params.rcv_sa = 1;
    bc_transfer_params.wcnt   = 2;
    bc_transfer_params.tic = API_BC_TIC_INT_ON_XFER_END;
    bc_transfer_params.hlt = API_BC_HLT_NO_HALT;
    bc_transfer_params.rte = API_DIS;
    bc_transfer_params.sxh = API_BC_SRVW_DIS;
    bc_transfer_params.rsp = API_BC_RSP_AUTOMATIC;
    bc_transfer_params.gap_mode = API_BC_GAP_MODE_STANDARD;
    bc_transfer_params.gap = 40;
    bc_transfer_params.swxm = 0xFFFF;

    AiUInt32 desc_addr_xfer1;
    api_status = ApiCmdBCXferDef(bc_module_handle, bc_biu_selection, &bc_transfer_params, &desc_addr_xfer1);
    check_api_status(api_status, "ApiCmdBCXferDef (Transfer ID 1)");

    AiUInt16 data_to_send[32];
    memset(data_to_send, 0, sizeof(data_to_send));
    data_to_send[0] = 0xAAAA;
    data_to_send[1] = 0xBBBB;

    AiUInt16 return_buffer_id;     // rid için değişken
    AiUInt32 return_buffer_address; // raddr için değişken

    api_status = ApiCmdBufDef(bc_module_handle, bc_biu_selection, API_BUF_BC_MSG,
                              BC_SEND_HID, BC_SEND_BID,
                              bc_transfer_params.wcnt,
                              data_to_send,
                              &return_buffer_id,      // Düzeltilmiş: rid'in adresi
                              &return_buffer_address  // Düzeltilmiş: raddr'ın adresi
                             );
    check_api_status(api_status, "ApiCmdBufDef (BC Veri Yazma)");

    // Minor Frame Tanımla
    TY_API_BC_FRAME bc_minor_frame;
    memset(&bc_minor_frame, 0, sizeof(TY_API_BC_FRAME));
    bc_minor_frame.id = 1;
    bc_minor_frame.cnt = 1;
    bc_minor_frame.instr[0] = API_BC_INSTR_TRANSFER;
    bc_minor_frame.xid[0] = TRANSFER_ID_1;
    api_status = ApiCmdBCFrameDef(bc_module_handle, bc_biu_selection, &bc_minor_frame);
    check_api_status(api_status, "ApiCmdBCFrameDef (Minor Frame 1)");

    // Major Frame Tanımla (ApiCmdBCMFrameDefEx için doğru tip)
    TY_API_BC_MFRAME_EX bc_major_frame_ex; // Düzeltilmiş tip
    memset(&bc_major_frame_ex, 0, sizeof(TY_API_BC_MFRAME_EX));
    bc_major_frame_ex.cnt = 1;
    // bc_major_frame_ex.padding1; // memset ile sıfırlandı
    bc_major_frame_ex.fid[0] = bc_minor_frame.id;

    api_status = ApiCmdBCMFrameDefEx(bc_module_handle, bc_biu_selection, &bc_major_frame_ex); // Düzeltilmiş değişken
    check_api_status(api_status, "ApiCmdBCMFrameDefEx");

    // BC Operasyonunu Başlat
    AiUInt32 maj_frame_addr_out;
    // MAX_API_BC_MFRAME_EX başlık dosyasında tanımlı olmalı (genellikle Api1553i_def.h)
    // Eğer tanımlı değilse, uygun bir boyut (örneğin 512) manuel olarak girilebilir.
    // Bu örnekte sadece 1 minor frame olduğundan, dizi boyutu 1 bile yeterli olurdu.
    AiUInt32 min_frame_addr_out[MAX_API_BC_MFRAME_EX]; // AIM başlık dosyasındaki sabiti kullanın

    printf("BC: BC operasyonu baslatiliyor...\n");
    api_status = ApiCmdBCStart(bc_module_handle, bc_biu_selection,
                               API_BC_START_IMMEDIATELY,
                               1,
                               10.0f,
                               0,
                               &maj_frame_addr_out, min_frame_addr_out);
    check_api_status(api_status, "ApiCmdBCStart");

    printf("BC: Transfer baslatildi. Yanit icin 2 saniye bekleniyor...\n");
    sleep(2);

    // Transfer Durumunu Oku
    TY_API_BC_XFER_DSP xfer_stat_report;
    api_status = ApiCmdBCXferRead(bc_module_handle, bc_biu_selection, TRANSFER_ID_1, 0x7, &xfer_stat_report);
     if (api_status == API_OK) {
        printf("BC: Transfer durumu okundu.\n");
        // Durum kelimesi formatı: Bit 15 (Hata), Bit 10-5 (Kullanılmıyor), Bit 4-0 (RT Adresi)
        if ((xfer_stat_report.st1 & 0x8000) == 0 && (xfer_stat_report.st1 & 0x001F) == bc_transfer_params.rcv_rt) {
            printf("BC: RT %d'den hatasiz durum kelimesi alindi: 0x%04X\n", bc_transfer_params.rcv_rt, xfer_stat_report.st1);
        } else if ((xfer_stat_report.st1 & 0x001F) != bc_transfer_params.rcv_rt && xfer_stat_report.st1 != 0) {
             fprintf(stderr, "BC HATA: Durum kelimesinde RT adresi uyusmazligi. Beklenen RT %d, Alinan SW: 0x%04X\n",
                    bc_transfer_params.rcv_rt, xfer_stat_report.st1);
        } else if (xfer_stat_report.st1 == 0) { // st1 == 0 genellikle yanıt alınamadığı anlamına gelir
            fprintf(stderr, "BC HATA: RT'den yanit alinamadi (Durum kelimesi 0).\n");
        }
        else { // Hata biti (bit 15) set edilmişse
            fprintf(stderr, "BC HATA: RT %d'den alinan durum kelimesinde hata belirtildi: 0x%04X\n",
                    bc_transfer_params.rcv_rt, xfer_stat_report.st1);
        }
    } else {
        // ApiCmdBCXferRead başarısız olduysa (zaman aşımı veya başka bir API hatası)
        // check_api_status zaten hatayı basacaktır.
        fprintf(stderr, "BC UYARI: ApiCmdBCXferRead basarisiz oldu, RT'den yanit alinmamis olabilir.\n");
        check_api_status(api_status, "ApiCmdBCXferRead (Transfer Durumu Okuma)");
    }

    // BC Operasyonunu Durdur
    api_status = ApiCmdBCHalt(bc_module_handle, bc_biu_selection);
    check_api_status(api_status, "ApiCmdBCHalt");

    // BC Stream'ini Kapat
    api_status = ApiClose(bc_module_handle);
    check_api_status(api_status, "ApiClose (BC Stream)");

    printf("MIL-STD-1553 BC Uygulamasi (Standart Cerceve) Tamamlandi.\n");
    return 0;
}
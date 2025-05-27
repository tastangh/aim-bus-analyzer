#include "Api1553.h" 
#include <stdio.h>   
#include <string.h>  
#include <stdlib.h>  

// Hata kontrolü için yardımcı fonksiyon
void check_api_status(AiReturn status, const char* operation_name) {
    if (status != API_OK) {
        char error_message[256];
        const char* msg_ptr = ApiGetErrorMessage(status);
        if (msg_ptr) {
             snprintf(error_message, sizeof(error_message), "%s", msg_ptr);
        } else {
             snprintf(error_message, sizeof(error_message), "Bilinmeyen hata kodu için mesaj alınamadı.");
        }

        fprintf(stderr, "HATA: %s basarisiz oldu! Kod: %d, Mesaj: %s\n",
                operation_name, status, error_message);
        // Hata durumunda programı sonlandırabiliriz
        // ApiClose çağrılmadan çıkmak ideal değil ama örnek için basit tutalım.
        exit(1);
    } else {
        printf("BASARILI: %s\n", operation_name);
    }
}

int main() {
    AiReturn status;
    AiUInt32 module_handle = 0; // Başlangıç değeri
    AiUInt8 biu = API_BIU_1; // İlk Veriyolu Arayüz Birimini kullanalım

    printf("MIL-STD-1553 BC Ornegi Baslatiliyor...\n");

    // 1. Kütüphaneyi Başlat
    status = ApiInit();
    if (status <= 0) {
        fprintf(stderr, "HATA: ApiInit basarisiz oldu veya kart bulunamadi. Donus Kodu: %d\n", status);
        return 1;
    }
    printf("BASARILI: ApiInit - %d kart bulundu.\n", status);

    // 2. AIM Kartını/Arayüzünü Aç
    TY_API_OPEN open_params;
    memset(&open_params, 0, sizeof(TY_API_OPEN));
    open_params.ul_Module = 0; // İlk kart (indeks 0)
    open_params.ul_Stream = 1; // İlk akış/kanal (BIU 1'e karşılık gelir)
    // ANET cihazları için: strcpy(open_params.ac_SrvName, "local"); // veya IP adresi

    status = ApiOpenEx(&open_params, &module_handle);
    check_api_status(status, "ApiOpenEx");
    // if (status != API_OK) return 1; // check_api_status zaten hata durumunda çıkıyor

    // 3. BIU'yu Sıfırla
    TY_API_RESET_INFO reset_info;
    status = ApiCmdReset(module_handle, biu, API_RESET_ALL, &reset_info);
    check_api_status(status, "ApiCmdReset (BIU icin)");

    // 4. BC Modunu Başlat
    // HATA DÜZELTMELERİ: Sabitler düzeltildi.
    // ApiCmdBCIni prototipi: (ul_ModuleHandle, biu, retr, svrq, tbm, gsb)
    // Kılavuz Sayfa 244-245
    status = ApiCmdBCIni(module_handle, biu,
                         API_DIS,                // retr: Yeniden deneme yok (0)
                         API_ENA,                // svrq: Servis isteği yönetimi etkin (1)
                         API_TBM_TRANSFER,       // tbm: Veriyolu anahtarlama transfer bazlı (0)
                         API_BC_XFER_BUS_PRIMARY // gsb: Global başlangıç veriyolu (tbm=0 ise önemsiz olabilir, 0 (A bus) güvenli)
                        );
    check_api_status(status, "ApiCmdBCIni");

    // 5. BC Transferini Tanımla ve Veriyi Hazırla
    TY_API_BC_XFER bc_xfer_params;
    memset(&bc_xfer_params, 0, sizeof(TY_API_BC_XFER));

    bc_xfer_params.xid = 1;
    bc_xfer_params.hid = 1; // Bu fonksiyon için kritik değil ama atanmalı

    bc_xfer_params.type = API_BC_TYPE_BCRT;
    bc_xfer_params.chn  = API_BC_XFER_BUS_PRIMARY;

    bc_xfer_params.rcv_rt = 5;
    bc_xfer_params.rcv_sa = 1;
    bc_xfer_params.wcnt   = 1;

    bc_xfer_params.tic = API_BC_TIC_NO_INT;
    bc_xfer_params.hlt = API_BC_HLT_NO_HALT;
    bc_xfer_params.rte = API_DIS; // HATA DÜZELTMESİ: API_RETRY_DIS yerine API_DIS
    bc_xfer_params.sxh = API_BC_SRVW_DIS;
    bc_xfer_params.rsp = API_BC_RSP_AUTOMATIC;
    bc_xfer_params.gap_mode = API_BC_GAP_MODE_STANDARD;
    bc_xfer_params.gap = 20;
    bc_xfer_params.swxm = 0xFFFF;

    AiUInt16 data_to_send[32];
    memset(data_to_send, 0, sizeof(data_to_send));
    data_to_send[0] = 0x0457; // Decimal 1111

    // 6. Asenkron Olarak Gönder ve Yanıtı Bekle (Blocking)
    TY_API_BC_XFER_DSP transfer_status_report;
    printf("BC->RT Transferi deneniyor: RT %d, SA %d, Veri: 0x%04X (%d decimal)\n",
           bc_xfer_params.rcv_rt, bc_xfer_params.rcv_sa, data_to_send[0], data_to_send[0]);

    status = ApiCmdBCAcycPrepAndSendTransferBlocking(module_handle, biu,
                                                     &bc_xfer_params,
                                                     data_to_send,
                                                     &transfer_status_report);
    check_api_status(status, "ApiCmdBCAcycPrepAndSendTransferBlocking");

    if (status == API_OK) { // Bu kontrol check_api_status'tan sonra gereksiz olabilir ama mantıksal akış için kalabilir
        printf("Asenkron transfer denemesi tamamlandi.\n");
        if ((transfer_status_report.st1 & 0xF800) == 0 &&
            (transfer_status_report.st1 & 0x001F) == bc_xfer_params.rcv_rt) {
            printf("RT %d, SA %d'den hatasiz durum kelimesi alindi: 0x%04X\n",
                   bc_xfer_params.rcv_rt, bc_xfer_params.rcv_sa, transfer_status_report.st1);
        } else {
            fprintf(stderr, "RT %d, SA %d'den alinan durum kelimesinde hata var veya RT adresi uyusmuyor: 0x%04X\n",
                    bc_xfer_params.rcv_rt, bc_xfer_params.rcv_sa, transfer_status_report.st1);
        }
    }

    // 7. Kartı/Arayüzü Kapat
    if (module_handle != 0) { // Sadece geçerli bir handle varsa kapat
        status = ApiClose(module_handle);
        check_api_status(status, "ApiClose");
    }

    printf("MIL-STD-1553 BC Ornegi Tamamlandi.\n");
    return 0;
}
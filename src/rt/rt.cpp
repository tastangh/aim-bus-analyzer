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
        fprintf(stderr, "RT HATA: %s basarisiz oldu! Kod: %d, Mesaj: %s\n",
                operation_name, status_code, error_message_buffer);
        exit(1);
    } else {
        printf("RT BASARILI: %s\n", operation_name);
    }
}

int main() {
    AiReturn api_status;
    AiUInt32 rt_module_handle = 0;
    AiUInt8 rt_biu_selection = API_BIU_1; // ApiOpenEx sonrası genellikle önemsiz
    const AiUInt8 RT_ADDRESS = 5;
    const AiUInt8 SUBADDRESS_RECEIVE = 1;

    printf("MIL-STD-1553 RT Uygulamasi Baslatiliyor...\n");

    // 1. API Kütüphanesini Başlat
    api_status = ApiInit();
    if (api_status <= 0) {
        fprintf(stderr, "RT HATA: ApiInit basarisiz oldu veya kart bulunamadi. Donus Kodu: %d\n", api_status);
         if (api_status < 0) check_api_status(api_status, "ApiInit (Hata Durumu)");
        return 1;
    }
    printf("RT BASARILI: ApiInit - %d kart bulundu.\n", api_status);

    // 2. RT Stream'ini Aç
    TY_API_OPEN rt_open_params;
    memset(&rt_open_params, 0, sizeof(TY_API_OPEN));
    rt_open_params.ul_Module = 0;       // Aynı kart
    rt_open_params.ul_Stream = 2;       // RT için Stream 2
    strcpy(rt_open_params.ac_SrvName, "local");

    api_status = ApiOpenEx(&rt_open_params, &rt_module_handle);
    check_api_status(api_status, "ApiOpenEx (RT Stream)");

    // 3. RT BIU'sunu Sıfırla
    TY_API_RESET_INFO rt_reset_info;
    api_status = ApiCmdReset(rt_module_handle, rt_biu_selection, API_RESET_ALL, &rt_reset_info);
    check_api_status(api_status, "ApiCmdReset (RT BIU)");

    // 4. RT'yi Başlat (RT 5)
    api_status = ApiCmdRTIni(rt_module_handle, rt_biu_selection,
                             RT_ADDRESS,
                             API_RT_ENABLE_SIMULATION,
                             API_RT_RSP_BOTH_BUSSES, // Her iki veriyolundan da yanıt ver
                             8.0f,                   // Yanıt süresi (örneğin 8µs)
                             (RT_ADDRESS << 11)      // nxw: Sonraki durum kelimesi (sadece RT adresi)
                            );
    check_api_status(api_status, "ApiCmdRTIni (RT 5 icin)");

    // 5. RT Arabellek Başlığını Tanımla (Receive SA 1 için)
    TY_API_RT_BH_INFO rt_bh_info_sa1;
    memset(&rt_bh_info_sa1, 0, sizeof(TY_API_RT_BH_INFO));
    const AiUInt16 RT_RECEIVE_HID = 101; // BC'deki hid'den farklı olabilir, RT kendi başlıklarını yönetir
    const AiUInt16 RT_RECEIVE_BID = 101; // RT'nin veriyi saklayacağı arabellek ID'si

    api_status = ApiCmdRTBHDef(rt_module_handle, rt_biu_selection,
                               RT_RECEIVE_HID, RT_RECEIVE_BID,
                               0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0,
                               &rt_bh_info_sa1);
    check_api_status(api_status, "ApiCmdRTBHDef (RT5, SA1 Alim Basligi)");

    // 6. RT Subaddress'ini (SA 1) Alıcı Olarak Yapılandır
    api_status = ApiCmdRTSACon(rt_module_handle, rt_biu_selection,
                               RT_ADDRESS,
                               SUBADDRESS_RECEIVE,
                               RT_RECEIVE_HID, // Yukarıda tanımlanan RT arabellek başlığı
                               API_RT_TYPE_RECEIVE_SA,
                               API_RT_ENABLE_SA, // SA'yı etkinleştir, kesme yok
                               0,                // rmod
                               API_RT_SWM_OR,    // smod
                               (RT_ADDRESS << 11) // swm: Varsayılan durum kelimesi
                              );
    check_api_status(api_status, "ApiCmdRTSACon (RT5, SA1 Receive)");

    // 7. RT Operasyonunu Başlat
    api_status = ApiCmdRTStart(rt_module_handle, rt_biu_selection);
    check_api_status(api_status, "ApiCmdRTStart");

    printf("RT %d, SA %d alim icin dinlemede...\n", RT_ADDRESS, SUBADDRESS_RECEIVE);

    // Gelen veriyi periyodik olarak kontrol edip yazdıralım
    AiUInt16 received_data[32];
    TY_API_RT_SA_MSG_DSP rt_sa_msg_status;

    for (int i = 0; i < 10; ++i) { // 10 saniye boyunca kontrol et
        sleep(1); // 1 saniye bekle

        // SA 1 için mesaj durumunu oku
        // ApiCmdRTSAMsgRead prototipi: (ul_ModuleHandle, biu, rt_addr, sa, sa_type, clr, *psa_dsp)
        api_status = ApiCmdRTSAMsgRead(rt_module_handle, rt_biu_selection,
                                       RT_ADDRESS, SUBADDRESS_RECEIVE, API_RT_TYPE_RECEIVE_SA,
                                       1, // clr: Durum bitlerini oku ve temizle (1)
                                       &rt_sa_msg_status);
        if (api_status == API_OK) {
            // trw (Transfer Report Word) BUF_STAT alanını kontrol et
            // BUF_STAT: 0 = Not Used, 1 = Full, 2 = Empty
            if ((rt_sa_msg_status.trw & 0x00E0) >> 5 == API_BUF_FULL) { // BUF_STAT'ı izole et
                printf("RT: SA %d'den veri alindi. Buffer ID: %d\n", SUBADDRESS_RECEIVE, rt_sa_msg_status.bid);

                // Arabellekten veriyi oku
                api_status = ApiCmdBufRead(rt_module_handle, rt_biu_selection,
                                           API_BUF_RT_MSG,       // bt: RT mesaj arabelleği
                                           RT_RECEIVE_HID,       // hid: Arabellek Başlık ID'si
                                           rt_sa_msg_status.bid, // bid: Okunacak arabellek ID'si
                                           2,                    // len: Okunacak kelime sayısı
                                           received_data,        // data
                                           NULL, NULL            // rid, raddr (çıkış, burada gerekmiyor)
                                          );
                check_api_status(api_status, "ApiCmdBufRead (RT Veri Okuma)");
                printf("RT: Alinan veri[0]: 0x%04X, Alinan veri[1]: 0x%04X\n", received_data[0], received_data[1]);
            }
        } else if (api_status != API_ERR_QUEUE_EMPTY) { // Kuyruk boş hatası normalse, diğerlerini raporla
             check_api_status(api_status, "ApiCmdRTSAMsgRead");
        }
    }

    // 8. RT Operasyonunu Durdur
    api_status = ApiCmdRTHalt(rt_module_handle, rt_biu_selection);
    check_api_status(api_status, "ApiCmdRTHalt");

    // 9. RT Stream'ini Kapat
    api_status = ApiClose(rt_module_handle);
    check_api_status(api_status, "ApiClose (RT Stream)");

    printf("MIL-STD-1553 RT Uygulamasi Tamamlandi.\n");
    return 0;
}
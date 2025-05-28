#include "Api1553.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // sleep fonksiyonu için (Linux/macOS)
// #include <windows.h> // Sleep fonksiyonu için (Windows)
#include <vector>    // Dinamik boyutlu arabellek için

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
        fprintf(stderr, "BM HATA: %s basarisiz oldu! Kod: %d, Mesaj: %s\n",
                operation_name, status_code, error_message_buffer);
        exit(1);
    } else {
        printf("BM BASARILI: %s\n", operation_name);
    }
}

// BM Veri Kuyruğu'ndan okunan veriyi ayrıştırmak ve yazdırmak için fonksiyon
void parse_and_print_bm_data(const std::vector<AiUInt32>& buffer, AiUInt32 bytes_read) {
    if (bytes_read == 0) {
        // printf("BM: Kuyrukta yeni veri yok.\n");
        return;
    }
    if (bytes_read % 4 != 0) {
        fprintf(stderr, "BM UYARI: Okunan byte sayısı 4'ün katı değil (%u byte), ayrıştırma sorunlu olabilir.\n", bytes_read);
        // return; // İsteğe bağlı olarak burada çıkılabilir veya devam edilebilir
    }

    printf("\n--- BM Veri Blogu (Uzunluk: %u byte) ---\n", bytes_read);

    for (AiUInt32 i = 0; i < bytes_read / 4; ++i) {
        AiUInt32 monitor_word = buffer[i];
        AiUInt8 type = (monitor_word >> 28) & 0x0F; // Bit 31-28
        AiUInt8 c_flag = (monitor_word >> 27) & 0x01; // Bit 27
        AiUInt32 entry = monitor_word & 0x07FFFFFF; // Bit 26-0

        printf("Monitor Kelimesi[%u]: 0x%08X (Tip: 0x%X, C:%d, Giriş: 0x%07X)\n", i, monitor_word, type, c_flag, entry);

        // Referans Kılavuzu Bölüm 8.6.1'e göre tipi yorumla
        switch (type) {
            case 0x0: // Entry not updated
                printf("  Tip: Guncellenmemis Giris\n");
                break;
            case 0x1: // Error Word Entry
                {
                    printf("  Tip: Hata Kelimesi Girisi\n");
                    AiUInt8 s_flag = (entry >> 26) & 0x01;
                    // AiUInt8 reserved_error = (entry >> 18) & 0xFF; // Bit 25-18
                    AiUInt8 t_flag = (entry >> 17) & 0x01;
                    AiUInt8 r_flag = (entry >> 16) & 0x01;
                    AiUInt16 error_bits = entry & 0xFFFF;
                    printf("    S:%d, T:%d, R:%d, Hata Bitleri: 0x%04X\n", s_flag, t_flag, r_flag, error_bits);
                    // Hata bitlerini daha detaylı ayrıştırabilirsiniz (Ref Man. 8.6.5)
                }
                break;
            case 0x2: // Time Tag Low Entry
                {
                    printf("  Tip: Zaman Etiketi Dusuk Kelime\n");
                    // AiUInt8 r_tt_low = (entry >> 26) & 0x01; // Bit 26
                    AiUInt16 seconds = (entry >> 20) & 0x3F; // Bit 25-20
                    AiUInt32 microseconds = entry & 0xFFFFF; // Bit 19-0
                    printf("    Saniye: %u, Mikrosaniye: %u\n", seconds, microseconds);
                }
                break;
            case 0x3: // Time Tag High Entry
                {
                    printf("  Tip: Zaman Etiketi Yuksek Kelime\n");
                    // AiUInt16 r_tt_high = (entry >> 20) & 0x7F; // Bit 26-20
                    AiUInt16 days = (entry >> 11) & 0x1FF; // Bit 19-11
                    AiUInt8 hours = (entry >> 6) & 0x1F;   // Bit 10-6
                    AiUInt8 minutes = entry & 0x3F;        // Bit 5-0
                    printf("    Gun: %u, Saat: %u, Dakika: %u\n", days, hours, minutes);
                }
                break;
            // 0x8'den 0xF'ye kadar olanlar Bus Word Entry'leridir
            case 0x8: // Command Word on Primary Bus
            case 0x9: // Command Word 2 on Primary Bus
            case 0xA: // Data Word on Primary Bus
            case 0xB: // Status Word on Primary Bus
            case 0xC: // Command Word on Secondary Bus
            case 0xD: // Command Word 2 on Secondary Bus
            case 0xE: // Data Word on Secondary Bus
            case 0xF: // Status Word on Secondary Bus
                {
                    printf("  Tip: Veriyolu Kelimesi Girisi (");
                    if (type >= 0xC) printf("Ikincil Bus, "); else printf("Birincil Bus, ");
                    if (type == 0x8 || type == 0xC) printf("Komut Kelimesi)\n");
                    else if (type == 0x9 || type == 0xD) printf("Komut Kelimesi 2)\n");
                    else if (type == 0xA || type == 0xE) printf("Veri Kelimesi)\n");
                    else if (type == 0xB || type == 0xF) printf("Durum Kelimesi)\n");

                    AiUInt8 s_flag_bus = (entry >> 26) & 0x01;
                    // AiUInt8 r_bus = (entry >> 25) & 0x01;
                    AiUInt16 gap_time_raw = (entry >> 16) & 0x1FF; // Bit 24-16 (9 bit)
                    AiUInt16 bus_word_data = entry & 0xFFFF;
                    float gap_time_us = gap_time_raw * 0.25f;
                    if (gap_time_raw == 0 && bus_word_data != 0) gap_time_us = 2.0f; // Varsayılan ardışık kelime boşluğu

                    printf("    S_Flag:%d, Gap: %.2fus (0x%X), Veriyolu Kelimesi: 0x%04X\n",
                           s_flag_bus, gap_time_us, gap_time_raw, bus_word_data);
                }
                break;
            default:
                printf("  Tip: Bilinmeyen veya Ayrıştırılmamış (0x%X)\n", type);
                break;
        }
    }
    printf("--- BM Veri Blogu Sonu ---\n");
}


int main() {
    AiReturn api_status;
    AiUInt32 bm_module_handle = 0;
    AiUInt8 bm_biu_selection = API_BIU_1; // ApiOpenEx sonrası genellikle önemsiz
    AiUInt32 data_queue_id = API_DATA_QUEUE_ID_BM_REC_BIU1; // Stream 1'i izlemek için
                                                        // Eğer BC Stream 1'de, RT Stream 2'de ise
                                                        // ve ikisini birden izlemek istiyorsanız,
                                                        // donanımınızın ve API'nin bunu nasıl desteklediğini
                                                        // kontrol etmeniz gerekir. Genellikle bir stream
                                                        // sadece bir BIU'ya atanır. Loopback yapıyorsanız
                                                        // ve BM'yi aynı loopback'e bağlarsanız,
                                                        // her iki stream'in trafiğini de görebilirsiniz.

    printf("MIL-STD-1553 Bus Monitor Uygulamasi Baslatiliyor...\n");

    // 1. API Kütüphanesini Başlat
    api_status = ApiInit();
    if (api_status <= 0) {
        fprintf(stderr, "BM HATA: ApiInit basarisiz oldu veya kart bulunamadi. Donus Kodu: %d\n", api_status);
        if (api_status < 0) check_api_status(api_status, "ApiInit (Hata Durumu)");
        return 1;
    }
    printf("BM BASARILI: ApiInit - %d kart bulundu.\n", api_status);

    // 2. BM Stream'ini Aç
    TY_API_OPEN bm_open_params;
    memset(&bm_open_params, 0, sizeof(TY_API_OPEN));
    bm_open_params.ul_Module = 0;       // İlk kart
    bm_open_params.ul_Stream = 3;       // BM için Stream 3 (BC için 1, RT için 2 kullanıldığını varsayarak)
                                        // Eğer sadece BM çalıştırıyorsanız Stream 1 de olabilir.
    strcpy(bm_open_params.ac_SrvName, "local");

    api_status = ApiOpenEx(&bm_open_params, &bm_module_handle);
    check_api_status(api_status, "ApiOpenEx (BM Stream)");

    // 3. BM BIU'sunu Sıfırla
    TY_API_RESET_INFO bm_reset_info;
    api_status = ApiCmdReset(bm_module_handle, bm_biu_selection, API_RESET_ALL, &bm_reset_info);
    check_api_status(api_status, "ApiCmdReset (BM BIU)");

    // 4. Bus Monitor'ü Başlat
    api_status = ApiCmdBMIni(bm_module_handle, bm_biu_selection);
    check_api_status(api_status, "ApiCmdBMIni");

    // 5. BM Yakalama Modunu Ayarla (Recording with Data Queue için özel bir ayar yok, varsayılan iyi olmalı)
    // Genellikle API_BM_CAPMODE_ALL (Standart Yakalama) veya API_BM_CAPMODE_RECORDING (Sürekli Kayıt) kullanılır.
    // Data Queue kullanımı için API_BM_CAPMODE_RECORDING daha uygun olabilir.
    TY_API_BM_CAP_SETUP cap_setup;
    memset(&cap_setup, 0, sizeof(cap_setup));
    cap_setup.cap_mode = API_BM_CAPMODE_RECORDING; // Sürekli kayıt ve veri kuyruğuna aktarım için
    // cap_setup.cap_tat = 0; // Sürekli kayıt için 0
    // cap_setup.cap_mcc = 0; // Sürekli kayıt için 0
    // cap_setup.cap_fsize = 0; // Kullanılmıyor

    api_status = ApiCmdBMCapMode(bm_module_handle, bm_biu_selection, &cap_setup);
    check_api_status(api_status, "ApiCmdBMCapMode");


    // 6. Veri Kuyruğunu Aç
    AiUInt32 queue_size_bytes = 0;
    api_status = ApiCmdDataQueueOpen(bm_module_handle, data_queue_id, &queue_size_bytes);
    check_api_status(api_status, "ApiCmdDataQueueOpen");
    printf("BM: Veri kuyrugu ID %u, boyut %u byte olarak acildi.\n", data_queue_id, queue_size_bytes);
    if (queue_size_bytes == 0) {
        fprintf(stderr, "BM HATA: Veri kuyrugu boyutu 0, bu bir sorun olabilir.\n");
        // return 1; // veya devam et
    }

    // 7. Veri Kuyruğu Yakalamayı Başlat
    api_status = ApiCmdDataQueueControl(bm_module_handle, data_queue_id, API_DATA_QUEUE_CTRL_MODE_START);
    check_api_status(api_status, "ApiCmdDataQueueControl (START)");

    // 8. Bus Monitor Kaydını Başlat
    api_status = ApiCmdBMStart(bm_module_handle, bm_biu_selection);
    check_api_status(api_status, "ApiCmdBMStart");

    printf("BM dinlemede... (Ctrl+C ile durdurun)\n");

    std::vector<AiUInt32> read_buffer_vector(queue_size_bytes / 4 + 1); // En azından bir tam kuyruk kadar yer ayıralım + biraz
    TY_API_DATA_QUEUE_READ data_queue_read_params;
    TY_API_DATA_QUEUE_STATUS queue_status_info;

    // Veriyolunu izle ve verileri oku
    for (int loop_count = 0; loop_count < 60; ++loop_count) { // Örnek olarak 60 saniye
        data_queue_read_params.id = data_queue_id;
        data_queue_read_params.buffer = read_buffer_vector.data();
        data_queue_read_params.bytes_to_read = queue_size_bytes; // Mümkün olduğunca çok oku

        api_status = ApiCmdDataQueueRead(bm_module_handle, &data_queue_read_params, &queue_status_info);

        if (api_status == API_OK) {
            // printf("BM: Okunan byte: %u, Kuyruktaki byte: %u, Toplam transfer: %llu\n",
            //        queue_status_info.bytes_transfered,
            //        queue_status_info.bytes_in_queue,
            //        queue_status_info.total_bytes_transfered);
            if (queue_status_info.bytes_transfered > 0) {
                parse_and_print_bm_data(read_buffer_vector, queue_status_info.bytes_transfered);
            }
        } else if (api_status == API_ERR_QUEUE_EMPTY) {
            // Kuyruk boş, sorun değil, polling yapıyoruz.
        }
        else {
            check_api_status(api_status, "ApiCmdDataQueueRead");
        }
        sleep(1); // 1 saniye aralıklarla kontrol et
    }

    // 9. Bus Monitor Kaydını Durdur
    api_status = ApiCmdBMHalt(bm_module_handle, bm_biu_selection);
    check_api_status(api_status, "ApiCmdBMHalt");

    // 10. Veri Kuyruğu Yakalamayı Durdur
    api_status = ApiCmdDataQueueControl(bm_module_handle, data_queue_id, API_DATA_QUEUE_CTRL_MODE_STOP);
    check_api_status(api_status, "ApiCmdDataQueueControl (STOP)");

    // 11. Veri Kuyruğunu Kapat
    api_status = ApiCmdDataQueueClose(bm_module_handle, data_queue_id);
    check_api_status(api_status, "ApiCmdDataQueueClose");

    // 12. BM Stream'ini Kapat
    api_status = ApiClose(bm_module_handle);
    check_api_status(api_status, "ApiClose (BM Stream)");

    printf("MIL-STD-1553 Bus Monitor Uygulamasi Tamamlandi.\n");
    return 0;
}
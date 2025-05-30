#include "Api1553.h"
#include "bm.hpp" 
#include <stdio.h>
#include <cstring> // memset ve strcpy için
#include <string>  
#include <vector>  // Dinamik buffer için
#include <thread>  
#include <chrono>  
#include <inttypes.h> 

AiReturn configureHardware(AiUInt32 ulModHandle, AiUInt32 streamId, AiUInt8 coupling);
void parseAndPrintBMData(const unsigned char* buffer, AiUInt32 bytesRead); 

// Global handle, hata durumunda kapatmak için
static AiUInt32 ulModHandle = 0;

int main( int argc, char *argv[ ] )
{
    ConfigStruct         xConfig;

    printf("*******************************************************************************\n");
    printf("***                           MIL-STD-1553 BUS MONITOR                      ***\n");
    printf("***                         Copyright (c) Turkish Aerospace                 ***\n");
    printf("*******************************************************************************\n");

    xConfig.ulDevice = 0;
    xConfig.ulStream = 1;
    xConfig.ucUseServer = 0;
    strcpy(xConfig.acServer, "local");
    xConfig.ulCoupling = API_CAL_CPL_TRANSFORM;

    TY_API_RESET_INFO    xApiResetInfo;
    TY_API_OPEN          xApiOpen;
    AiUInt8             ucResetMode           = API_RESET_ALL;

    memset( &xApiResetInfo, 0, sizeof(xApiResetInfo) );
    memset( &xApiOpen,      0, sizeof(xApiOpen)      );

    AiReturn ret = API_OK;

    ret = ApiInit();
    if (ret <= 0) { // Bulunan kart sayısı veya hata
        printf("ApiInit failed or no boards found. Return value: %d\n", ret);
        return ret;
    }
    printf("ApiInit successful, found %d board(s).\n", ret);

    xApiOpen.ul_Module = xConfig.ulDevice;
    xApiOpen.ul_Stream = xConfig.ulStream;
    strncpy(xApiOpen.ac_SrvName, xConfig.acServer, sizeof(xApiOpen.ac_SrvName) - 1);
    xApiOpen.ac_SrvName[sizeof(xApiOpen.ac_SrvName) - 1] = '\0';

    ret = ApiOpenEx( &xApiOpen, &ulModHandle );
    AIM_CHECK_ERROR(ret, "ApiOpenEx");
    printf("ApiOpenEx successful. ModuleHandle: 0x%X\n", ulModHandle);

    ret = ApiCmdReset(ulModHandle, (AiUInt8)xConfig.ulStream, ucResetMode, &xApiResetInfo);
    AIM_CHECK_ERROR(ret, "ApiCmdReset");
    printf("ApiCmdReset successful.\n");

    ret = configureHardware(ulModHandle, xConfig.ulStream, xConfig.ulCoupling);
    AIM_CHECK_ERROR(ret, "configureHardware");
    printf("configureHardware successful.\n");

    // --- Bus Monitor Başlatma Adımları ---
    printf("Initializing Bus Monitor...\n");
    ret = ApiCmdBMIni(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMIni");
    printf("ApiCmdBMIni successful.\n");

    TY_API_BM_CAP_SETUP bmCapSetup;
    memset(&bmCapSetup, 0, sizeof(bmCapSetup));
    bmCapSetup.cap_mode = API_BM_CAPMODE_RECORDING; // Sürekli kayıt için
    // bmCapSetup.cap_tat = 0; // API_BM_CAPMODE_RECORDING için 0 olmalı
    // bmCapSetup.cap_fsize = 0; // Kullanılmıyor (referans kılavuzu)

    printf("Configuring BM Capture Mode...\n");
    ret = ApiCmdBMCapMode(ulModHandle, (AiUInt8)xConfig.ulStream, &bmCapSetup);
    AIM_CHECK_ERROR(ret, "ApiCmdBMCapMode");
    printf("ApiCmdBMCapMode successful.\n");

    // Data Queue Ayarları
    AiUInt32 dataQueueId = API_DATA_QUEUE_ID_BM_REC_BIU1; // Stream 1 için
    if (xConfig.ulStream == 2) dataQueueId = API_DATA_QUEUE_ID_BM_REC_BIU2; // Örnek, stream'e göre ID seçimi
    // Diğer stream ID'leri için kılavuza bakın
    AiUInt32 queueSize = 0;
    TY_API_DATA_QUEUE_READ queueReadParams;
    TY_API_DATA_QUEUE_STATUS queueStatus;
    std::vector<unsigned char> rxBuffer; // Dinamik boyutlu buffer
    const AiUInt32 READ_CHUNK_SIZE = 16 * 1024; // Her seferinde okunacak chunk boyutu (örneğin 16KB)

    printf("Opening Data Queue (ID: %u)...\n", dataQueueId);
    ret = ApiCmdDataQueueOpen(ulModHandle, dataQueueId, &queueSize);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueOpen");
    if (queueSize == 0) {
        printf("Error: Data Queue size is 0. Cannot proceed.\n");
        // Hata yönetimi... ApiClose ve ApiExit çağrılmalı
        if (ulModHandle != 0) ApiClose(ulModHandle);
        ApiExit();
        return -1;
    }
    printf("Data Queue opened. Onboard ASP Queue Size: %u bytes\n", queueSize);
    rxBuffer.resize(READ_CHUNK_SIZE); // Okuma buffer'ını ayarla

    printf("Starting Data Queue Control...\n");
    ret = ApiCmdDataQueueControl(ulModHandle, dataQueueId, API_DATA_QUEUE_CTRL_MODE_START);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueControl - START");
    printf("Data Queue started for BM recording.\n");

    printf("Starting Bus Monitor data capture...\n");
    ret = ApiCmdBMStart(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMStart");
    printf("Bus Monitor started. Monitoring bus traffic. Press Enter to stop...\n");

    // Ana veri okuma döngüsü
    // Bu döngü, Enter tuşuna basılana kadar veya bir hata oluşana kadar çalışır
    bool monitoringActive = true;
    while (monitoringActive) {
    
        memset(&queueReadParams, 0, sizeof(queueReadParams));
        queueReadParams.id = dataQueueId;
        queueReadParams.buffer = rxBuffer.data();
        queueReadParams.bytes_to_read = READ_CHUNK_SIZE;

        memset(&queueStatus, 0, sizeof(queueStatus));

        ret = ApiCmdDataQueueRead(ulModHandle, &queueReadParams, &queueStatus);
        if (ret != API_OK) {
            AIM_CHECK_ERROR(ret, "ApiCmdDataQueueRead"); // Makro çıkış yapacak
            monitoringActive = false; // Hata durumunda döngüden çık
            break;
        }

        if (queueStatus.bytes_transfered > 0) {
            printf("Read %u bytes. In queue: %u. Total transferred since open: %" PRIu64 "\n",
                queueStatus.bytes_transfered,
                queueStatus.bytes_in_queue,
                queueStatus.total_bytes_transfered);
            parseAndPrintBMData(rxBuffer.data(), queueStatus.bytes_transfered);
        }

        // Döngünün çok hızlı dönmemesi için kısa bir bekleme
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }


    // --- Bus Monitor Durdurma ve Kapatma Adımları ---
    printf("Stopping Bus Monitor data capture...\n");
    ret = ApiCmdBMHalt(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMHalt");
    printf("Bus Monitor halted.\n");

    printf("Stopping Data Queue Control...\n");
    ret = ApiCmdDataQueueControl(ulModHandle, dataQueueId, API_DATA_QUEUE_CTRL_MODE_STOP);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueControl - STOP");
    printf("Data Queue stopped.\n");

    printf("Closing Data Queue...\n");
    ret = ApiCmdDataQueueClose(ulModHandle, dataQueueId);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueClose");
    printf("Data Queue closed.\n");

    // --- Kapatma İşlemleri ---
    printf("Bus Monitor Operations Complete.\n");

    if (ulModHandle != 0) {
        ret = ApiClose(ulModHandle);
        AIM_CHECK_ERROR(ret, "ApiClose"); // Bu satır ApiClose sonrası ulModHandle sıfırlanmadan önce olmalı
        printf("ApiClose successful.\n");
        ulModHandle = 0; // Handle kapatıldıktan sonra sıfırla
    }

    ApiExit();
    printf("ApiExit called.\n");

    printf("Bus Monitor Terminated.\n");
    return 0;
}

AiReturn configureHardware(AiUInt32 modHandle, AiUInt32 streamId, AiUInt8 coupling){ // ulModHandle ismi değiştirildi
    AiReturn ret = 0;
    printf("Configuring hardware for coupling mode: %d on stream %d\n", coupling, streamId);

    ret = ApiCmdCalCplCon(modHandle, (AiUInt8)streamId, API_CAL_BUS_PRIMARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Primary");

    ret = ApiCmdCalCplCon(modHandle, (AiUInt8)streamId, API_CAL_BUS_SECONDARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Secondary");

    printf("Hardware coupling configured successfully.\n");
    return API_OK;
}

// Bus Monitor verilerini parse edip yazdırmak için basit bir fonksiyon (Detaylandırılmalı)
void parseAndPrintBMData(const unsigned char* buffer, AiUInt32 bytesRead) {
    printf("--- Parsing BM Data (%u bytes) ---\n", bytesRead);
    // Referans Kılavuzu Bölüm 8.6 (Format of Data Stored in the 1553 Monitor Buffer)
    // Her bir Monitor Word 32-bit (4 byte) uzunluğundadır.
    AiUInt32 numWords = bytesRead / 4;
    const AiUInt32* pMonitorWords = (const AiUInt32*)buffer;

    for (AiUInt32 i = 0; i < numWords; ++i) {
        AiUInt32 monitorWord = pMonitorWords[i];
        AiUInt8 type = (monitorWord >> 28) & 0x0F; // Bit 31-28 Type
        AiUInt8 cFlag = (monitorWord >> 27) & 0x01; // Bit 27 C (Connection) Flag
        AiUInt32 entry = monitorWord & 0x07FFFFFF; // Bit 26-0 Entry

        printf("  Word %2u: Raw=0x%08X, Type=0x%X, C=%u, Entry=0x%07X\n", i, monitorWord, type, cFlag, entry);

        if (type == 0x8 || type == 0xC) { // Command Word (Primary or Secondary)
            // Entry'nin alt 16 biti komut kelimesidir
            AiUInt16 cmdWord = entry & 0xFFFF;
            AiUInt8 rtAddr = (cmdWord >> 11) & 0x1F;
            AiUInt8 tr = (cmdWord >> 10) & 0x01;
            AiUInt8 sa_mc = (cmdWord >> 5) & 0x1F;
            AiUInt8 wc_mc = cmdWord & 0x1F;
            printf("    CMD: RT=%02u, T/R=%u, SA/MC=%02u, WC/MC=%02u (0x%04X)\n",
                   rtAddr, tr, sa_mc, wc_mc, cmdWord);
        }
    }
    printf("--- End of BM Data Parse ---\n");
}
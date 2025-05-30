#include "Api1553.h"
#include <stdio.h>
#include <cstring> // memset ve strcpy için
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <inttypes.h> // PRIu64 için

// bm.hpp içinde değilse ConfigStruct tanımı:
#ifndef BM_HPP_CONTENT // Çift tanımı önlemek için bir kontrol
#define BM_HPP_CONTENT
typedef struct ConfigStruct
{
  AiUInt32 ulDevice;
  AiUInt32 ulStream;
  AiUInt8  ulCoupling; // AiUInt32'den AiUInt8'e düzeltildi
  AiUInt8  ucUseServer;
  AiChar   acServer[28]; // TY_API_OPEN.ac_SrvName ile uyumlu
} ConfigStruct;

// Global handle, hata durumunda kapatmak için
static AiUInt32 ulModHandle = 0;

// Basit bir hata kontrol makrosu
#define AIM_CHECK_ERROR(retVal, funcName) \
    if (retVal != API_OK) { \
        const char* errMsgPtr = ApiGetErrorMessage(retVal); \
        printf("ERROR in %s: %s (0x%04X) at %s:%d\n", funcName, errMsgPtr ? errMsgPtr : "Unknown Error", retVal, __FILE__, __LINE__); \
        if (ulModHandle != 0) { \
            printf("Attempting to close module handle 0x%X due to error.\n", ulModHandle); \
            ApiClose(ulModHandle); \
            ulModHandle = 0; \
        } \
        printf("Exiting API due to error.\n"); \
        ApiExit(); \
        return retVal; \
    }
#endif // BM_HPP_CONTENT


AiReturn configureHardware(AiUInt32 modHandle, AiUInt32 streamId, AiUInt8 coupling);
void parseAndPrintBMData(const unsigned char* buffer, AiUInt32 bytesRead);

// Yardımcı Fonksiyon: 1553 Komut Kelimesini Ayrıştırma
void parseCommandWord(AiUInt16 cmdWord) {
    AiUInt8 rtAddr = (cmdWord >> 11) & 0x1F;
    AiUInt8 tr     = (cmdWord >> 10) & 0x01;
    AiUInt8 sa_mc  = (cmdWord >> 5)  & 0x1F;
    AiUInt8 wc_mc  = cmdWord & 0x1F;

    printf("      CMD: RT=%02u, T/R=%u, ", rtAddr, tr);
    if (sa_mc == 0 || sa_mc == 31) { // Mode Code
        printf("MC=%02u (Mode Code Op %u)\n", sa_mc, wc_mc);
    } else { // Subaddress
        printf("SA=%02u, WC=%02u\n", sa_mc, wc_mc);
    }
}

// Yardımcı Fonksiyon: 1553 Durum Kelimesini Ayrıştırma
void parseStatusWord(AiUInt16 statusWord) {
    AiUInt8 rtAddr        = (statusWord >> 11) & 0x1F;
    AiUInt8 msgError      = (statusWord >> 10) & 0x01;
    AiUInt8 instBit       = (statusWord >> 9)  & 0x01;
    AiUInt8 srvReqBit     = (statusWord >> 8)  & 0x01;
    // Bits 7-4 reserved
    AiUInt8 bcastRcvdBit  = (statusWord >> 3)  & 0x01;
    AiUInt8 busyBit       = (statusWord >> 2)  & 0x01;
    AiUInt8 subSysFlagBit = (statusWord >> 1)  & 0x01;
    AiUInt8 termFlagBit   = statusWord & 0x01;

    printf("      STA: RT=%02u, ME=%u, INST=%u, SRQ=%u, BCAST_RX=%u, BUSY=%u, SSF=%u, TF=%u (0x%04X)\n",
           rtAddr, msgError, instBit, srvReqBit, bcastRcvdBit, busyBit, subSysFlagBit, termFlagBit, statusWord);
}

// Yardımcı Fonksiyon: 1553 Veri Kelimesini Yazdırma
void parseDataWord(AiUInt16 dataWord) {
    printf("      DAT: 0x%04X (%u)\n", dataWord, dataWord);
}

// Yardımcı Fonksiyon: Bus Word Entry Ayrıştırma (Tip 0x8 - 0xF)
void parseBusWordEntry(AiUInt32 entryData, AiUInt8 type) {
    // Bölüm 8.6.2'ye göre
    // AiUInt8 startTriggerFlag = (entryData >> 26) & 0x01; // S
    // AiUInt8 reservedGap    = (entryData >> 25) & 0x01; // R
    // AiUInt16 gapTimeValue    = (entryData >> 16) & 0x1FF; // Bit 24-16 Gap
    AiUInt16 busWord         = entryData & 0xFFFF;      // Bit 15-0 Bus Word

    switch (type) {
        case 0x8: printf("    CW1 (Pri): "); parseCommandWord(busWord); break;
        case 0x9: printf("    CW2 (Pri): "); parseCommandWord(busWord); break; // RT-RT için
        case 0xA: printf("    DW  (Pri): "); parseDataWord(busWord);    break;
        case 0xB: printf("    SW  (Pri): "); parseStatusWord(busWord);  break;
        case 0xC: printf("    CW1 (Sec): "); parseCommandWord(busWord); break;
        case 0xD: printf("    CW2 (Sec): "); parseCommandWord(busWord); break; // RT-RT için
        case 0xE: printf("    DW  (Sec): "); parseDataWord(busWord);    break;
        case 0xF: printf("    SW  (Sec): "); parseStatusWord(busWord);  break;
        default: printf("    Unknown Bus Word Type for parsing\n"); break;
    }
}

// Yardımcı Fonksiyon: Error Word Entry Ayrıştırma (Tip 0x1)
void parseErrorWordEntry(AiUInt32 entryData) {
    // Bölüm 8.6.5'e göre
    AiUInt8 transmitRTFlag   = (entryData >> 17) & 0x01; // T
    AiUInt8 receiveRTFlag    = (entryData >> 16) & 0x01; // R
    AiUInt16 errorType       = entryData & 0xFFFF;      // Bit 15-0 Error

    printf("    ERR: T_RT=%u, R_RT=%u, ErrorMask=0x%04X\n", transmitRTFlag, receiveRTFlag, errorType);
    if (errorType & (1 << 15)) printf("      ANY_ERROR\n");
    if (errorType & (1 << 14)) printf("      Alternate Bus Response Error\n");
    if (errorType & (1 << 13)) printf("      Low Word Count Error\n");
    if (errorType & (1 << 12)) printf("      High Word Count Error\n");
    if (errorType & (1 << 11)) printf("      Status Word Exception\n");
    if (errorType & (1 << 10)) printf("      Terminal Address Error / RT-RT Protocol Error\n");
    if (errorType & (1 << 9))  printf("      Early Response or Gap Too Short Error\n");
    if (errorType & (1 << 8))  printf("      Illegal Command Word / Reserved Mode Code Error\n");
    if (errorType & (1 << 7))  printf("      Transmission on Both MILbus Channels\n");
    if (errorType & (1 << 6))  printf("      Interword Gap Error\n");
    if (errorType & (1 << 5))  printf("      Inverted Sync Error\n");
    if (errorType & (1 << 4))  printf("      Parity Error\n");
    if (errorType & (1 << 3))  printf("      Low Bit Count Error\n");
    if (errorType & (1 << 2))  printf("      High Bit Count Error\n");
    if (errorType & (1 << 1))  printf("      Manchester Coding Error\n");
    if (errorType & (1 << 0))  printf("      Terminal No Response Error\n");
}

// Yardımcı Fonksiyon: Time Tag Low Entry Ayrıştırma (Tip 0x2)
void parseTimeTagLowEntry(AiUInt32 entryData) {
    AiUInt32 seconds      = (entryData >> 20) & 0x3F;
    AiUInt32 microseconds = entryData & 0xFFFFF;
    printf("    TT_L: Seconds=%02u, Microseconds=%06u\n", seconds, microseconds);
}

// Yardımcı Fonksiyon: Time Tag High Entry Ayrıştırma (Tip 0x3)
void parseTimeTagHighEntry(AiUInt32 entryData) {
    AiUInt32 days    = (entryData >> 11) & 0x1FF;
    AiUInt32 hours   = (entryData >> 6)  & 0x1F;
    AiUInt32 minutes = entryData & 0x3F;
    printf("    TT_H: DayOfYear=%03u, Hours=%02u, Minutes=%02u\n", days, hours, minutes);
}

void parseAndPrintBMData(const unsigned char* buffer, AiUInt32 bytesRead) {
    printf("--- Parsing BM Data (%u bytes) ---\n", bytesRead);
    AiUInt32 numMonitorWords = bytesRead / 4;
    const AiUInt32* pMonitorWords = reinterpret_cast<const AiUInt32*>(buffer);

    for (AiUInt32 i = 0; i < numMonitorWords; ++i) {
        AiUInt32 monitorWord = pMonitorWords[i];
        AiUInt8 type  = (monitorWord >> 28) & 0x0F;
        AiUInt8 cFlag = (monitorWord >> 27) & 0x01;
        AiUInt32 entryData = monitorWord & 0x07FFFFFF;

        printf("  Idx %3u: Raw=0x%08X, Type=0x%X, C=%u, EntryRaw=0x%07X\n", i, monitorWord, type, cFlag, entryData);

        switch (type) {
            case 0x0: printf("    Entry Not Updated\n"); break;
            case 0x1: parseErrorWordEntry(entryData); break;
            case 0x2: parseTimeTagLowEntry(entryData); break;
            case 0x3: parseTimeTagHighEntry(entryData); break;
            case 0x8: case 0x9: case 0xA: case 0xB:
            case 0xC: case 0xD: case 0xE: case 0xF:
                parseBusWordEntry(entryData, type);
                break;
            default: printf("    Reserved/Unknown Monitor Word Type: 0x%X\n", type); break;
        }
    }
    printf("--- End of BM Data Parse (%u words) ---\n", numMonitorWords);
}


int main( int argc, char *argv[ ] )
{
    ConfigStruct xConfig;

    printf("*******************************************************************************\n");
    printf("***                           MIL-STD-1553 BUS MONITOR                      ***\n");
    printf("***                         Copyright (c) Turkish Aerospace                 ***\n");
    printf("*******************************************************************************\n");

    // ulModHandle zaten global ve 0 olarak başlatıldı.

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
    if (ret <= 0) {
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

    printf("Initializing Bus Monitor...\n");
    ret = ApiCmdBMIni(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMIni");
    printf("ApiCmdBMIni successful.\n");

    TY_API_BM_CAP_SETUP bmCapSetup;
    memset(&bmCapSetup, 0, sizeof(bmCapSetup));
    bmCapSetup.cap_mode = API_BM_CAPMODE_RECORDING;
    printf("Configuring BM Capture Mode...\n");
    ret = ApiCmdBMCapMode(ulModHandle, (AiUInt8)xConfig.ulStream, &bmCapSetup);
    AIM_CHECK_ERROR(ret, "ApiCmdBMCapMode");
    printf("ApiCmdBMCapMode successful.\n");

    AiUInt32 dataQueueId = API_DATA_QUEUE_ID_BM_REC_BIU1;
    if (xConfig.ulStream == 2) dataQueueId = API_DATA_QUEUE_ID_BM_REC_BIU2;
    AiUInt32 queueSize = 0;
    TY_API_DATA_QUEUE_READ queueReadParams;
    TY_API_DATA_QUEUE_STATUS queueStatus;
    std::vector<unsigned char> rxBuffer;
    const AiUInt32 READ_CHUNK_SIZE = 16 * 1024;

    printf("Opening Data Queue (ID: %u)...\n", dataQueueId);
    ret = ApiCmdDataQueueOpen(ulModHandle, dataQueueId, &queueSize);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueOpen");
    if (queueSize == 0) {
        printf("Error: Data Queue size is 0. Cannot proceed.\n");
        if (ulModHandle != 0) ApiClose(ulModHandle);
        ApiExit();
        return -1;
    }
    printf("Data Queue opened. Onboard ASP Queue Size: %u bytes\n", queueSize);
    rxBuffer.resize(READ_CHUNK_SIZE);

    printf("Starting Data Queue Control...\n");
    ret = ApiCmdDataQueueControl(ulModHandle, dataQueueId, API_DATA_QUEUE_CTRL_MODE_START);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueControl - START");
    printf("Data Queue started for BM recording.\n");

    printf("Starting Bus Monitor data capture...\n");
    ret = ApiCmdBMStart(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMStart");
    printf("Bus Monitor started. Monitoring bus traffic for a few seconds...\n");

    bool monitoringActive = true;
    int monitoring_duration_seconds = 10; // Örnek: 10 saniye izle
    auto startTime = std::chrono::steady_clock::now();

    while (monitoringActive) {
        memset(&queueReadParams, 0, sizeof(queueReadParams));
        queueReadParams.id = dataQueueId;
        queueReadParams.buffer = rxBuffer.data();
        queueReadParams.bytes_to_read = READ_CHUNK_SIZE;
        memset(&queueStatus, 0, sizeof(queueStatus));

        ret = ApiCmdDataQueueRead(ulModHandle, &queueReadParams, &queueStatus);
        if (ret != API_OK) {
            AIM_CHECK_ERROR(ret, "ApiCmdDataQueueRead");
            break;
        }

        if (queueStatus.bytes_transfered > 0) {
            printf("Read %u bytes. In queue: %u. Total transferred since open: %" PRIu64 "\n",
                   queueStatus.bytes_transfered, queueStatus.bytes_in_queue, queueStatus.total_bytes_transfered);
            parseAndPrintBMData(rxBuffer.data(), queueStatus.bytes_transfered);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Kuyruğu çok sık sorgulama

        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= monitoring_duration_seconds) {
            printf("Monitoring duration of %d seconds ended.\n", monitoring_duration_seconds);
            monitoringActive = false;
        }
    }

    printf("Stopping Bus Monitor data capture...\n");
    ret = ApiCmdBMHalt(ulModHandle, (AiUInt8)xConfig.ulStream);
    AIM_CHECK_ERROR(ret, "ApiCmdBMHalt");
    printf("Bus Monitor halted.\n");

    printf("Stopping Data Queue Control...\n");
    ret = ApiCmdDataQueueControl(ulModHandle, dataQueueId, API_DATA_QUEUE_CTRL_MODE_STOP);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueControl - STOP");
    printf("Data Queue stopped.\n");

    // Kuyrukta kalan son verileri oku (varsa)
    printf("Reading any remaining data in queue...\n");
    memset(&queueReadParams, 0, sizeof(queueReadParams));
    queueReadParams.id = dataQueueId;
    queueReadParams.buffer = rxBuffer.data();
    queueReadParams.bytes_to_read = READ_CHUNK_SIZE; // Veya queueStatus.bytes_in_queue (dikkatli ol)
    memset(&queueStatus, 0, sizeof(queueStatus));
    ret = ApiCmdDataQueueRead(ulModHandle, &queueReadParams, &queueStatus);
    if (ret == API_OK && queueStatus.bytes_transfered > 0) {
         printf("Final Read: %u bytes. In queue: %u.\n", queueStatus.bytes_transfered, queueStatus.bytes_in_queue);
         parseAndPrintBMData(rxBuffer.data(), queueStatus.bytes_transfered);
    }


    printf("Closing Data Queue...\n");
    ret = ApiCmdDataQueueClose(ulModHandle, dataQueueId);
    AIM_CHECK_ERROR(ret, "ApiCmdDataQueueClose");
    printf("Data Queue closed.\n");

    printf("Bus Monitor Operations Complete.\n");

    if (ulModHandle != 0) {
        ret = ApiClose(ulModHandle);
        AIM_CHECK_ERROR(ret, "ApiClose");
        printf("ApiClose successful.\n");
        ulModHandle = 0;
    }

    ApiExit();
    printf("ApiExit called.\n");

    printf("Bus Monitor Terminated.\n");
    return 0;
}

AiReturn configureHardware(AiUInt32 modHandle, AiUInt32 streamId, AiUInt8 coupling){
    AiReturn ret = 0;
    printf("Configuring hardware for coupling mode: %d on stream %d\n", coupling, streamId);

    ret = ApiCmdCalCplCon(modHandle, (AiUInt8)streamId, API_CAL_BUS_PRIMARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Primary");

    ret = ApiCmdCalCplCon(modHandle, (AiUInt8)streamId, API_CAL_BUS_SECONDARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Secondary");

    printf("Hardware coupling configured successfully.\n");
    return API_OK;
}
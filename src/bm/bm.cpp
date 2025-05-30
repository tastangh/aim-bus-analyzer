#include "Api1553.h"
#include "bm.hpp"
#include <cstring> 
#include <string>  



AiReturn configureHardware(AiUInt32 ulModHandle, AiUInt32 streamId, AiUInt8 coupling); // streamId eklendi

int main( int argc, char *argv[ ] )
{
    ConfigStruct         xConfig;

    printf("*******************************************************************************\n");
    printf("***                           MIL-STD-1553 BUS MONITOR                      ***\n");
    printf("***                         Copyright (c) Turkish Aerospace                 ***\n");
    printf("*******************************************************************************\n");

    AiUInt32            ulModHandle           = 0;

    xConfig.ulDevice = 0;     // İlk AIM kartı
    xConfig.ulStream = 1;     // Kart üzerindeki ilk stream/BIU
    xConfig.ucUseServer = 0;
    strcpy(xConfig.acServer, "local"); // Yerel kart için
    xConfig.ulCoupling = API_CAL_CPL_TRANSFORM;


    TY_API_RESET_INFO    xApiResetInfo;
    TY_API_OPEN          xApiOpen;
    // TY_API_DRIVER_INFO   xDriverInfo; // Bu örnekte kullanılmıyor
    AiUInt8             ucResetMode           = API_RESET_ALL; // Veya API_RESET_WITHOUT_SIMBUF

    memset( &xApiResetInfo, 0, sizeof(xApiResetInfo) );
    memset( &xApiOpen,      0, sizeof(xApiOpen)      );
    // memset( &xDriverInfo,   0, sizeof(xDriverInfo)   );


    AiReturn ret = API_OK; // Başlangıçta API_OK olarak ayarlamak iyi bir pratiktir.

    // 1. ApiInit çağrısı en başta olmalı
    ret = ApiInit();
    if (ret == 0) { 
        printf("ApiInit failed or no boards found. Return value: %d\n", ret);
        // AIM_CHECK_ERROR(ret, "ApiInit"); // Eğer ApiInit hata kodu döndürüyorsa
        if (ret <= 0) return ret; // Ya da basitçe çık
    }
    printf("ApiInit successful, found %d board(s).\n", ret);


    // 2. ApiOpenEx için xApiOpen yapısını doldur
    xApiOpen.ul_Module = xConfig.ulDevice;
    xApiOpen.ul_Stream = xConfig.ulStream;
    strncpy(xApiOpen.ac_SrvName, xConfig.acServer, sizeof(xApiOpen.ac_SrvName) - 1);
    xApiOpen.ac_SrvName[sizeof(xApiOpen.ac_SrvName) - 1] = '\0'; // Null-terminate

    ret = ApiOpenEx( &xApiOpen, &ulModHandle );
    AIM_CHECK_ERROR(ret, "ApiOpenEx");
    printf("ApiOpenEx successful. ModuleHandle: 0x%X\n", ulModHandle);

    // 3. ApiCmdReset çağrısı (ApiOpenEx'ten sonra)
    // ApiOpenEx kullanıldığında biu parametresi (ikinci parametre) göz ardı edilir,
    // ancak yine de stream numarasını geçmek kafa karışıklığını azaltır.
    ret = ApiCmdReset(ulModHandle, (AiUInt8)xConfig.ulStream, ucResetMode, &xApiResetInfo);
    AIM_CHECK_ERROR(ret, "ApiCmdReset");
    printf("ApiCmdReset successful.\n");

    // 4. Donanımı yapılandır
    ret = configureHardware(ulModHandle, xConfig.ulStream, xConfig.ulCoupling);
    AIM_CHECK_ERROR(ret, "configureHardware");
    printf("configureHardware successful.\n");

    printf("Bus Monitor Initialization Complete.\n");

    // ... (Bus Monitor işlemleri buraya eklenecek) ...

    // Kapatma işlemleri
    if (ulModHandle != 0) {
        ret = ApiClose(ulModHandle);
        AIM_CHECK_ERROR(ret, "ApiClose");
        printf("ApiClose successful.\n");
    }

    // ApiExit en son çağrılmalı
    // ApiExit dönüş değeri void olduğu için kontrol edilmez (Referans Kılavuzu sayfa 30)
    ApiExit();
    printf("ApiExit called.\n");

    printf("Bus Monitor Terminated.\n");
    return 0;
}

AiReturn configureHardware(AiUInt32 ulModHandle, AiUInt32 streamId, AiUInt8 coupling){
    AiReturn ret = 0;
    // ApiOpenEx kullanıldığında biu parametresi (ikinci parametre) göz ardı edilir.
    // Kılavuzda bu durum belirtiliyor. streamId veya 1 gibi bir değer geçilebilir.
    // Hata API_ERR_WRONG_BIU ise, genellikle ulModHandle'ın kendisi hatalıdır
    // ya da ApiOpen/ApiOpenEx doğru şekilde stream'i açamamıştır.

    printf("Configuring hardware for coupling mode: %d on stream %d\n", coupling, streamId);

    /* Set the primary coupling */
    ret = ApiCmdCalCplCon(ulModHandle, (AiUInt8)streamId, API_CAL_BUS_PRIMARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Primary");

    /* Set the secondary coupling */
    ret = ApiCmdCalCplCon(ulModHandle, (AiUInt8)streamId, API_CAL_BUS_SECONDARY, coupling);
    AIM_CHECK_ERROR(ret, "ApiCmdCalCplCon - Secondary");

    printf("Hardware coupling configured successfully.\n");
    return API_OK; // Veya 0
}
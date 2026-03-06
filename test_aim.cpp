#include <stdio.h>
#include "Api1553.h" // AIM API başlık dosyası
#include <string.h>

int main() {
    printf("Attempting to initialize AIM API...\n");
    AiReturn ret = ApiInit();
    if (ret <= 0) {
        printf("Error: ApiInit() failed. Code: %d\n", ret);
        return 1;
    }
    printf("ApiInit() successful.\n");

    AiUInt32 devId = 0; // Test için Cihaz ID 0'ı deniyoruz
    printf("Attempting to open Device ID %u...\n", devId);

    TY_API_OPEN open_params;
    AiUInt32 handle;
    memset(&open_params, 0, sizeof(open_params));
    open_params.ul_Module = devId;
    open_params.ul_Stream = 1;
    strcpy(open_params.ac_SrvName, "local");

    ret = ApiOpenEx(&open_params, &handle);

    if (ret == API_OK) {
        printf("SUCCESS! Device ID %u opened successfully with handle %u.\n", devId, handle);
        ApiClose(handle);
    } else {
        const char* errMsg = ApiGetErrorMessage(ret);
        printf("FAILURE! ApiOpenEx failed for Device ID %u. Error: %s (Code: %d)\n", devId, errMsg, ret);
    }
    
    ApiExit();
    return 0;
}
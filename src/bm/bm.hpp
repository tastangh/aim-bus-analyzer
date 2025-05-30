/* Datatypes used in sample.c */
typedef struct ConfigStruct
{
  AiUInt32 ulDevice;
  AiUInt32 ulStream;
  AiUInt32 ulCoupling;
  AiUInt8  ucUseServer;
  AiChar   acServer[28];
} ConfigStruct;



// Basit bir hata kontrol makrosu (DÜZELTİLMİŞ)
#define AIM_CHECK_ERROR(retVal, funcName) \
    if (retVal != API_OK) { \
        const char* errMsgPtr = ApiGetErrorMessage(retVal); \
        printf("ERROR in %s: %s (0x%04X) at %s:%d\n", funcName, errMsgPtr ? errMsgPtr : "Unknown Error", retVal, __FILE__, __LINE__); \
        return retVal; \
    }

AiReturn configureHardware(AiUInt32 ulModHandle, AiUInt8 coupling);


#include <string>

class wirelessConfig
{
    public:
        static void init();
        static void event();

        static void detectProc();

        static void detectorRecv(uint8_t rData);

        static void tweliteRecv(uint8_t rData);

        static void sendConfigCharProc(const char *cmds);

        static void setTweliteInit();
        static void setTweliteAppID(uint32_t app_id);

        static bool tweliteNeedInit();

        static uint8_t getModuleType();

        static char versionInfo[16];
        static uint32_t wModuleAddr;

        static uint32_t twelite_app_id;

    private:
        static uint8_t wModuleType;
        static bool InitNeedFlag;

        static uint32_t eventTimeCounter;

        static uint16_t lastRecvTime;
        static char recvBuf[128];
        static char recvBufPos;

        static void detectorEvent();
        static void analyze();

        static std::string inputBuffer;
        static std::string currentLine;

};

#define WIRELESS_CONFIG_MODULE_TYPE_NONE 0
#define WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART 1
#define WIRELESS_CONFIG_MODULE_TYPE_ESP32_WIRELESS 2

#define WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UNKNOWN 240

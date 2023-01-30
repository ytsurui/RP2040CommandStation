
class throttleAppProgUI
{
public:
    static void init(void);
    static void event(void);
    static void task(void);

    static void dispInit(void);
    static void dispEvent(void);
    static void dispTerminate(void);

    static void dispCVprogrammerInit(void);
    static void dispAddrProgrammerInit(void);

    static void keypadEvent(uint8_t code);

    static void readCallBack(uint8_t cvValue, bool stat);
    static void readProgressCallBack(uint8_t nowValue, uint8_t total);

    static void writeCallBack(uint8_t cvValue, bool stat);

    static void readAddrCV29Callback(uint8_t cvValue, bool stat);
    static void readAddrCV1Callback(uint8_t cvValue, bool stat);
    static void readAddrCV17Callback(uint8_t cvValue, bool stat);
    static void readAddrCV18Callback(uint8_t cvValue, bool stat);

    static void writeAddrCV29Callback(uint8_t cvValue, bool stat);
    static void writeAddrCV1Callback(uint8_t cvValue, bool stat);
    static void writeAddrCV17Callback(uint8_t cvValue, bool stat);
    static void writeAddrCV18Callback(uint8_t cvValue, bool stat);

private:
    static uint16_t cvNum;
    static uint8_t cvValue;
    static uint16_t progAddr;

    static uint8_t readAddrCV17temp;

    static bool inputFlag;
    static bool inputFlag2;

    static uint8_t mode;

    static uint8_t progressOldValue, progressOldTotal;
};

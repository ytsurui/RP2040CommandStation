
class progtrak
{
public:
    static bool sendOpsModeWriteByte(uint16_t trainAddr, uint16_t cvNum, uint8_t writeValue);
    static bool sendOpsModeWriteBit(uint16_t trainAddr, uint16_t cvNum, uint8_t pos, bool data);

    static bool checkProgMode(void);
    static bool readCurrentRunFlag(void);
    static void readCurrent(void);

    static void readDirectModeBit(uint16_t cvNum);
    static void readDirectModeByte(uint16_t cvNum);
    static void writeDirectModeByte(uint16_t cvNum, uint8_t writeValue);
    static void writeDirectModeBit(uint16_t cvNum, uint8_t pos, bool value);

    static void setProgMode(uint8_t progModeValue);
    static void setReadWrite(uint8_t rwMode);
    static void setCVnum(uint16_t cvNum);
    static void setCVvalue(uint8_t cvValue);
    static void setCallback(void (*cb)(uint8_t, bool));
    static void setProgressCallback(void (*cb)(uint8_t, uint8_t));

    static void setCVbitPos(uint8_t pos);
    static void setCVbitValue(bool data);

    static void RunProg(void);

    static void EventProg(void);

private:
    static bool progModeFlag;
    static bool readCurrentFlag;

    static void sendResetPacket();
    static void sendIdlePacket();
    static void sendDirectReadByte(uint16_t cvNum, uint8_t checkDataValue);
    static void sendDirectReadBit(uint16_t cvNum, uint8_t checkBitPos, uint8_t bitValue);
    static void sendDirectWriteByte(uint16_t cvNum, uint8_t writeValue);
    static void sendDirectWriteBit(uint16_t cvNum, uint8_t pos, bool value);

    static uint8_t progMode;
    static uint16_t progCVnum;
    static uint8_t progCVvalue;
    static bool progStat;

    static uint8_t progCVbitpos;
    static bool progCVbitValue;

    static uint16_t initCurrentValue;
    static bool currentStat;

    static void (*progCb)(uint8_t, bool);
    static void (*progProgressCb)(uint8_t, uint8_t);
};

#define PROGMODE_PAGED_BYTE 0x01
#define PROGMODE_DIRECT_BIT 0x04
#define PROGMODE_DIRECT_BYTE 0x05
#define PROGMODE_READ 0x00
#define PROGMODE_WRITE 0x80
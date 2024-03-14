
#define PACKETBUFFER_SIZE 10
#define PACKET_MAX_SIZE 20

class dccsignal
{
public:
    static void event(void);
    static bool checkRunRailcomCutout(void);

    static void execPacket(void);
    static bool putPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType);
    static void calcChecksumPacket(uint8_t *packet, uint8_t length);

    static uint8_t getWaitPacketCount(void);

    static void SetBiDiCutout(bool newstat);
    static bool GetBiDiCutoutStat(void);

private:
    typedef struct
    {
        uint8_t packet[PACKET_MAX_SIZE];
        uint8_t length;
        uint8_t retryCount;
        uint16_t targetAddr;
        uint16_t targetType;
        bool sendWaitFlag;
    } dccPacketBuffer;

    typedef struct
    {
        uint8_t packet[PACKET_MAX_SIZE];
        uint8_t length;
        uint8_t retryCount;
    } packetStruct;

    static packetStruct sendCache;
    static dccPacketBuffer packetBuf[PACKETBUFFER_SIZE];

    static uint8_t dccPBufInPos;
    static uint8_t dccPBufOutPos;
    static uint8_t dccPBufCount;

    static bool bufferMutexFlag;
    static bool bufferCounterMutexFlag;
    static bool cutoutFlagMutexFlag;

    static bool enableBiDiCutout;

    static packetStruct *getPacketBuffer(void);
};

/*
typedef struct
{
    uint8_t packet[PACKET_MAX_SIZE];
    uint8_t length;
    uint8_t retryCount;
    uint16_t targetAddr;
    uint16_t targetType;
    bool sendWaitFlag;
} dccPacketBuffer;

typedef struct
{
    uint8_t packet[PACKET_MAX_SIZE];
    uint8_t length;
    uint8_t retryCount;
} packetStruct;

void eventDCCsignalGenerater(void);
bool checkRunRailcomCutout(void);

void execDCCpacket(void);
bool putPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType);
void calcChecksumPacket(uint8_t *packet, uint8_t length);
*/
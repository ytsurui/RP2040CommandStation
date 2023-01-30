

#define ACC_ADDR_QUEUE_SIZE 10

class accessoryCtrl
{
public:
    static bool sendAccessoryPacket(uint16_t accAddr, bool stat);

    static bool getAccessoryStat(uint16_t accAddr);
    static void setAccessoryStat(uint16_t accAddr, bool stat);

    static void init();
    static void task();

private:
    typedef struct
    {
        uint8_t index;
        uint32_t bitpos;
    } AccAddrCache;

    static uint32_t accStatCache[64];

    static uint16_t addrQueue[ACC_ADDR_QUEUE_SIZE];
    static uint8_t queueInPos;
    static uint8_t queueOutPos;

    static AccAddrCache encodeAccAddr(uint16_t accAddr);

    static bool sendAccPacketData(uint16_t accAddr, bool stat);
};

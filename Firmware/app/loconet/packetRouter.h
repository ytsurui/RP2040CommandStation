
#define LOCONET_PACKET_BUF_MAX 32

class loconetPacketRouter
{
public:
    static void init(void);
    static void event(void);

    static void recv(uint8_t packetByte);

    static void setSender(void (*method)(uint8_t));

private:
    static uint8_t packetBuf[LOCONET_PACKET_BUF_MAX];
    static uint8_t bufpos;
    static uint8_t packetLength;
    static uint8_t checksum;

    static void packetExec(void);

    typedef struct
    {
        void (*func)(uint8_t);
        bool assigned;
    } cb_info;

    static cb_info cbdata;

    static void packetSend(uint8_t packet[], uint8_t length);
    static void sendLongAck(uint8_t ackOpCode, uint8_t ackMsg);
    static void sendSlotReadData(uint8_t slotID);

    static void execLocoAdr(void);
    static void execSlotRequestData(void);
    static void execSlotWriteData(void);
    static void execMoveSlot(void);
    static void execWriteSlot1(void);

    static void execLocoSpd(void);
    static void execLocoDirf(void);
    static void execLocoSnd(void);

    static void execSwitchRequest(void);
    static void execSwitchState(void);
};

// 2-Byte Instructions
#define LT_PACKET_GP_ON 0x83
#define LT_PACKET_GP_OFF 0x82
#define LT_PACKET_IDLE 0x85
#define LT_PACKET_BUSY 0x81

#define LT_PACKET_LOCO_ADR 0xBF
#define LT_PACKET_REQUEST_SLOT_DATA 0xBB
#define LT_PACKET_LOCO_SPD 0xA0
#define LT_PACKET_LOCO_DIRF 0xA1
#define LT_PACKET_LOCO_SND 0xA2

#define LT_PACKET_LONG_ACK 0xB4

#define LT_PACKET_OPC_MOVE_SLOT 0xBA

#define LT_PACKET_SLOT_STAT1 0xB5

#define LT_PACKET_SLOT_READ_DATA 0xE7
#define LT_PACKET_SLOT_WRITE_DATA 0xEF

#define LT_PACKET_SWITCH_STATE 0xBC
#define LT_PACKET_SWITCH_REQUEST 0xB0

#define LT_PACKET_LINK_SLOT 0xB9
#define LT_PACKET_UNLINK_SLOT 0xB8

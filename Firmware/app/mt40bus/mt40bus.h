
#define MT40BUS_BUF_LENGTH   64

#define LAST_SEND_RECV_COUNT_TIMEOUT    10000       // 10000ms = 10sec

class mt40busCtrl
{
    public:
        static void init(void);
        static void eventMS(void);

        static void recv(uint8_t rData);

        static void setSender(void (*method)(uint8_t));
        static void sendCmd(uint32_t cmd, uint32_t *args, uint8_t length);

        static void setCarrierSenseFunc(bool (*method)(void));
        
        // Echo Function
        static void sendCmdEcho(void);

    private:

        typedef struct
        {
            void (*func)(uint8_t);
            bool assigned;
        } cb_info;

        static cb_info senderCb;

        typedef struct
        {
            bool (*func)(void);
            bool assigned;
        } cb_bool_info;

        static cb_bool_info carrierSenseCb;

        typedef struct
        {
            uint8_t Buf[MT40BUS_BUF_LENGTH];
            uint8_t length;
        } packetBuf;

        static packetBuf recvData;
        static packetBuf execData;

        static uint32_t lastSendRecvCount;

        static void bufCopy(packetBuf *src, packetBuf *dest);

        static void execPacket();

        static bool argCheck(uint8_t *packet, uint8_t *pos, uint16_t *getData);

        static bool compareStr(char *arg1, uint8_t length1, char *arg2, uint8_t length2);

        static uint8_t decodeASCIItoNum(char data, bool enHEX);
        static void encodeNumToASCIIoct(uint32_t src, uint8_t *dest, uint8_t *length);

        static uint16_t decodeLocoAddr(uint32_t arg);
        static uint16_t decodeAccAddr(uint32_t arg);


        // Power Control / Toggle
        static void execCmdPW(uint32_t *args, uint8_t argCount);
        static void execCmdPWT(uint32_t *args, uint8_t argCount);
        static void execCmdPWS(uint32_t *args, uint8_t argCount);

        // Train Direction
        static void execCmdDI(uint32_t *args, uint8_t argCount);    // Train Direction
        static void execCmdDIS(uint32_t *args, uint8_t argCount);   // Train Direction Status

        // Train Function
        static void execCmdFN(uint32_t *args, uint8_t argCount);    // Train Function
        static void execCmdFNS(uint32_t *args, uint8_t argCount);   // Train Function Status

        // Train Speed
        static void execCmdSP(uint32_t *args, uint8_t argCount);    // Set Train Speed
        static void execCmdSPS(uint32_t *args, uint8_t argCount);   // Get Train Speed Status

        // Turnout(Accessory) Operation
        static void execCmdTO(uint32_t *args, uint8_t argCount);    // Turnout Operation
        static void execCmdTOS(uint32_t *args, uint8_t argCount);   // Turnout Operation Status

        // CommandStation Power Status
        static void execCmdCPS(uint32_t *args, uint8_t argCount);
};

#define MT40BUS_BUF_LENGTH   64

class mt40busCtrl
{
    public:
        static void init(void);

        static void recv(uint8_t rData);
        static void setSender(void (*method)(uint8_t));

    private:

        typedef struct
        {
            void (*func)(uint8_t);
            bool assigned;
        } cb_info;

        static cb_info senderCb;

        typedef struct
        {
            uint8_t Buf[MT40BUS_BUF_LENGTH];
            uint8_t length;
        } packetBuf;

        static packetBuf recvData;
        static packetBuf execData;

        static void bufCopy(packetBuf *src, packetBuf *dest);

        static void execPacket();

        static bool argCheck(uint8_t *packet, uint8_t *pos, uint16_t *getData);

        static bool compareStr(char *arg1, uint8_t length1, char *arg2, uint8_t length2);

        static uint8_t decodeASCIItoNum(char data, bool enHEX);

        static uint16_t decodeLocoAddr(uint32_t arg);
        static uint16_t decodeAccAddr(uint32_t arg);

        // Power Control / Toggle
        static void execCmdPW(uint32_t *args, uint8_t argCount);
        static void execCmdPWT(uint32_t *args, uint8_t argCount);
        static void execCmdPWS(uint32_t *args, uint8_t argCount);

        // Train Direction
        static void execCmdDI(uint32_t *args, uint8_t argCount);

        // Train Function
        static void execCmdFN(uint32_t *args, uint8_t argCount);

        // Train Speed
        static void execCmdSP(uint32_t *args, uint8_t argCount);

        // Turnout(Accessory) Operation
        static void execCmdTO(uint32_t *args, uint8_t argCount);
        static void execCmdTOS(uint32_t *args, uint8_t argCount);

};
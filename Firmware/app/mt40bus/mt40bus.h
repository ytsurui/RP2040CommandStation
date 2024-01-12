
#define MT40BUS_BUF_LENGTH   64

#define LAST_SEND_RECV_COUNT_TIMEOUT    10000       // 10000ms = 10sec

#define POWER_MANAGER_MAX_COUNT 1024

#define ENABLE_LBUS
//#define SET_LBUS_LOCAL_COMMAND

class mt40busCtrl
{
    public:
        static void init(void);
        static void eventMS(void);

        void recv(uint8_t rData);
        void receivedEchoCb(void (*func)(uint8_t, bool));

        static void setSender(void (*method)(uint8_t));
        static void sendCmd(uint32_t cmd, uint32_t *args, uint8_t length);

        static void setCarrierSenseFunc(bool (*method)(void));

        // Power Command
        static void sendCmdPWSresp(void);
        
        // Echo Function
        static void sendCmdEcho(void);

        // Power Manager Command
        static void PMinit(void);
        static void sendCmdPMUP(uint32_t addr);
        static void sendCmdPMSTresp(uint32_t addr, bool stat);
        static bool getPMdownStat();
        static void clearPMdownStat(uint32_t addr);

        static mt40busCtrl recvObj[3];

#ifdef ENABLE_LBUS
        static void setLBUScommandReceiver(void (*func)(uint8_t));
        static void setLBUScommandSender(void (*func)(uint8_t));
        static void sendLBUSdata(uint8_t *packet, uint8_t length);
#endif

        // MT40 DCC PowerManager Functions/Variables
        static bool getPowerManagerDown();

    private:
    
        typedef struct
        {
            void (*func)(uint8_t, bool);
            bool assigned;
        } cb_int_bool;

        typedef struct
        {
            void (*func)(uint8_t);
            bool assigned;
        } cb_info;

        static cb_info senderCb;

        cb_int_bool recvedEchoCb;

        typedef struct
        {
            bool (*func)(void);
            bool assigned;
        } cb_bool_info;

        static cb_bool_info carrierSenseCb;




#ifdef ENABLE_LBUS
        static cb_info LBUSrecvCb;
        static cb_info LBUSsendCb;
#endif

        typedef struct
        {
            uint8_t Buf[MT40BUS_BUF_LENGTH];
            uint8_t length;
        } packetBuf;

        packetBuf recvData;
        bool privatePacket;

        static packetBuf execData;

        static uint32_t lastSendRecvCount;

        static void bufCopy(packetBuf *src, packetBuf *dest);

        static void execPacket();

        static bool argCheck(uint8_t *packet, uint8_t *pos, uint16_t *getData);

        static bool compareStr(char *arg1, uint8_t length1, char *arg2, uint8_t length2);

        static uint8_t decodeASCIItoNum(char data, bool enHEX);
        static void encodeNumToASCIIoct(uint32_t src, uint8_t *dest, uint8_t *length);
        static char encodeNumToHexStr(uint8_t data);

        static uint16_t decodeLocoAddr(uint32_t arg);
        static uint16_t decodeAccAddr(uint32_t arg);

        // MT40 DCC PowerManager Functions/Variables
        static bool powerManagerDown;    // Power Manager Down Flag
        static uint32_t powerManagerStat[POWER_MANAGER_MAX_COUNT / 32];   // Power Manager Status Flags Bit

        typedef struct
        {
            uint8_t index;
            uint8_t bitpos;
        } PMaddrIndexPos;

        static void setPMstatus(uint32_t addr, bool stat);
        static bool getPMstatus(uint32_t addr);
        static PMaddrIndexPos getPMaddrIndexPos(uint32_t addr);

        // Power Control / Toggle
        static void execCmdPW(uint32_t *args, uint8_t argCount);
        static void execCmdPWT(uint32_t *args, uint8_t argCount);
        static void execCmdPWS(uint32_t *args, uint8_t argCount);

        // Train Direction
        static void execCmdDI(uint32_t *args, uint8_t argCount);    // Train Direction
        static void execCmdDIS(uint32_t *args, uint8_t argCount);   // Train Direction Status

        static void execCmdRDI(uint32_t *args, uint8_t argCount);   // RootUser/Robot Train Direction
        static void execCmdRDIT(uint32_t *args, uint8_t argCount);  // RootUser/Robot Train Direction Toggle
        static void execCmdRDIS(uint32_t *args, uint8_t argCount);  // RootUser/Robot Train Direction Status

        // Train Function
        static void execCmdFN(uint32_t *args, uint8_t argCount);    // Train Function
        static void execCmdFNS(uint32_t *args, uint8_t argCount);   // Train Function Status


        // Train Speed
        static void execCmdSP(uint32_t *args, uint8_t argCount);    // Set Train Speed
        static void execCmdSPS(uint32_t *args, uint8_t argCount);   // Get Train Speed Status

        static void execCmdRSP(uint32_t *args, uint8_t argCount);   // RootUser/Robot Set Train Speed
        static void execCmdRSPS(uint32_t *args, uint8_t argCount);  // RootUser/Robot Get Train Speed Status

        // Turnout(Accessory) Operation
        static void execCmdTO(uint32_t *args, uint8_t argCount);    // Turnout Operation
        static void execCmdTOS(uint32_t *args, uint8_t argCount);   // Turnout Operation Status

        // CommandStation Power Status
        static void execCmdCPS(uint32_t *args, uint8_t argCount);

        // CutOut Status
        static void execCmdCO(uint32_t *args, uint8_t argCount);    // CutOut railcom

        // Power Manager Command
        static void execCmdPMD(uint32_t *args, uint8_t argCount);   // Power Manager Down
        static void execCmdPMUP(uint32_t *args, uint8_t argCount);  // Power Manager Up
        static void execCmdPMST(uint32_t *args, uint8_t argCount);  // Power Manager Status
};
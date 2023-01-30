
#define TRAIN_CTRL_MAX 500

#define PACKET_SEND_INTERVAL 1000
#define TRAIN_TIMEOUT_MS 2400000
//#define TRAIN_TIMEOUT_MS 10000

class trainInfo
{
public:
    void init(void);
    uint16_t task(uint16_t appendWaitCount);

    uint16_t getAddr(void);
    bool setAddr(uint16_t newAddr);
    void clearAddr(void);

    bool setSpeed14(uint8_t dir, uint8_t spd);
    bool setSpeed28(uint8_t dir, uint8_t spd);
    bool setSpeed128(uint8_t dir, uint8_t spd);
    bool setFuncG1(uint8_t data);
    bool setFuncG2(uint8_t data);
    bool setFuncG3(uint8_t data);
    bool setFuncG4(uint8_t data);
    bool setFuncG5(uint8_t data);
    bool setFuncG6(uint8_t data);

    uint8_t getFuncG1(void);
    uint8_t getFuncG2(void);
    uint8_t getFuncG3(void);
    uint8_t getFuncG4(void);
    uint8_t getFuncG5(void);
    uint8_t getFuncG6(void);

private:
    typedef struct
    {
        bool enable;
        uint8_t data1;
        uint8_t data2;
        uint16_t sendcount;
    } trainDataInfo;

    typedef struct
    {
        trainDataInfo speed14;
        trainDataInfo speed28;
        trainDataInfo speed128;
        trainDataInfo FuncGroup1; // F0-F4
        trainDataInfo FuncGroup2; // F5-F8
        trainDataInfo FuncGroup3; // F9-F12
        trainDataInfo FuncGroup4; // F13-F20
        trainDataInfo FuncGroup5; // F21-F28
        trainDataInfo FuncGroup6; // F29-F32
    } trainCtrlInfo;

    uint16_t addr;
    uint32_t lastCtrlCounter;

    trainCtrlInfo trainData;
    void taskStub(trainDataInfo targetData);
};

class trainctrl
{
public:
    static void init(void);
    static void eventMS(void);
    static void task(void);

    typedef struct
    {
        trainInfo *train;
        bool enable;
    } trainctrlresp;

    static trainctrlresp selectNewTrain(uint16_t addr);
    static trainctrlresp getTrainCtrl(uint16_t addr);
    static void releaseTrain(uint16_t addr);

private:
    static bool enableTask;
    static uint16_t eventCounter;

    static uint16_t smallLastCtrlCountValue;
    static uint16_t appendLastCtrlCountValue;

    static trainInfo trainCtrlData[TRAIN_CTRL_MAX];
};
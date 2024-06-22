
#define TRAIN_CTRL_MAX 1000

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

    bool setRobotSpeed(uint8_t dir, uint8_t spd);   // ロボットの速度制限は0から100(%)で行われる

    bool setSpeed14(uint8_t dir, uint8_t spd);
    bool setSpeed28(uint8_t dir, uint8_t spd);
    bool setSpeed128(uint8_t dir, uint8_t spd);
    bool setFuncG1(uint8_t data);
    bool setFuncG2(uint8_t data);
    bool setFuncG3(uint8_t data);
    bool setFuncG4(uint8_t data);
    bool setFuncG5(uint8_t data);
    bool setFuncG6(uint8_t data);
    bool setFuncG7(uint8_t data);
    bool setFuncG8(uint8_t data);
    bool setFuncG9(uint8_t data);
    bool setFuncG10(uint8_t data);

    bool getSpeedType(uint8_t *spd, uint8_t *spdType);
    uint8_t getFuncG1(void);
    uint8_t getFuncG2(void);
    uint8_t getFuncG3(void);
    uint8_t getFuncG4(void);
    uint8_t getFuncG5(void);
    uint8_t getFuncG6(void);
    uint8_t getFuncG7(void);
    uint8_t getFuncG8(void);
    uint8_t getFuncG9(void);
    uint8_t getFuncG10(void);

    uint8_t getDirFlag(void);
    void setDirFlag(uint8_t dir);

    bool setRobotDirection(uint8_t dir);
    bool setRobotMaxSpd(uint8_t spd);
    uint8_t getRobotDirection();
    uint8_t getRobotMaxSpd();

private:
    uint8_t robotDir;
    uint8_t robotSpd;

    uint8_t origDir;
    uint8_t origSpd;

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
        trainDataInfo FuncGroup6; // F29-F36
        trainDataInfo FuncGroup7; // F37-F44
        trainDataInfo FuncGroup8; // F45-F52
        trainDataInfo FuncGroup9; // F53-F60
        trainDataInfo FuncGroup10; // F61-F68
    } trainCtrlInfo;

    uint16_t addr;
    uint32_t lastCtrlCounter;

    uint8_t directionFlag;

    trainCtrlInfo trainData;
    void taskStub(trainDataInfo targetData);

    void funcSendStub(trainDataInfo *fg, uint16_t appendWaitCount, uint16_t *smallSendCount, uint8_t funcGroup);

    void refreshTrainSpdDir();
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
    static trainctrlresp getTrainCtrlWithNewObj(uint16_t addr);
    static void releaseTrain(uint16_t addr);

private:
    static bool enableTask;
    static uint16_t eventCounter;

    static uint16_t smallLastCtrlCountValue;
    static uint16_t appendLastCtrlCountValue;

    static trainInfo trainCtrlData[TRAIN_CTRL_MAX];
};
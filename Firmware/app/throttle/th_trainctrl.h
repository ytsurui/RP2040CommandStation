
class throttleAppTrainCtrl
{
public:
    void init(char showPrefixCode);
    void event(void);
    void task(void);
    void dispInit(void);
    void dispEvent(void);
    void dispTerminate(void);

    void keypadEvent(uint8_t code);
    void directionEvent(uint8_t dir);
    void volumeEvent(uint8_t value);

    uint16_t getTrainAddr(void);
    char getPrefixCode(void);

private:
    uint16_t trainAddr;
    uint16_t selectingTrainAddr;
    char prefixCode;

    uint8_t directionCache;
    uint8_t oldSendDirection;
    uint8_t spdCache;

    uint8_t printSpd;
    uint8_t printDirection;

    uint8_t showPage;

    uint8_t ctrlFuncGroup;

    uint16_t lastCtrlEventCount;

    void printFuncMark(bool stat);
    void printFuncLabel(void);
    void printFuncStat(void);
};

#define TRAINCTRL_TRAINADDR_NONSELECT 0xFFFF

#define TRAINCTRL_SHOWPAGE_TOP 0

#define TRAINCTRL_SHOWPAGE_SELECT 2
#define TRAINCTRL_SHOWPAGE_RELEASE 3
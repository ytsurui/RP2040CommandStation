
#include "pico/stdlib.h"
#include "../trainctrl.h"

#define LOCO_SLOT_MAX 100
#define LOCO_SLOT_ID_PREFIX 8

#define LOCO_DIR_FOR 1
#define LOCO_DIR_REV 2

#define LOCO_TIMEOUT_VALUE 1800000

#define LOCO_PACKET_RESEND_COUNT 50

class loconetTrainSlot
{
public:
    static void globalInit(void);
    static void globalEventMS(void);
    void init(void);
    void event(void);
    void setInfo(uint8_t packet[]);

    static uint8_t getSlotID(uint16_t trainAddr);
    static uint8_t getFreeSlotID(void);

    static bool validSlotID(uint8_t slotID);
    static loconetTrainSlot *getSlotObj(uint8_t slotID);

    bool writeSpd(uint8_t spd);
    bool writeFunc1(uint8_t data);
    bool writeFunc2(uint8_t data);

    uint8_t readSpd(void);
    uint8_t readFunc1(void);
    uint8_t readFunc2(void);

    uint16_t getTrainAddr(void);
    void writeTrainAddr(uint16_t addr);

    void setInUse(void);
    void setIdle(void);
    void setCommon(void);

    uint8_t readStat1(void);
    uint8_t readStat2(void);

    void setStat1(uint8_t data);
    void setStat2(uint8_t data);

private:
    /*
    typedef struct
    {
        uint8_t slotStat;
        uint16_t trainAddr;

    } slotInfo;
    */

    bool _writeSpd(trainctrl::trainctrlresp *tInfo, uint8_t spd);

    uint16_t trainAddr;

    uint8_t spdValue;
    uint8_t func1Value;
    uint8_t func2Value;

    /*
     * slotStat1:
     *   bit7: SL_SPURGE (1 = slot purged, internal use only)
     *   bit6: SL_CONUP
     *   bit5: SL_BUSY
     *   bit4: SL_ACTIVE
     *   bit3: SL_CONDN
     *
     *   bit6/bit3: Consist Config
     *      11 = Logical Mid Consist, Linked up and down
     *      10 = Logical Consist Top, only linked downwards
     *      01 = Logical Consist sub-member, only linked upwards
     *      00 = free locomotive, no consist indirection/linking
     *   bit5/bit4: Slot Status
     *      11 = in use, loco address in slot
     *      10 = idle, loco address in slot
     *      01 = common, loco address in slot
     *      00 = free
     *   bit2-bit0: Speed Steps
     *      011 = 128-speed step
     *      010 = 14-speed step
     *      001 = 28-speed step, generate trinary packets
     *      000 = 28-speed step, 3-byte-packet regular mode
     *      111 = 128-speed step, advanced consisting
     *      100 = 28-speed step, advanced consisting
     */
    uint8_t slotStat1;

    /*
     * slotStat2:
     *   bit3:
     */
    uint8_t slotStat2;

    uint8_t direction;

    uint32_t timeoutCounter;

    uint8_t eventCounter;

    uint8_t lastSendSpd;
    uint8_t lastSendDirection;
    uint8_t lastSendFunc1;
    uint8_t lastSendFunc2;

    uint8_t spdLastSendCounter;
    uint8_t func1LastSendCounter;
    uint8_t func2LastSendCounter;

    static loconetTrainSlot trainSlot[];
};
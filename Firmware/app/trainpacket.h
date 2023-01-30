#define TRAIN_ADDR_MODE_US 1
#define TRAIN_ADDR_MODE_EU 2

#define TRAIN_SPD_DIR_FOR 1
#define TRAIN_SPD_DIR_REV 2

#define TRAIN_FUNC_BASIC_F0_F4 1
#define TRAIN_FUNC_BASIC_F5_F8 2
#define TRAIN_FUNC_BASIC_F9_F12 3

#define TRAIN_FUNC_EXTERNAL_F13_F20 1
#define TRAIN_FUNC_EXTERNAL_F20_F28 2

class trainpacket
{

public:
    /*
     *  sendSpeed28Packet
     *      Send 28-Speed Step Control Packet
     *      direction: For / Rev
     *      Speed: Speed Raw Value
     */
    static bool sendSpeed28Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed);

    /*
     *  sendSpeed14Packet
     *      Send 14-Speed Step Control Packet
     *      direction: For / Rev
     *      Speed: Speed Raw Value
     */
    static bool sendSpeed14Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed);

    /*
     *  sendSpeed128Packet
     *      Send 128-Speed Step Control Packet
     *      direction: For / Rev
     *      Speed: Speed Raw Value
     */
    static bool sendSpeed128Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed);

    /*
     *  sendFuncGroupPacket
     *      Send F0-F12 Packet
     *      FuncGroup: Select F0-F4 / F5-F8 / F9-F12
     *      FuncInfo: Function Status Mask
     */
    static bool sendFuncGroupPacket(uint16_t trainAddr, uint8_t funcGroup, uint8_t funcInfo);

    /*
     *  sendExternalFuncPacket
     *      Send F13-F28 Packet
     *      FuncGroup: Select F13-F20 / F21-F28
     *      FuncInfo: Function Status Mask
     */
    static bool sendExternalFuncPacket(uint16_t trainAddr, uint8_t funcGroup, uint8_t funcInfo);

private:
    typedef struct
    {
        uint8_t HighAddrPacket;
        uint8_t LowAddrPacket;
        bool failure;
    } TrainAddrDecoded;

    static TrainAddrDecoded decodeTrainAddr(uint16_t trainAddr, uint8_t mode);

    static uint8_t spd1428Encode(uint8_t spd, uint8_t mode);
#define SPD_MODE_14 1
#define SPD_MODE_28 2

    static uint8_t addrMode;
};

#include "pico/stdlib.h"

#include "../trainctrl.h"
#include "mt40bus.h"

#include "../../peripheral/dccsignalport.h"

#include "../acc_ctrl.h"

void mt40busCtrl::execCmdPW(uint32_t *args, uint8_t argCount)
{
    switch (argCount) {
        case 1:
            if (args[0] == 0) {
                dccport::setPowerStat(false);
            } else if (args[0] == 1) {
                dccport::setPowerStat(true);
            }
            break;
        case 2:
            if (args[1] == 0) {
                dccport::setPowerStat(false);
            } else if (args[1] == 1) {
                dccport::setPowerStat(true);
            }
            break;
    }
}

void mt40busCtrl::execCmdPWT(uint32_t *args, uint8_t argCount)
{
    dccport::togglePowerStat();
}

void mt40busCtrl::execCmdPWS(uint32_t *args, uint8_t argCount)
{
    if (argCount == 0) {
        // Request to Master Packet

    }
}

// Train Direction
void mt40busCtrl::execCmdDI(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    if (argCount != 2) return;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (args[2] == 2) {
        // Reverse
        trainCtrlObj.train->setDirFlag(2);
    } else if (args[2] == 1 || args[2] == 0) {
        // Forward
        trainCtrlObj.train->setDirFlag(1);
    }
}

// Train Function
void mt40busCtrl::execCmdFN(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    uint8_t funcData, funcMask;
    trainctrl::trainctrlresp trainCtrlObj;

    if (argCount != 3) return;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (args[1] == 0) {
        // Func Group 1 (0)
        funcMask = 0x10;
        funcData = trainCtrlObj.train->getFuncG1();
        if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG1(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG1(funcData);
        }
    } else if (args[1] <= 4) {
        // Func Group 1 (1-4)
        funcMask = 0x01 << (args[1] - 1);
        funcData = trainCtrlObj.train->getFuncG1();
         if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG1(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG1(funcData);
        }
    } else if (args[1] <= 8) {
        // Func Group 2 (5-8)
        funcMask = 0x01 << (args[1] - 5);
        funcData = trainCtrlObj.train->getFuncG2();
         if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG2(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG2(funcData);
        }
    } else if (args[1] <= 12) {
        // Func Group 3 (9-12)
        funcMask = 0x01 << (args[1] - 9);
        funcData = trainCtrlObj.train->getFuncG3();
         if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG3(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG3(funcData);
        }
    } else if (args[1] <= 20) {
        // Func Group 4 (13-20)
        funcMask = 0x01 << (args[1] - 13);
        funcData = trainCtrlObj.train->getFuncG4();
         if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG4(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG4(funcData);
        }
    } else if (args[1] <= 28) {
        // Func Group 5 (21-28)
        funcMask = 0x01 << (args[1] - 21);
        funcData = trainCtrlObj.train->getFuncG5();
         if (args[2] == 0) {
            funcData &= ~funcMask;
            trainCtrlObj.train->setFuncG5(funcData);
        } else if (args[2] == 1) {
            funcData |= funcMask;
            trainCtrlObj.train->setFuncG5(funcData);
        }
    }
}

// Train Speed
void mt40busCtrl::execCmdSP(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    uint8_t spd;
    trainctrl::trainctrlresp trainCtrlObj;

    if (argCount != 3) return;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    spd = (uint8_t)(args[1] & 0x000000FF);

    switch (args[2]) {
        case 0:
            // speed-step 14
            trainCtrlObj.train->setSpeed14(trainCtrlObj.train->getDirFlag(), spd);
            break;
        case 1:
            // speed-step 28
            trainCtrlObj.train->setSpeed28(trainCtrlObj.train->getDirFlag(), spd);
            break;
        case 2:
            // speed-step 128
            trainCtrlObj.train->setSpeed128(trainCtrlObj.train->getDirFlag(), spd);
            break;
    }
}

// Turnout (Accessories)
void mt40busCtrl::execCmdTO(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;

    if (argCount != 2) return;

    addr = decodeAccAddr(args[0]);
    if (addr == 0xFFFF) return;     // error

    if (args[1] == 1) {
        // Closed
        accessoryCtrl::sendAccessoryPacket(addr, true);
    } else if (args[1] == 0) {
        // Thrown
        accessoryCtrl::sendAccessoryPacket(addr, false);
    }
}

void mt40busCtrl::execCmdTOS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;

    if (argCount < 1) return;

    addr = decodeAccAddr(args[0]);
    if (addr == 0xFFFF) return;

    if (argCount == 1) {
        // Status Request
        
    }
}

uint16_t mt40busCtrl::decodeLocoAddr(uint32_t arg)
{
    if (arg >= 0xC000 && arg <= 0xE70F) {
        return (uint16_t)(arg - 0xC000);
    }
    return 0xFFFF;
}

uint16_t mt40busCtrl::decodeAccAddr(uint32_t arg)
{
    // 1(0x3800) - 2044(0x3FFC)
    if (arg >= 0x3800 && arg <= 0x3FFC) {
        return (uint16_t)(arg - 0x3800);
    }
    return 0xFFFF;
}

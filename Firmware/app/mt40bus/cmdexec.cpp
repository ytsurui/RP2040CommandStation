
#include "pico/stdlib.h"
#include <stdio.h>

#include "../trainctrl.h"
#include "mt40bus.h"

#include "../../peripheral/dccsignalport.h"

#include "../acc_ctrl.h"

#include "../voltage_monitor.h"
#include "../current_monitor.h"

void mt40busCtrl::execCmdPW(uint32_t *args, uint8_t argCount)
{
    //printf("newStat: %d, argCount: %d\n", args[0], argCount);
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
        uint32_t respArgs[1];
        if (dccport::getPowerStat()) {
            respArgs[0] = 1;
        } else {
            respArgs[0] = 0;
        }

        sendCmd('PWS', respArgs, 1);
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

    if (args[1] == 2) {
        // Reverse
        trainCtrlObj.train->setDirFlag(2);
    } else if (args[1] == 1 || args[1] == 0) {
        // Forward
        trainCtrlObj.train->setDirFlag(1);
    }
}

// Train Direction Status
//      DIS(addr)
//      DIS(addr. status);
void mt40busCtrl::execCmdDIS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    if (argCount == 1) {

        addr = decodeLocoAddr(args[0]);
        if (addr == 0xFFFF) return;

        trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
        if (!trainCtrlObj.enable) {
            return;
        }

        uint32_t respArgs[2];
        respArgs[0] = args[0];

        respArgs[1] = trainCtrlObj.train->getDirFlag();
        sendCmd('DIS', respArgs, 2);

        return;
    }
}

// Train Function
//      FN(addr,funcNum,stat)
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

// Train Function Status
//      FNS(addr, group)
//      FNS(addr, group, status)
void mt40busCtrl::execCmdFNS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    if (argCount < 1) return;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (argCount == 2) {
        // Function Status Request
        uint32_t respArgs[3];

        respArgs[0] = args[0];
        respArgs[1] = args[1];

        switch (args[1]) {
            case 1:
                // Group1 (F0-F4)
                respArgs[2] = trainCtrlObj.train->getFuncG1();
                break;
            case 2:
                // Group2 (F5-F12)
                respArgs[2] = (trainCtrlObj.train->getFuncG3() << 4) | trainCtrlObj.train->getFuncG2();
                break;
            case 3:
                // Group3 (F13-F20)
                respArgs[2] = trainCtrlObj.train->getFuncG4();
                break;
            case 4:
                // Group4 (F21-F28)
                respArgs[2] = trainCtrlObj.train->getFuncG5();
                break;
            default:
                return;
        }

        sendCmd('FNS', respArgs, 3);
        return;
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

void mt40busCtrl::execCmdSPS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    uint8_t spd, ctrlType;
    
    if (argCount < 1) return;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) return;

    if (argCount == 1) {
        // Speed Status Request
        uint32_t respArgs[3];

        respArgs[0] = args[0];

        if (trainCtrlObj.train->getSpeedType(&spd, &ctrlType)) {
            respArgs[1] = spd;
            respArgs[2] = ctrlType;

            sendCmd('SPS', respArgs, 3);
        }
        return;
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
        uint32_t respArgs[2];

        respArgs[0] = args[0];
        if (accessoryCtrl::getAccessoryStat(addr)) {
            // Closed
            respArgs[1] = 1;
        } else {
            // Thrown
            respArgs[1] = 0;
        }

        sendCmd('TOS', respArgs, 2);
    }
}

void mt40busCtrl::execCmdCPS(uint32_t *args, uint8_t argCount)
{
    if (argCount == 0) {
        // CommandStation Status Request
        uint32_t respArgs[2];

        respArgs[0] = voltageMonitor::get();
        respArgs[1] = currentMonitor::getCurrent();

        sendCmd('CPS', respArgs, 2);
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


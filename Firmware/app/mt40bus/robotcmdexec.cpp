
#include "pico/stdlib.h"
#include <stdio.h>

#include "../trainctrl.h"
#include "mt40bus.h"

#include "../../peripheral/dccsignalport.h"

#include "../acc_ctrl.h"

#include "../voltage_monitor.h"
#include "../current_monitor.h"


// RootUser / Robot Train Direction
void mt40busCtrl::execCmdRDI(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (args[1] == 1) {
        trainCtrlObj.train->setRobotDirection(1);
    } else {
        trainCtrlObj.train->setRobotDirection(0);
    }
}

// RootUser / Robot Train Direction Toggle
void mt40busCtrl::execCmdRDIT(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (trainCtrlObj.train->getRobotDirection()) {
        trainCtrlObj.train->setRobotDirection(0);
    } else {
        trainCtrlObj.train->setRobotDirection(1);
    }
}

// RootUser / Robot Train Direction Status
void mt40busCtrl::execCmdRDIS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (argCount == 1) {
        uint32_t respArgs[2];
        respArgs[0] = args[0];
        respArgs[1] = trainCtrlObj.train->getRobotDirection();
        sendCmd('RDIS', respArgs, 2);
    }
}

// RootUser / Robot Set Train Speed Status
void mt40busCtrl::execCmdRSP(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    uint8_t spdLimit;
    trainctrl::trainctrlresp trainCtrlObj;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    spdLimit = (uint8_t)(args[1]);
    if (spdLimit > 100) {
        // 不正値対策で何もしない
        return;
    }

    trainCtrlObj.train->setRobotMaxSpd(spdLimit);
}

// RootUser / Robot Get Train Speed Status
void mt40busCtrl::execCmdRSPS(uint32_t *args, uint8_t argCount)
{
    uint16_t addr;
    trainctrl::trainctrlresp trainCtrlObj;

    addr = decodeLocoAddr(args[0]);
    if (addr == 0xFFFF) return;

    trainCtrlObj = trainctrl::getTrainCtrlWithNewObj(addr);
    if (!trainCtrlObj.enable) {
        return;
    }

    if (argCount == 1) {
        // Speed Limit Request
        uint32_t respArgs[2];
        respArgs[0] = args[0];
        respArgs[1] = trainCtrlObj.train->getRobotMaxSpd();
        sendCmd('RSPS', respArgs, 2);
    }
}
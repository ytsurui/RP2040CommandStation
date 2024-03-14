
#include "pico/stdlib.h"
#include "mt40bus.h"

#include <stdio.h>
#include <string.h>

//mt40busCtrl::packetBuf mt40busCtrl::recvData;
mt40busCtrl::packetBuf mt40busCtrl::execData;

mt40busCtrl mt40busCtrl::recvObj[3];

void mt40busCtrl::init()
{
    //recvData.bufpos = 0;
    //recvData.length = 0;

    senderCb.func = nullptr;
    senderCb.assigned = false;
    carrierSenseCb.func = nullptr;
    carrierSenseCb.assigned = false;

    recvObj[0].recvData.length = 0;
    recvObj[1].recvData.length = 0;
    recvObj[2].recvData.length = 0;
    recvObj[0].privatePacket = false;
    recvObj[1].privatePacket = false;
    recvObj[2].privatePacket = false;

    PMinit();
}

void mt40busCtrl::recv(uint8_t rData)
{
    lastSendRecvCount = 0;   

    if ((rData == '\n') || (rData == ' ')) {
        // Ignore
        //printf("packet ignore\n");
        return;
    }
    if ((rData == '{') || (rData == '}')) {
        // Private Packet
        privatePacket = true;
    }
    if (rData == '\r') {
        // Data End
        //printf("data end\n");
        bufCopy(&recvData, &execData);
        recvData.length = 0;

        if (execData.length == 0) {
            privatePacket = false;
            return;
        }

        // Execution
        if (execData.length < 4) {
            execData.length = 0;
            privatePacket = false;
            return;
        }
        
        // Parse

        if (recvedEchoCb.assigned) {
            uint8_t i;

            for (i = 0; i < execData.length; i++) {
                recvedEchoCb.func(execData.Buf[i], privatePacket);
            }

            recvedEchoCb.func('\r', privatePacket);
            recvedEchoCb.func('\n', privatePacket);
        }

        execPacket();
        //printf("data exec\n");
        execData.length = 0;
        privatePacket = false;
        return;
    }

    if (recvData.length >= MT40BUS_BUF_LENGTH) {
        return;
    }

    //printf("data recv: %c\n", rData);
    recvData.Buf[recvData.length] = rData;
    recvData.length++;
}

void mt40busCtrl::receivedEchoCb(void (*func)(uint8_t, bool)) {
    recvedEchoCb.func = func;
    recvedEchoCb.assigned = true;
}

void mt40busCtrl::bufCopy(packetBuf *src, packetBuf *dest)
{
    uint8_t i;

    for (i = 0; i < src->length; i++) {
        dest->Buf[i] = src->Buf[i];
    }
    dest->length = src->length;
}

#define EXEC_PACKET_MODE_CMD    1
#define EXEC_PACKET_MODE_ARGS   2
#define EXEC_PACKET_MODE_LBUS   11

void mt40busCtrl::execPacket()
{
    uint8_t i;
    bool cmdChkFlag;
    bool cmdExitFlag;
    uint8_t mode;
    uint8_t decodeData;
    bool enableHEX;
    uint8_t argGetCount;
    bool noneArgFlag = true;
    uint8_t argIndex = 0;

    char cmdBytes[5];
    uint32_t cmdLength = 0;

    //uint32_t cmdData;
    uint32_t argTable[10];

    bool LBUSlowerFlag;
    uint8_t LBUSrecvDatas[32];
    uint8_t LBUSgetpos;

    bool localPacket = false;

    // Parse
    //if (execData.Buf[0] != '(' && execData.Buf[0] != '{') {
    //    return;
    //}

    //if (execData.Buf[execData.length - 1] != ')' && execData.Buf[execData.length - 1] != '}') {
    //    return;
    //}



    //packetHead[0] = execData.Buf[0];
    //packetHead[1] = execData.Buf[1];

    cmdChkFlag = false;
    cmdExitFlag = false;
    //cmdData = 0;
    argGetCount = 0;
    mode = EXEC_PACKET_MODE_CMD;

    //printf("%s\n", execData.Buf);

    for (i = 0; i < execData.length; i++) {
        //printf("buf[%d]: %c, mode=%d\n", i, execData.Buf[i], mode);

        switch (mode) {
            case EXEC_PACKET_MODE_CMD:    
                if (i > 4) return;
                if (execData.Buf[i] == '(' || execData.Buf[i] == '{') {
                    cmdChkFlag = true;
                    if (compareStr(cmdBytes, cmdLength, (char*)"LBUS", 4)) {
                    //if (cmdData == 'LBUS') {
                        mode = EXEC_PACKET_MODE_LBUS;
                        LBUSlowerFlag = false;
                        LBUSgetpos = 0;
                    } else {
                        mode = EXEC_PACKET_MODE_ARGS;
                    }
                    argTable[0] = 0;

                    if (execData.Buf[i] == '{') {
                        localPacket = true;
                    }
                    break;
                }
                //printf("cmdData: %d, data: %d\n", cmdData, execData.Buf[i]);
                cmdBytes[i] = execData.Buf[i];
                cmdLength++;
                //cmdData = (cmdData << 8) | execData.Buf[i];
                //printf("cmdData: %d\n", cmdData);
                break;
            case EXEC_PACKET_MODE_ARGS:
                switch (execData.Buf[i]) {
                    case ',':
                        // ARG END
                        enableHEX = false;
                        argGetCount = 0;
                        argIndex++;
                        argTable[argIndex] = 0;
                        break;
                    case '}':
                        localPacket = true;
                    case ')':
                        // Command END
                        cmdExitFlag = true;
                        if (!noneArgFlag) argIndex++;
                        break;
                    case 'x':
                    case 'X':
                        if (argGetCount == 1 && argTable[argIndex] == 0) {
                            enableHEX = true;
                            argGetCount = 0;
                        }
                        break;
                    default:
                        decodeData = decodeASCIItoNum(execData.Buf[i], enableHEX);
                        noneArgFlag = false;
                        if (decodeData == 0xFF)
                            return;
                        if (enableHEX) {
                            // 16進数表記
                            argTable[argIndex] = (argTable[argIndex] << 4) | decodeData;
                        } else {
                            // 10進数表記
                            argTable[argIndex] = (argTable[argIndex] * 10) + decodeData;
                        }
                        argGetCount++;
                        break;
                }
                break;
#ifdef ENABLE_LBUS
            case EXEC_PACKET_MODE_LBUS:
                switch (execData.Buf[i]) {
                    case '}':
                        localPacket = true;
                    case ')':
                        // Command END
                        cmdExitFlag = true;
                        break;
                    default:
                        decodeData = decodeASCIItoNum(execData.Buf[i], true);
                        if (LBUSlowerFlag) {
                            LBUSrecvDatas[LBUSgetpos] |= decodeData;
                            LBUSlowerFlag = false;
                            LBUSgetpos++;
                        } else {
                            LBUSrecvDatas[LBUSgetpos] = (decodeData << 4);
                            LBUSlowerFlag = true;
                        }
                        break;
                }
#endif
        }
        if (cmdExitFlag) {
            // コマンド終了フラグを確認
            break;
        }
        
    }

    //printf("cmdData: %d\n", cmdData);

    if (!cmdChkFlag || !cmdExitFlag) return;

#ifdef ENABLE_LBUS
    if (compareStr(cmdBytes, cmdLength, (char*)"LBUS", 4)) {
    //if (cmdData == 'LBUS') {
        if (LBUSrecvCb.assigned) {
            for (i = 0; i < LBUSgetpos; i++) {
                LBUSrecvCb.func(LBUSrecvDatas[i]);
            }
        }
        return;
    }
#endif

    switch (cmdLength) {
        case 2:
            if (compareStr(cmdBytes, cmdLength, (char*)"PW", 2)) {
                // Power
                execCmdPW(argTable, argIndex);        
            } else if (compareStr(cmdBytes, cmdLength, (char*)"DI", 2)) {
                // Train Direction
                execCmdDI(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"FN", 2)) {
                // Train Function
                execCmdFN(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"SP", 2)) {
                // Train Speed
                execCmdSP(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"TO", 2)) {
                // Turnout (Accessory Decoders)
                execCmdTO(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"CO", 2)) {
                // Curout Status
                execCmdCO(argTable, argIndex);
            }
            break;
        case 3:
            if (compareStr(cmdBytes, cmdLength, (char*)"PWT", 3)) {
                // Power Toggle
                execCmdPWT(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"PWS", 3)) {
                // Power Status
                execCmdPWS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"PMD", 3)) {
                // Power Manager Down
                execCmdPMD(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"DIS", 3)) {
                // Train Direction Status
                execCmdDIS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"FNS", 3)) {
                // Train Function Status;
                execCmdFNS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"SPS", 3)) {
                // Train Speed Status
                execCmdSPS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"TOS", 3)) {
                // Turnout (Accessory Decoders) Status
                execCmdTOS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"CPS", 3)) {
                // CommandStation Power Status
                execCmdCPS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"RDI", 3)) {
                // Rootuser/Robot Direction Input
                execCmdRDI(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"RSP", 3)) {
                //Rootuser/Robot Speed
                execCmdRSP(argTable, argIndex);
            }
            break;
        case 4:
            if (compareStr(cmdBytes, cmdLength, (char*)"ECHO", 4)) {
                // Echo (No Operation)
            } else if (compareStr(cmdBytes, cmdLength, (char*)"PMUP", 4)) {
                // Power Manager UP
                execCmdPMUP(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"PMST", 4)) {
                // Power Manager STatus
                execCmdPMST(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"RDIT", 4)) {
                // Rootuser/Robot Direction Input Toggle
                execCmdRDIT(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"RDIS", 4)) {
                // Rootuser/Robot Diretcion Status
                execCmdRDIS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"RSPS", 4)) {
                //Rootuser/Robot Speed Status
                execCmdRSPS(argTable, argIndex);
            } else if (compareStr(cmdBytes, cmdLength, (char*)"WTYP", 4)) {
                // Wireless Config Wireless Type
            } else if (compareStr(cmdBytes, cmdLength, (char*)"WCFG", 4)) {
                // Wireless Config Request
            }
            break;
    }

    //if (compareStr(cmdBytes, cmdLength, (char*)"ECHO", 4)) {
    //    // Echo (No Operation)
    //} else if (compareStr(cmdBytes, cmdLength, (char*)"PW", 2)) {
    //    // Power
    //    execCmdPW(argTable, argIndex);        
    //} else if (compareStr(cmdBytes, cmdLength, (char*)"PWT", 3)) {
    //    // Power Toggle
    //    execCmdPWT(argTable, argIndex);
    //} else if (compareStr(cmdBytes, cmdLength, (char*)"PWS", 3)) {
    //    // Power Status
    //    execCmdPWS(argTable, argIndex);
    //}
    /*
    switch (cmdData) {
        //case 'ECHO':
        //    // Echo
        //    break;
        //case 'PW':
        //    // Power
        //    execCmdPW(argTable, argIndex);            
        //    break;
        //case 'PWT':
        //    // Power Toggle
        //    execCmdPWT(argTable, argIndex);
        //    break;
        //case 'PWS':
        //    // Power Status
        //    execCmdPWS(argTable, argIndex);
        //    break;
        //case 'PMD':
        //    // Power Manager Down
        //    execCmdPMD(argTable, argIndex);
        //    break;
        //case 'PMUP':
        //    // Power Manager Up
        //    execCmdPMUP(argTable, argIndex);
        //    break;
        //case 'PMST':
        //    // Power Manager STatus
        //    execCmdPMST(argTable, argIndex);
        //    break;
        //case 'DI':
        //    // Train Direction
        //    execCmdDI(argTable, argIndex);
        //    break;
        //case 'DIS':
        //    // Train Direction Status
        //    execCmdDIS(argTable, argIndex);
        //    break;
        //case 'FN':
        //    // Train Function
        //    execCmdFN(argTable, argIndex);
        //    break;
        //case 'FNS':
        //    // Train Function Status;
        //    execCmdFNS(argTable, argIndex);
        //    break;
        //case 'SP':
        //    // Train Speed
        //    execCmdSP(argTable, argIndex);
        //    break;
        //case 'SPS':
        //    // Train Speed Status
        //    execCmdSPS(argTable, argIndex);
        //    break;
        //case 'TO':
        //    // Turnout (Accessory Decoders)
        //    execCmdTO(argTable, argIndex);
        //    break;
        //case 'TOS':
        //    // Turnout (Accessory Decoders) Status
        //    execCmdTOS(argTable, argIndex);
        //    break;
        //case 'CPS':
        //    // CommandStation Power Status
        //    execCmdCPS(argTable, argIndex);
        //    break;
        //case 'RDI':
        //    // Rootuser/Robot Direction Input
        //    execCmdRDI(argTable, argIndex);
        //    break;
        //case 'RDIT':
        //    // Rootuser/Robot Direction Input Toggle
        //    execCmdRDIT(argTable, argIndex);
        //    break;
        //case 'RDIS':
        //    // Rootuser/Robot Diretcion Status
        //    execCmdRDIS(argTable, argIndex);
        //    break;
        //case 'RSP':
        //    //Rootuser/Robot Speed
        //    execCmdRSP(argTable, argIndex);
        //    break;
        //case 'RSPS':
        //    //Rootuser/Robot Speed Status
        //    execCmdRSPS(argTable, argIndex);
        //    break;
        default:
            //printf("unknown packet: cmdData=%s\n", cmdData);
            break;
    }
    */

    for (i = 0; i < MT40BUS_BUF_LENGTH; i++) {
        execData.Buf[i] = 0;
    }

}



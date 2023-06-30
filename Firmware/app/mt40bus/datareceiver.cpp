
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
}

void mt40busCtrl::recv(uint8_t rData)
{
    lastSendRecvCount = 0;   

    if ((rData == '\n') || (rData == ' ')) {
        // Ignore
        //printf("packet ignore\n");
        return;
    }
    if (rData == '\r') {
        // Data End
        //printf("data end\n");
        bufCopy(&recvData, &execData);
        recvData.length = 0;

        if (execData.length == 0) {
            return;
        }

        // Execution
        if (execData.length < 4) {
            return;
        }
        
        // Parse

        if (recvedEchoCb.assigned) {
            uint8_t i;

            for (i = 0; i < execData.length; i++) {
                recvedEchoCb.func(execData.Buf[i]);
            }

            recvedEchoCb.func('\r');
            recvedEchoCb.func('\n');
        }

        execPacket();
        //printf("data exec\n");
        return;
    }

    if (recvData.length >= MT40BUS_BUF_LENGTH) {
        return;
    }

    //printf("data recv: %c\n", rData);
    recvData.Buf[recvData.length] = rData;
    recvData.length++;
}

void mt40busCtrl::receivedEchoCb(void (*func)(uint8_t)) {
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


    uint32_t cmdData;
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
    cmdData = 0;
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
                    if (cmdData == 'LBUS') {
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
                cmdData = (cmdData << 8) | execData.Buf[i];
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
    if (cmdData == 'LBUS') {
        if (LBUSrecvCb.assigned) {
            for (i = 0; i < LBUSgetpos; i++) {
                LBUSrecvCb.func(LBUSrecvDatas[i]);
            }
        }
        return;
    }
#endif

    switch (cmdData) {
        case 'ECHO':
            // Echo
            break;
        case 'PW':
            // Power
            execCmdPW(argTable, argIndex);            
            break;
        case 'PWT':
            // Power Toggle
            execCmdPWT(argTable, argIndex);
            break;
        case 'PWS':
            // Power Status
            execCmdPWS(argTable, argIndex);
            break;
        case 'DI':
            // Train Direction
            execCmdDI(argTable, argIndex);
            break;
        case 'DIS':
            // Train Direction Status
            execCmdDIS(argTable, argIndex);
            break;
        case 'FN':
            // Train Function
            execCmdFN(argTable, argIndex);
            break;
        case 'FNS':
            // Train Function Status;
            execCmdFNS(argTable, argIndex);
            break;
        case 'SP':
            // Train Speed
            execCmdSP(argTable, argIndex);
            break;
        case 'SPS':
            // Train Speed Status
            execCmdSPS(argTable, argIndex);
            break;
        case 'TO':
            // Turnout (Accessory Decoders)
            execCmdTO(argTable, argIndex);
            break;
        case 'TOS':
            // Turnout (Accessory Decoders) Status
            execCmdTOS(argTable, argIndex);
            break;
        case 'CPS':
            // CommandStation Power Status
            execCmdCPS(argTable, argIndex);
            break;
        default:
            printf("unknown packet: cmdData=%s\n", cmdData);
            break;
    }

    for (i = 0; i < MT40BUS_BUF_LENGTH; i++) {
        execData.Buf[i] = 0;
    }

}


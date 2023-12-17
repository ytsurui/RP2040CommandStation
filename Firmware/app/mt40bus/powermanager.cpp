#include "pico/stdlib.h"
#include "mt40bus.h"

bool mt40busCtrl::powerManagerDown;
uint32_t mt40busCtrl::powerManagerStat[POWER_MANAGER_MAX_COUNT / 32];

void mt40busCtrl::PMinit()
{
    uint8_t i;

    powerManagerDown = false;

    for (i = 0; i < (POWER_MANAGER_MAX_COUNT / 32); i++) {
        powerManagerStat[i] = 0;
    }
}

bool mt40busCtrl::getPMdownStat()
{
    return powerManagerDown;
}

void mt40busCtrl::clearPMdownStat(uint32_t addr)
{
    if (addr == 0) {
        powerManagerDown = false;
        
        uint8_t i;
        
        for (i = 0; i < (POWER_MANAGER_MAX_COUNT / 32); i++) {
            powerManagerStat[i] = 0;
        }
        
        return;
    }

    setPMstatus(addr, false);
} 

void mt40busCtrl::setPMstatus(uint32_t addr, bool stat)
{
    mt40busCtrl::PMaddrIndexPos addrInfo;

    addrInfo = getPMaddrIndexPos(addr);

    if (stat) {
        powerManagerStat[addrInfo.index] |= addrInfo.bitpos;
    } else {
        powerManagerStat[addrInfo.index] &= ~addrInfo.bitpos;
    }
}

bool mt40busCtrl::getPMstatus(uint32_t addr)
{
    mt40busCtrl::PMaddrIndexPos addrInfo;

    addrInfo = getPMaddrIndexPos(addr);

    if (powerManagerStat[addrInfo.index] & addrInfo.bitpos) {
        return true;
    }
    return false;
}

mt40busCtrl::PMaddrIndexPos mt40busCtrl::getPMaddrIndexPos(uint32_t addr)
{
    mt40busCtrl::PMaddrIndexPos respData;

    respData.index = (addr / 32);
    respData.bitpos = (0x01 << (addr % 32));

    return respData;
}
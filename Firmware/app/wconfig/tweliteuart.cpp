#include <string>
#include <sstream>
#include <cstring>

#include "pico/stdlib.h"
#include "wconfig.h"

char wirelessConfig::versionInfo[16];
uint32_t wirelessConfig::wModuleAddr;

uint32_t wirelessConfig::twelite_app_id;

bool wirelessConfig::tweliteNeedInit()
{
    return InitNeedFlag;
}

void wirelessConfig::setTweliteInit()
{
    sendConfigCharProc("+++mD\r\nk0d0a,0,0\r\nS\r\n");
}

void wirelessConfig::setTweliteAppID(uint32_t new_app_id)
{
    char cmds[16];
    snprintf(cmds, sizeof(cmds), "+++a%08X\r\nS\r\n", new_app_id);
    sendConfigCharProc(cmds);
}
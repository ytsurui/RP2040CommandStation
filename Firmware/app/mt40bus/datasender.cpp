
#include "pico/stdlib.h"
#include "mt40bus.h"

mt40busCtrl::cb_info mt40busCtrl::senderCb;

void mt40busCtrl::setSender(void (*method)(uint8_t))
{
    senderCb.func = method;
    senderCb.assigned = true;
}

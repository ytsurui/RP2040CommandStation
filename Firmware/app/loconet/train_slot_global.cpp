
#include "pico/stdlib.h"
#include "train_slot.h"

loconetTrainSlot loconetTrainSlot::trainSlot[LOCO_SLOT_MAX];

void loconetTrainSlot::globalInit(void)
{
    uint8_t i;

    for (i = 0; i < LOCO_SLOT_MAX; i++)
    {
        trainSlot[i].init();
    }
}

void loconetTrainSlot::globalEventMS(void)
{
    uint8_t i;

    for (i = 0; i < LOCO_SLOT_MAX; i++)
    {
        trainSlot[i].event();

        if (!trainSlot[i].readStat1() & 0x80)
        {
            // enable slot
            trainSlot[i].timeoutCounter++;
            if (trainSlot[i].timeoutCounter > LOCO_TIMEOUT_VALUE)
            {
                // timeout release
                trainSlot[i].init();
            }
            else
            {
            }
        }
    }
}

uint8_t loconetTrainSlot::getSlotID(uint16_t trainAddr)
{
    uint8_t i;

    if (trainAddr == 0)
    {
        return (0xFF);
    }

    for (i = 0; i < LOCO_SLOT_MAX; i++)
    {
        if (trainSlot[i].trainAddr == trainAddr)
        {
            return (i + LOCO_SLOT_ID_PREFIX);
        }
    }

    return (0xFF);
}

uint8_t loconetTrainSlot::getFreeSlotID(void)
{
    uint8_t i;

    for (i = 0; i < LOCO_SLOT_MAX; i++)
    {
        //if (trainSlot->trainAddr == 0)
        if (trainSlot[i].slotStat1 & 0x80)
        {
            return (i + LOCO_SLOT_ID_PREFIX);
        }
    }

    return (0xFF);
}

bool loconetTrainSlot::validSlotID(uint8_t slotID)
{
    if (slotID < LOCO_SLOT_ID_PREFIX)
    {
        return false;
    }
    if (slotID >= (LOCO_SLOT_ID_PREFIX + LOCO_SLOT_MAX))
    {
        return false;
    }
    return true;
}

loconetTrainSlot *loconetTrainSlot::getSlotObj(uint8_t slotID)
{
    return (&trainSlot[slotID - LOCO_SLOT_ID_PREFIX]);
}

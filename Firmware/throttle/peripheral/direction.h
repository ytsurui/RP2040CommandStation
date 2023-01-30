

namespace directionSW
{
    void init(void);
    void event(void);

    bool checkChange(uint8_t channel);
    uint8_t readStat(uint8_t channel);
};

#define DIRECTION_FOR 1
#define DIRECTION_REV 2
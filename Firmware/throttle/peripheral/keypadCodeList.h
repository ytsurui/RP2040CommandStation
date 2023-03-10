
#define CODE_LIST_LENGTH 37

typedef struct
{
    uint32_t rawData;
    uint8_t respCode;
} ScanCodeData;

ScanCodeData codeList[CODE_LIST_LENGTH] = {
    // Basic Keys
    {0x00000001, 1},
    {0x00000002, 2},
    {0x00000004, 3},  // SW9 - (3)
    {0x00000008, 11}, // SW13 - (A)
    {0x00000010, 4},
    {0x00000020, 5},
    {0x00000040, 6},
    {0x00000080, 12}, // SW14 - (B)
    {0x00000100, 7},
    {0x00000200, 8},
    {0x00000400, 9},
    {0x00000800, 27}, // SW15 (Switch)
    {0x00001000, 21}, // SW4 (*)
    {0x00002000, 0},  // SW8 (0)
    {0x00004000, 22}, // SW12 (#)
    {0x00008000, 28}, // SW16 (Prog)

    // Combo Keys
    {0x00005000, 23}, // '*' + '#'
    {0x00003000, 50}, // '*' + '0'
    {0x00001001, 51},
    {0x00001002, 52},
    {0x00001004, 53},
    {0x00001010, 54},
    {0x00001020, 55},
    {0x00001040, 56},
    {0x00001100, 57},
    {0x00001200, 58},
    {0x00001400, 59}, // '*' + '9'
    {0x00006000, 60}, // '#' + '0'
    {0x00004001, 61},
    {0x00004002, 62},
    {0x00004004, 63},
    {0x00004010, 64},
    {0x00004020, 65},
    {0x00004040, 66},
    {0x00004100, 67},
    {0x00004200, 68},
    {0x00004400, 69}, // '#' + '9'
};

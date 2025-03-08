#ifdef __cplusplus
extern "C" void initLCD(void);
extern "C" void testMessage(void);

extern "C" void sendNum(uint16_t value);
extern "C" void writeLCDsingleNum(uint8_t value);
extern "C" void setLCDpos(uint8_t row, uint8_t column);
extern "C" void sendSpaceLCD(uint8_t count);
extern "C" void writeLCDdata(char data);
extern "C" void writeLCDstring(char *str);
extern "C" void showHex32Bit(uint32_t num);
extern "C" void clearLCD(void);
extern "C" void showCursor(void);
extern "C" void hideCursor(void);
#else
void initLCD(void);
void testMessage(void);

void sendNum(uint16_t value);
void writeLCDsingleNum(uint8_t value);
void setLCDpos(uint8_t row, uint8_t column);
void sendSpaceLCD(uint8_t count);
void writeLCDdata(char data);
void writeLCDstring(char *str);
void showHex32Bit(uint32_t num);
void clearLCD(void);
void showCursor(void);
void hideCursor(void);
#endif
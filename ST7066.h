#ifndef ST7066_H_
#define ST7066_H_

#include <avr/io.h>
#include <avr/pgmspace.h>


// -- GPIO description ----------------------------------------------------------------------------

#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_DDR  DDRD
#define LCD_CTRL_PIN  PIND

#define LCD_RW_PIN_NUM 5    // Select read or write. 0: Write 1: Read
#define LCD_RS_PIN_NUM 6    // Select registers. 0: Instruction register (for write) Busy flag: address counter (for read) 1: Data register (for write and read)
#define LCD_E_PIN_NUM  7    // Starts data read/write.

#define LCD_RW_PIN (1<<LCD_RW_PIN_NUM)
#define LCD_RS_PIN (1<<LCD_RS_PIN_NUM)
#define LCD_E_PIN  (1<<LCD_E_PIN_NUM)

#define LCD_CTRL_PIN_MASK (LCD_RW_PIN | LCD_RS_PIN | LCD_E_PIN)

#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR  DDRB
#define LCD_DATA_PIN  PINB

#define LCD_D4_PIN_NUM 0
#define LCD_D5_PIN_NUM 1
#define LCD_D6_PIN_NUM 2
#define LCD_D7_PIN_NUM 3

#define LCD_D4_PIN (1<<LCD_D4_PIN_NUM)
#define LCD_D5_PIN (1<<LCD_D5_PIN_NUM)
#define LCD_D6_PIN (1<<LCD_D6_PIN_NUM)
#define LCD_D7_PIN (1<<LCD_D7_PIN_NUM)

#define LCD_DATA_PIN_MASK (LCD_D7_PIN | LCD_D6_PIN | LCD_D5_PIN | LCD_D4_PIN)    // 4-bit interface data

#define LCD_DATA_DIR_SELECT_READ  LCD_CTRL_PORT |= LCD_RW_PIN
#define LCD_DATA_DIR_SELECT_WRITE LCD_CTRL_PORT &= ~(LCD_RW_PIN)

#define BYTE_TYPE_COMMAND 0
#define BYTE_TYPE_DATA 1

#define LCD_COMMAND_DISPLAY_FORMAT  0x28  // 4-bit bus mode; Font size - 5x8; Display line number - 2-line mode
#define LCD_COMMAND_DISPLAY_ENTRYM  0x06  // Cursor moves to right

//#define LCD_COMMAND_DISPLAY_ENABLE  0x0C  // Cursor - OFF; Cursor blinking - OFF
#define LCD_COMMAND_DISPLAY_ENABLE  0x0F

#define LCD_COMMAND_DISPLAY_DISABLE 0x08
#define LCD_COMMAND_CLEAR_DISPLAY   0x01
#define LCD_COMMAND_RETURN_HOME     0x02
#define LCD_COMMAND_SET_CURSOR      0x80

#define LCD_LINE_LENGTH 20      // Number of symbols in one string
#define LCD_FIRST_LINE  0x00    // Start first line DDRAM address
#define LCD_SECOND_LINE 0x40    // Start second line DDRAM address

#define LCD_CLEAR_DISPLAY           lcdSendByte(LCD_COMMAND_CLEAR_DISPLAY, BYTE_TYPE_COMMAND)
#define LCD_RETURN_HOME             lcdSendByte(LCD_COMMAND_RETURN_HOME, BYTE_TYPE_COMMAND)
#define LCD_SET_CURSOR(pos, line)   lcdSendByte(LCD_COMMAND_SET_CURSOR | line | pos, BYTE_TYPE_COMMAND)

#define UINT_BCD_MULTIPLIERS {10000, 1000, 100, 10, 1}
#define UINT_STRING_BUFF_LEN 6  // For max of unsigned int value (65535) and end of file symbol

void initLcd();
void lcdPulseEnablePin();
void lcdSendNibble(uint8_t nibbleToSend);
void lcdSendByte(uint8_t byteToSend, uint8_t byteType);
void lcdWriteString(char *string);
void waitBusyFlag();
void lcdClearArea(uint8_t dispLine, uint8_t fromPos, uint8_t toPos);
void uintToBcd(uint16_t number, char *bcdStringPtr);
void lcdReplaceData(char *oldData, char *newData, uint8_t line);
void lcdWriteString_P(PGM_P pgm_string);

#endif /* ST7066_H_ */
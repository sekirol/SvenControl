
#define F_CPU 8000000UL
#include <util/delay.h>
#include <string.h>

#include "ST7066.h"

//=============================================================================
void initLcd()
{
    lcdSendNibble(0x03);
    _delay_ms(50);
    lcdSendNibble(0x03);
    _delay_ms(5);
    lcdSendNibble(0x03);
    
    _delay_ms(1);
    lcdSendNibble(0x02);
        
    lcdSendByte(LCD_COMMAND_DISPLAY_FORMAT, BYTE_TYPE_COMMAND);
    lcdSendByte(LCD_COMMAND_DISPLAY_ENTRYM, BYTE_TYPE_COMMAND);
    lcdSendByte(LCD_COMMAND_CLEAR_DISPLAY, BYTE_TYPE_COMMAND);
    lcdSendByte(LCD_COMMAND_DISPLAY_ENABLE, BYTE_TYPE_COMMAND);
}
//=============================================================================
void lcdWriteString(char *string)
{
    while ((string != 0) && (*string != '\0'))
    {
        lcdSendByte(*string++, BYTE_TYPE_DATA);
    }
}
//=============================================================================
void lcdSendNibble(uint8_t nibbleToSend)
{
    // Clear data on LCD data bus
    LCD_DATA_PORT &= ~(LCD_DATA_PIN_MASK);
    
    // Set data on LCD data bus
    LCD_DATA_PORT |= (LCD_DATA_PIN_MASK & nibbleToSend);
    
    // Set LCD pin E and clear
    lcdPulseEnablePin();
}
//=============================================================================
void lcdSendByte(uint8_t byteToSend, uint8_t byteType)
{
    waitBusyFlag();
    
    if (byteType == BYTE_TYPE_COMMAND)
    {
        LCD_CTRL_PORT &= ~(LCD_RS_PIN);
    }
    else // (byteType == BYTE_TYPE_DATA)
    {
        LCD_CTRL_PORT |= LCD_RS_PIN;
    }
    
    // Send High Nibble
    lcdSendNibble( (byteToSend & 0xF0)>>4 );
    // Send Low Nibble
    lcdSendNibble( byteToSend & 0x0F );
}
//=============================================================================
void waitBusyFlag()
{
    // Clear data on bus
    LCD_DATA_PORT &= ~(LCD_DATA_PIN_MASK);
    // Change D7 pin to input
    LCD_DATA_DDR &= ~(LCD_D7_PIN);
    
    // Set read instruction mode
    LCD_CTRL_PORT |= LCD_RW_PIN;
    LCD_CTRL_PORT &= ~LCD_RS_PIN;
    
    // Busy Flag checking
    while ((LCD_DATA_PIN & LCD_D7_PIN) != 0)
    {
        _delay_us(100);
    }
    
    // Change D7 pin to output
    LCD_DATA_DDR |= LCD_D7_PIN;
    // Set write mode
    LCD_CTRL_PORT &= ~(LCD_RW_PIN);
}    
//=============================================================================
void lcdPulseEnablePin()
{
    LCD_CTRL_PORT &= ~LCD_E_PIN;
    _delay_us(10);
    
    LCD_CTRL_PORT |= LCD_E_PIN;
    _delay_ms(10);
    
    LCD_CTRL_PORT &= ~LCD_E_PIN;
    _delay_ms(10);
}
//=============================================================================
void uintToBcd(uint16_t number, char *bcdStringPtr)
{
    uint16_t multiplier[] = UINT_BCD_MULTIPLIERS;
    char *highDigitPtr = bcdStringPtr;
    uint8_t decPlace = 0;   // 0 - Highest digit of number
    uint8_t digit    = 0;   // Every decimal place start  position 
    
    while (decPlace < UINT_STRING_BUFF_LEN - 1)
    {
        digit = 0;
        while (number >= multiplier[decPlace])
        {
            number -= multiplier[decPlace];
            digit++;
        }
    
        // If digit is zero when it is first position on bcd buffer - stay on current position
        if (!((bcdStringPtr == highDigitPtr) && (digit == 0)))
        {
            *bcdStringPtr = digit + '0';
            bcdStringPtr++;
        }
        
        decPlace++;
    }
    
    // If number is zero - display zero
    if ((highDigitPtr == bcdStringPtr) && (digit == 0))
    {
        *bcdStringPtr = '0';
        bcdStringPtr++;
    }
    
    *bcdStringPtr = '\0';
}
//=============================================================================
//
//=============================================================================
void lcdReplaceData(char *oldData, char *newData, uint8_t line)
{
    uint8_t i;
 
    size_t oldDataLen = strlen(oldData);   
    
    for (i = 0; newData[i] != '\0'; i++)
    {
        if ( (i > oldDataLen) || (oldData[i] != newData[i]) )
        {
            LCD_SET_CURSOR(i, line);
            lcdSendByte(newData[i], BYTE_TYPE_DATA);
            oldData[i] = newData[i];
        }
    }
    oldData[i] = '\0';
    
    while (i < oldDataLen)
    {
        LCD_SET_CURSOR(i, line);
        lcdSendByte(' ', BYTE_TYPE_DATA);
        i++;
    }
    
}
//=============================================================================
// Write string on LCD from flash ROM
//=============================================================================
void lcdWriteString_P(PGM_P pgm_string)
{
    char symb = pgm_read_byte(pgm_string);
    while (symb != '\0')
    {
        lcdSendByte(symb, BYTE_TYPE_DATA);
        symb = (char) pgm_read_byte(++pgm_string);
    }
}
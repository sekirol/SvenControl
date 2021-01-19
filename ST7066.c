
#define F_CPU 8000000UL
#include <util/delay.h>

#include "ST7066.h"

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

void lcdWriteString(char *string)
{
    while ((string != 0) && (*string != '\0'))
    {
        lcdSendByte(*string, BYTE_TYPE_DATA);
        string++;
    }
}

void lcdSendNibble(uint8_t nibbleToSend)
{
    // Clear data on LCD data bus
    LCD_DATA_PORT &= ~(LCD_DATA_PIN_MASK);
    
    // Set data on LCD data bus
    LCD_DATA_PORT |= (LCD_DATA_PIN_MASK & nibbleToSend);
    
    // Set LCD pin E and clear
    lcdPulseEnablePin();
}

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

void lcdPulseEnablePin()
{
    LCD_CTRL_PORT &= ~LCD_E_PIN;
    _delay_us(10);
    
    LCD_CTRL_PORT |= LCD_E_PIN;
    _delay_ms(10);
    
    LCD_CTRL_PORT &= ~LCD_E_PIN;
    _delay_ms(10);
}

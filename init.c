
#include "init.h"
#include "ST7066.h"

void initGpio()
{
    // Clear data from LCD control bus
    LCD_CTRL_PORT &= ~LCD_CTRL_PIN_MASK;
    // Set LCD control bus pins as output
    LCD_CTRL_DDR |= LCD_CTRL_PIN_MASK;
    
    // Clear data from LCD data bus
    LCD_DATA_PORT &= ~LCD_DATA_PIN_MASK;
    // Set LCD data bus pins as output
    LCD_DATA_DDR |= LCD_DATA_PIN_MASK;
    
    // Clear data from buttons pins
    BUTTONS_PORT &= ~INPUT_PIN_MASK;
    // Set buttons pins as input
    BUTTONS_DDR &= ~INPUT_PIN_MASK;
}

void initInterrupts()
{
    GIFR  |= (1<<INTF0);    // Clear External Interrupt Request 0 flag
    TIFR  |= (1<<TOV0);     // Clear Timer0 overflow interrupt flag
    
    MCUCR |= (1<<ISC01);    // The falling edge of INT0 generates an interrupt request.
    GICR  |= (1<<INT0);     // External Interrupt Request 0 Enable
    
    TIMSK |= 1<<TOIE0;      // Timer0 overflow interrupt enable
}
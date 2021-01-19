
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "init.h"
#include "ST7066.h"

struct
{
    uint8_t selChan, frVol, ctrVol, bckVol, subVol;
} settings;

volatile uint8_t pressedButton = 0;

int main(void)
{    
    initGpio();
    initLcd();
    
    initIRQ0();
    TIMSK |= 1<<TOIE0;            // Timer0 overflow interrupt enable
    
    sei();
    
    lcdWriteString("Nothing");
    
    while(1)
    {      
        if (pressedButton)
        {
            LCD_CLEAR_DISPLAY;
            
            switch (pressedButton)
            {
            case 1:
                 lcdWriteString("First");
                 break;
            case 2:
                 lcdWriteString("Second");
                 break;
            case 3:
                 lcdWriteString("Third");
                 break;
            case 4:
                 lcdWriteString("Fourth");
                 break;
            }
        
            pressedButton = 0;
        }
    }
}

ISR(INT0_vect)
{
    // Debounce pause
    GICR  &= ~(1<<INT0);            // Disable External Interrupt Request 0
    TIFR |= (1<<TOV0);              // Clear Timer0 overflow interrupt flag
    TCNT0 = 0;                      // Set timer value
    TCCR0 |= (1<<CS02 | 1<<CS00);   // Start timer - prescaler 1024
    
    // Which button is pressed
    switch (~(BUTTONS_PIN) &  BUTTON_PIN_MASK)
    {
    case BTN1_PIN:
        pressedButton = 1;
        break;
    case BTN2_PIN:
        pressedButton = 2;
        break;
    case BTN3_PIN:
        pressedButton = 3;
        break;
    case BTN4_PIN:
        pressedButton = 4;
        break;
    }
}

ISR(TIMER0_OVF_vect)
{    
    TCCR0 &= ~(1<<CS02 | 1<<CS01 | 1<<CS00);   // Stop debounce timer
    GIFR |= (1<<INTF0);                        // Clear External Interrupt Request 0 flag
    GICR |= (1<<INT0);                         // External Interrupt Request 0 Enable
}
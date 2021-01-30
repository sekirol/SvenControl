
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "init.h"
#include "ST7066.h"
#include "display.h"
#include "control.h"

volatile uint8_t needUpdate;

int main(void)
{
    
    initGpio();
    initLcd();
    initInterrupts();
    sei();
       
    while(1)
    {
        displayDataUpdate();
        
        _delay_ms(100);
    }
}

ISR(INT0_vect)
{    
    uint8_t *selChanPtr = &currentSettings.selChan;
    uint8_t *volChanPtr = &currentSettings.curVolChan;
    uint8_t *volumePtr  = &currentSettings.volumeValues[*volChanPtr];
    
    switch (~(BUTTONS_PIN) & BUTTON_PIN_MASK)
    {
        // Volume UP
        case BTN1_PIN:
            *volumePtr = (*volumePtr < MAX_VOLUME_VALUE) ? *volumePtr + 1: MAX_VOLUME_VALUE;
            break;
        // Volume DOWN
        case BTN2_PIN:
            *volumePtr = (*volumePtr > MIN_VOLUME_VALUE) ? *volumePtr - 1: MIN_VOLUME_VALUE;
            break;
        // Change volume channel
        case BTN3_PIN:
           *volChanPtr = (*volChanPtr < VOLUME_CHANNELS_NUMBER-1) ? *volChanPtr + 1 : 0;
           break;
        // Change input channel
        case BTN4_PIN:
            *selChanPtr = (*selChanPtr < INPUT_CHANNELS_NUMBER-1) ? *selChanPtr + 1 : 0;
            break;
    }
     
    // Debounce pause
    GICR  &= ~(1<<INT0);            // Disable External Interrupt Request 0
    TIFR |= (1<<TOV0);              // Clear Timer0 overflow interrupt flag
    TCNT0 = 0;                      // Set timer value
    TCCR0 |= (1<<CS02 | 1<<CS00);   // Start timer - prescaler 1024
     
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t delay = DEBOUNCE_DELAY_VALUE;
    
    if (delay)
    {
        delay--;
    }
    else
    {
        delay = DEBOUNCE_DELAY_VALUE;
        
        TCCR0 &= ~(1<<CS02 | 1<<CS01 | 1<<CS00);   // Stop debounce timer
        //MCUCR |= (1<<ISC01| 1<<ISC00);           // The rising edge of INT0 generates an interrupt request
        GIFR  |= (1<<INTF0);                       // Clear External Interrupt Request 0 flag
        GICR  |= (1<<INT0);                        // External Interrupt Request 0 Enable    
    }
}
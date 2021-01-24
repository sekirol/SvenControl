
#include <avr/pgmspace.h>

#include "ST7066.h"
#include "diplay.h"

//-- Progmem defenitions ------------------------------------------------------
const char volumeCannelString0[] PROGMEM = "Fnt volume: ";
const char volumeCannelString1[] PROGMEM = "Bck volume: ";
const char volumeCannelString2[] PROGMEM = "Ctr volume: ";
const char volumeCannelString3[] PROGMEM = "Sub volume: ";

PGM_P const volumeCannelStrings[] PROGMEM = {volumeCannelString0, volumeCannelString1, volumeCannelString2, volumeCannelString3};

const char inputChannelString[] PROGMEM = "Input channel: ";
//-----------------------------------------------------------------------------

void displayDataUpdate(uint8_t volume)
{
    LCD_RETURN_HOME;
    
    char volumeString[UINT_STRING_BUFF_LEN] = {0};
    PGM_P curVolumeString = pgm_read_ptr(&volumeCannelStrings[0]);
    
    uintToBcd(volume, volumeString);
    
    lcdWriteString_P(curVolumeString);
    lcdWriteString(volumeString);
    
    LCD_GOTO_2_LINE;
    
    lcdWriteString_P(inputChannelString);
    lcdWriteString(volumeString);
    
}
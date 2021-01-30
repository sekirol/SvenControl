
#include <avr/pgmspace.h>

#include "ST7066.h"
#include "display.h"
#include "main.h"
#include "control.h"

//-- Progmem defenitions ------------------------------------------------------
//const char volumeChannelString0[] PROGMEM = "Fnt volume: ";
//const char volumeChannelString1[] PROGMEM = "Bck volume: ";
//const char volumeChannelString2[] PROGMEM = "Ctr volume: ";
//const char volumeChannelString3[] PROGMEM = "Sub volume: ";

const char volumeChannelString0[] PROGMEM = "Front volume: ";
const char volumeChannelString1[] PROGMEM = "Back volume: ";
const char volumeChannelString2[] PROGMEM = "Center volume: ";
const char volumeChannelString3[] PROGMEM = "Subwoofer volume: ";

PGM_P const volumeChannelStrings[] PROGMEM = {volumeChannelString0, volumeChannelString1, volumeChannelString2, volumeChannelString3};

const char inputChannelString0[] PROGMEM = "Input channel one";
const char inputChannelString1[] PROGMEM = "Input channel two";
const char inputChannelString2[] PROGMEM = "Input channel three";
const char inputChannelString3[] PROGMEM = "Input channel four";

PGM_P const inputChannelStrings[] PROGMEM = {inputChannelString0, inputChannelString1, inputChannelString2, inputChannelString3};
//-----------------------------------------------------------------------------

void displayDataUpdate()
{   
    static char lcdFirstLineData[LCD_LINE_LENGTH];
    static char lcdSecondLineData[LCD_LINE_LENGTH];
    
    char stringData[LCD_LINE_LENGTH] = {0};
    
    uint8_t *selChanPtr = &currentSettings.selChan;
    uint8_t *volChanPtr = &currentSettings.curVolChan;
    uint8_t *volumePtr  = &currentSettings.volumeValues[*volChanPtr];    
    
    PGM_P curVolumeString = pgm_read_ptr(&volumeChannelStrings[*volChanPtr]);
    PGM_P curInputString =  pgm_read_ptr(&inputChannelStrings[*selChanPtr]);
    
    // Prepare data and display first string
    memcpy_P(stringData, curVolumeString, strlen_P(curVolumeString));
    uintToBcd(*volumePtr, &stringData[strlen_P(curVolumeString)]);
    lcdReplaceData(lcdFirstLineData, stringData, LCD_FIRST_LINE);
    
    // Clear string data
    for (uint8_t i = 0; i < LCD_LINE_LENGTH; i++)
    {
        stringData[i] = 0;
    }
    
    // Prepare data and display second string
    memcpy_P(stringData, curInputString, strlen_P(curInputString));
    lcdReplaceData(lcdSecondLineData, stringData, LCD_SECOND_LINE);
}
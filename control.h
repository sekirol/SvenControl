#ifndef CONTROL_H_
#define CONTROL_H_

#define VOLUME_CHANNEL_FRONT  0
#define VOLUME_CHANNEL_BACK   1
#define VOLUME_CHANNEL_CENTER 2
#define VOLUME_CHANNEL_SUB    3

#define VOLUME_CHANNELS_NUMBER 4
#define INPUT_CHANNELS_NUMBER  4

#define MAX_VOLUME_VALUE 100
#define MIN_VOLUME_VALUE 0

struct settings_struct
{
    uint8_t selChan;                               // Selected input channel number 
    uint8_t curVolChan;                            // Selected volume edit channel number
    uint8_t volumeValues[VOLUME_CHANNELS_NUMBER];  // Volume value on all out channels
};

extern struct settings_struct currentSettings;

#endif /* CONTROL_H_ */
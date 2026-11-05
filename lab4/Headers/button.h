#ifndef _BUTTON_H
#define _BUTTON_H

#include "main.h"

extern volatile uint16_t ledBright[3];

extern volatile uint8_t selected;
void buttonInit(uint32_t clockFreq);
void encoderInit(uint32_t clockFreq);
    
#endif
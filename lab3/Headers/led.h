#ifndef _LED_H
#define _LED_H

#include "main.h"

void ledInit();
void initPwm(uint32_t clockPeriod);

typedef struct{
    uint32_t* port;
    uint16_t  pin;
}ledPin_t;


#endif
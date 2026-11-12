#ifndef _DELAY_H
#define _DELAY_H

#include "main.h"

void customDelayMs(uint32_t delay);
void sysTickInit(uint32_t clockPeriod);
void customDelayUs(uint32_t delay);


#endif
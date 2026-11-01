#ifndef _UART_COMMUNICATION_H
#define _UART_COMMUNICATION_H

#include "main.h"
void communicationInit(uint32_t clockFreq);
void sendData(uint8_t* data, uint16_t size);
void sendStr(uint8_t* data);
#endif
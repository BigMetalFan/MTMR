#ifndef _I2C_H
#define _I2C_H

#include "main.h"

#define TIMEOUT 0xFFFU

void i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len);

#endif
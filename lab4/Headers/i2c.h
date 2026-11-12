#ifndef _I2C_H
#define _I2C_H

#include "stm32f4xx.h"

#define TIMEOUT 0xFFFU

void i2cInit(uint32_t clockPeriod);
int i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len);
int i2cReadRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t count);
int i2cWriteData(uint8_t addr, uint8_t* data, uint8_t len);

int i2cWriteConstant(uint8_t addr, uint8_t reg, uint8_t data, uint16_t len);

#endif
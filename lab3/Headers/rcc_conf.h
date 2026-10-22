#ifndef _RCC_CONF_H
#define _RCC_CONF_H

#include "main.h"

#define ON_HSI  0
#define ON_HSE  1
#define ON_PLL  2

uint32_t initRcc(uint8_t mode);

#endif
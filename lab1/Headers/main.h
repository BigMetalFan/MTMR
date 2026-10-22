#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "rcc_conf.h"
#include "delay.h"
#include "led.h"
#include "button.h"

#define ON_HSI  0
#define ON_HSE  1
#define ON_PLL  2

#define USER_SOURCE ON_PLL

#endif
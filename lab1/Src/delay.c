#include "delay.h"


void sysTickInit(uint32_t clockFreq){
	SysTick->CTRL |= 1 << SysTick_CTRL_CLKSOURCE_Pos;
	SysTick->CTRL |= 1 << SysTick_CTRL_TICKINT_Pos;
    SysTick->LOAD |= ((clockFreq/1000 - 1)&SysTick_LOAD_RELOAD_Msk) << SysTick_LOAD_RELOAD_Pos;
}

void SysTick_Handler(){
	SysTick->CTRL &= ~(1<< SysTick_CTRL_ENABLE_Pos);
}

void customDelayMS(){
    SysTick->VAL = 0;
	SysTick->CTRL |= 1<< SysTick_CTRL_ENABLE_Pos;
    while(SysTick->CTRL&(1<< SysTick_CTRL_ENABLE_Pos));
}

void customDelay(uint16_t delay){
	while(delay--){
		customDelayMS();
	}
}
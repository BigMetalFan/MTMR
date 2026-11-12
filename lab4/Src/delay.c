#include "delay.h"


void sysTickInit(uint32_t clockFreq){
	SysTick->CTRL |= 1 << SysTick_CTRL_CLKSOURCE_Pos;
    SysTick->LOAD |= ((clockFreq/1000000 - 1)&SysTick_LOAD_RELOAD_Msk) << SysTick_LOAD_RELOAD_Pos;
}


void customDelay1Ms(){
    SysTick->CTRL = 0;                    
    SysTick->VAL  = 0;                    
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));   
    SysTick->CTRL = 0;
}

void customDelayMs(uint32_t delay){
    if(delay == 0) return;
    uint32_t delay_us = delay*1000;
	while(delay_us--){
		customDelay1Ms();
	}
}

void customDelayUs(uint32_t delay){
    if(delay == 0) return;
	while(delay--){
		customDelay1Ms();
	}
}
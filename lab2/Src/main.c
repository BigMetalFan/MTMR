#include <stdint.h>
#include "main.h"


uint32_t clockPeriod = 0;

volatile uint16_t pwmVal = 0;

int main(void)
{
	
    clockPeriod = initRcc(USER_SOURCE);
    sysTickInit(clockPeriod);
    ledInit();
    buttonInit(clockPeriod);
    initMeasure(clockPeriod);
    initPwm(clockPeriod);

	for(;;){
        customDelay(100);
	}
}

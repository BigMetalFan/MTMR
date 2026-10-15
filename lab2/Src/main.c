#include <stdint.h>
#include "main.h"
#include "stdio.h"

uint32_t clockPeriod = 0;

extern volatile uint16_t adcValue;


int main(void)
{
	
    clockPeriod = initRcc(USER_SOURCE);
    sysTickInit(clockPeriod);
    ledInit();
    buttonInit(clockPeriod);
    initMeasure(clockPeriod);
    initPwm(clockPeriod);
    startMeasure();
    communicationInit(clockPeriod);

	for(;;){
        customDelay(1000);
        char sendData[255];
        
        sprintf(sendData, "data: %d\n",adcValue);
        
        sendStr((uint8_t*)sendData);
	}
}

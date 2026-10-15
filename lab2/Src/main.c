#include <stdint.h>
#include "main.h"
#include "stdio.h"

uint32_t clockPeriod = 0;

//extern volatile uint16_t adcValue;
extern volatile uint16_t valArray[2];

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
        
        sprintf(sendData, "data1: %d, data2: %d\n",valArray[0],valArray[1]);
        
        sendStr((uint8_t*)sendData);
	}
}

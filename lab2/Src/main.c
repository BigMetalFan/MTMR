#include <stdint.h>
#include "main.h"
#include "stdio.h"

uint32_t clockFreq = 0;

extern volatile uint16_t valArray[2];

int main(void)
{
	
    clockFreq = initRcc(USER_SOURCE);
    sysTickInit(clockFreq);
    ledInit();
    buttonInit(clockFreq);
    initMeasure(clockFreq);
    initPwm(clockFreq);
    startMeasure();
    communicationInit(clockFreq);

	for(;;){
        customDelay(1000);
        char sendData[255];
        
        sprintf(sendData, "data1: %d, data2: %d\n",valArray[0],valArray[1]);
        
        sendStr((uint8_t*)sendData);
	}
}

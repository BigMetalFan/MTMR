#include <stdint.h>
#include "main.h"
#include "stdio.h"

uint32_t clockFreq = 0;

extern volatile uint16_t valArray[2];
extern volatile float tempData;
int main(void)
{
	
    clockFreq = initRcc(USER_SOURCE);
    sysTickInit(clockFreq);
    initMeasure(clockFreq);
    startMeasure();
    communicationInit(clockFreq);

	for(;;){
        customDelay(1000);
        char sendData[255];
        
        sprintf(sendData, "data1: %d, data2: %d, temp: %f\n",valArray[0],valArray[1], tempData);
        
        sendStr((uint8_t*)sendData);
	}
}

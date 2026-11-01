#include <stdint.h>
#include "main.h"



extern volatile uint8_t dir;

uint16_t pbArray[] = {1,0,0,0,0,0,0};
uint16_t paArray[] = {0,128,64,32,16,8,4};

int main(void)
{
	
    uint32_t clockFreq = initRcc(USER_SOURCE);
    sysTickInit(clockFreq);
    ledInit();
    buttonInit(clockFreq);

	for(;;){
        
        for(int i = dir ? 0: 6;dir? i <7:i >= 0; dir? i++: i--){
            
            GPIOA->ODR = paArray[i];
            GPIOB->ODR = pbArray[i];
            customDelay(250);
        }
	}
}

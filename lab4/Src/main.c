#include <stdint.h>
#include "main.h"
#include "stdio.h"
#include "ssd1306.h"
#include "string.h"
uint32_t clockFreq = 0;


char select[3] = {0,0,0};
uint16_t lastLedBright[3] = {0};


char menuText[3][10] = {0};
char itemNames[3] = {'R','G','B'};

uint8_t lastSelect = 0;
int main(void)
{
    clockFreq = initRcc(ON_HSI);
    buttonInit(clockFreq);
    encoderInit(clockFreq);
    ledInit();
    initPwm(clockFreq);
    i2cInit(clockFreq);
    sysTickInit(clockFreq);
    ssd1306Init();
    
    lastSelect = 1;
	for(;;){
    
    memset(select,0,3);
    select[selected] = '+';
        

    
    if(selected != lastSelect || memcmp((void*)ledBright, (void*)lastLedBright, 6)){
        memcpy((void*)lastLedBright,(void*)ledBright, 6);
        lastSelect = selected;
        
        for(int i = 0; i<3; i++){
            sprintf(menuText[i], "%c: %d %c",itemNames[i],ledBright[i],select[i]);
        }
        ssd1306Clear();
        ssd1306DrawWholeString(0, 0, "RGB Controller");
        ssd1306DrawWholeString(2, 0, menuText[0]);
        ssd1306DrawWholeString(3, 0, menuText[1]);
        ssd1306DrawWholeString(4, 0, menuText[2]);
    }
    
        
//        ssd1306Clear();
//        ssd1306DrawString(0, 0, "RGB Controller");
//        ssd1306DrawString(2, 0, menuText[0]);
//        ssd1306DrawString(3, 0, menuText[1]);
//        ssd1306DrawString(4, 0, menuText[2]);
       
        
    }
    
}

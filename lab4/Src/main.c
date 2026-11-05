#include <stdint.h>
#include "main.h"
#include "stdio.h"
#include "ssd1306.h"
#include "string.h"
uint32_t clockFreq = 0;

char select[3] = {0,0,0};
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
    ssd1306Init();
    

	for(;;){
    
//    memset(select,0,3);
//    select[selected] = '+';
        
//    for(int i = 0; i<3; i++){
//        sprintf(menuText[i], "%c: %d %c",itemNames[i],ledBright[i],select[i]);
//        
//        
//    }
    /*
    if(selected != lastSelect){

    }*/
        lastSelect = selected;
        ssd1306Clear();
        ssd1306DrawString(0, 0, "RGB Controller");
        ssd1306DrawString(2, 0, menuText[0]);
        ssd1306DrawString(3, 0, menuText[1]);
        ssd1306DrawString(4, 0, menuText[2]);
    }
}

#include "uartCommunication.h"

void communicationInit(uint32_t clockPeriod){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    GPIOA->MODER |= (2<<GPIO_MODER_MODE2_Pos) | (2<<GPIO_MODER_MODE3_Pos);
    
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos)|(7 << GPIO_AFRL_AFSEL3_Pos);
    
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE;
    USART2->BRR = clockPeriod/115200;
    
}

void usartSendByte(uint8_t byte){
    USART2->DR = byte;
    while(!(USART2->SR&USART_SR_TC));
}

void sendData(uint8_t* data, uint16_t size){
    for(uint16_t i = 0; i<size; i++){
        usartSendByte(data[i]);
    }
}

void sendStr(uint8_t* data){
        for(uint16_t i = 0; ; i++){
            if(data[i] == 0) return;
            usartSendByte(data[i]);
    }
}
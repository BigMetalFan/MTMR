#include "potent.h"

volatile uint16_t adcValie;

void initMeasure(uint32_t clockPeriod){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    
    GPIOA->MODER |= 3<<GPIO_MODER_MODE1_Pos;
    
    TIM3->PSC = clockPeriod/10000000-1;
    TIM3->ARR = 1000-1;
    TIM3->CR2 |= 2<<TIM_CR2_MMS_Pos;
    
    ADC1->CR2 &= ~ADC_CR2_EXTEN;
    ADC1->CR2 |= (1 << ADC_CR2_EXTEN_Pos);
    ADC1->CR2 |= 8<<ADC_CR2_EXTSEL_Pos;
    ADC1->SQR1 = (0 << ADC_SQR1_L_Pos);
    ADC1->SQR3 = 1 << ADC_SQR3_SQ1_Pos;
    ADC1->SMPR2 |= 4<<ADC_SMPR2_SMP1_Pos;
    
    ADC1->CR1 |= 1<<ADC_CR1_EOCIE_Pos;
    
    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    
    NVIC_EnableIRQ(ADC_IRQn);
    
    ADC1->CR2 |= ADC_CR2_ADON;
}

void ADC_IRQHandler(){
    if(ADC1->SR & ADC_SR_EOC){
        ADC1->SR &= ~ADC_SR_EOC;
        adcValie = ADC1->DR;
        TIM11->CCR1 = ADC1->DR;
    }
}

void startMeasure(){
    TIM3->CR1 |= TIM_CR1_CEN;
}

void stopMeasure(){
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

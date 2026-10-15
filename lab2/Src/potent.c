#include "potent.h"

volatile uint16_t adcValue;

volatile uint16_t valArray[2] = {0};

void initMeasure(uint32_t clockPeriod){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    
    GPIOA->MODER |= 3<<GPIO_MODER_MODE1_Pos|3<<GPIO_MODER_MODE0_Pos;
    
    TIM3->PSC = clockPeriod/1000000-1;
    TIM3->ARR = 1000-1;
    TIM3->CR2 |= 2<<TIM_CR2_MMS_Pos;
    
    
    ADC1->CR2 &= ~ADC_CR2_EXTEN;
    ADC1->CR2 |= (1 << ADC_CR2_EXTEN_Pos);
    ADC1->CR2 |= 8<<ADC_CR2_EXTSEL_Pos;
    ADC1->CR2 |= ADC_CR2_DMA_Msk;
    ADC1->CR2 |= ADC_CR2_DDS_Msk;
    ADC1->CR1 |= ADC_CR1_SCAN;
    
    ADC1->SQR1 = 1 << ADC_SQR1_L_Pos;
    ADC1->SQR3 = (1 << ADC_SQR3_SQ1_Pos)|(0 << ADC_SQR3_SQ2_Pos);
    ADC1->SMPR2 |= 4<<ADC_SMPR2_SMP1_Pos;
    
    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    
    ADC1->CR2 |= ADC_CR2_ADON;
    
    DMA2_Stream0->CR =  (1<<DMA_SxCR_MSIZE_Pos)
                        |(1<<DMA_SxCR_PSIZE_Pos)
                        |(1<<DMA_SxCR_MINC_Pos)
                        |(1<<DMA_SxCR_TCIE_Pos)
                        |(1<<DMA_SxCR_CIRC_Pos);
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = (uint32_t)valArray;
    DMA2_Stream0->NDTR = 2;
    DMA2_Stream0->CR |= DMA_SxCR_EN;
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Stream0_IRQHandler(){
    if(DMA2->LISR&DMA_LISR_TCIF0){
        DMA2->LIFCR = DMA_LIFCR_CTCIF0;
        
        if(valArray[0]<20){
            adcValue = 0;
            TIM11->CCR1 = 0;
        }
        else{
            adcValue = valArray[0];
            TIM11->CCR1 = valArray[0];
        }
    }
}

void startMeasure(){
    TIM3->CR1 |= TIM_CR1_CEN;
}

void stopMeasure(){
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

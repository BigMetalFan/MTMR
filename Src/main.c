#include <stdint.h>
#include "main.h"


uint32_t clockPeriod = 16000000;
volatile int dir = 0;

void initRcc(uint8_t mode){
    
    if(mode == 0){ //HSI
        return;
    }
    if(mode == 1){  //HSE
        RCC->CR |= 1<<RCC_CR_HSEON_Pos;
        while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos))){
        }
        RCC->CFGR |= RCC_CFGR_SW_HSE;
        clockPeriod = 25000000;
    }
    
    if(mode == 2){  //PLL
        RCC->CR |= 1<<RCC_CR_HSEON_Pos;
        RCC->PLLCFGR |=1 << RCC_PLLCFGR_PLLSRC_HSE_Pos;
        while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos))){
        }
        RCC->PLLCFGR = (1 << RCC_PLLCFGR_PLLSRC_Pos) |
        (25UL << RCC_PLLCFGR_PLLM_Pos) | (280UL << RCC_PLLCFGR_PLLN_Pos) | (3 << RCC_PLLCFGR_PLLP_Pos);
        RCC->CR |= RCC_CR_PLLON;
        RCC->CFGR = RCC_CFGR_SW_1;
        
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1WS;

        while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_1WS);
        
        clockPeriod = 35000000;
    }

}

void sysTickInit(){
	SysTick->CTRL |= 1 << SysTick_CTRL_CLKSOURCE_Pos;
	SysTick->CTRL |= 1 << SysTick_CTRL_TICKINT_Pos;
    SysTick->LOAD |= ((clockPeriod/1000)&SysTick_LOAD_RELOAD_Msk) << SysTick_LOAD_RELOAD_Pos;
}

void SysTick_Handler(){
	SysTick->CTRL &= ~(1<< SysTick_CTRL_ENABLE_Pos);
}

void customDelayMS(){
    SysTick->VAL = 0;
	SysTick->CTRL |= 1<< SysTick_CTRL_ENABLE_Pos;
    while(SysTick->CTRL&(1<< SysTick_CTRL_ENABLE_Pos));
}

void customDelay(uint16_t delay){
	while(delay--){
		customDelayMS();
	}
}

void ledInit(){

	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos;
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos;


    sysTickInit();
	GPIOA->MODER |= (1<<GPIO_MODER_MODE2_Pos) | (1<<GPIO_MODER_MODE3_Pos)| (1<<GPIO_MODER_MODE4_Pos) |
		 (1<<GPIO_MODER_MODE5_Pos) |  (1<<GPIO_MODER_MODE6_Pos)|  (1<<GPIO_MODER_MODE7_Pos);
	GPIOB->MODER |= (1<<GPIO_MODER_MODE0_Pos);
}


void buttonInit(){
    GPIOB->MODER &= ~GPIO_MODER_MODE12_Msk;
    
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI12;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;
    
    EXTI->IMR |= EXTI_IMR_IM12;
    //EXTI->EMR |= EXTI_EMR_EM12;
    EXTI->RTSR |= 1 << EXTI_RTSR_TR12_Pos;
    
    RCC->APB2ENR |= 1 << RCC_APB2ENR_TIM10EN_Pos;
    
    TIM10->CR1 |= TIM_CR1_OPM_Msk;
    TIM10->DIER |= TIM_DIER_UIE_Msk;
    
    TIM10->PSC = clockPeriod/10000-1;
    TIM10->ARR = 500-1;
    
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void EXTI15_10_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR12_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR12_Msk;
    }
}

void TIM1_UP_TIM10_IRQHandler(){
    if(TIM10->SR & TIM_SR_UIF_Msk){
        dir ^= 1;
        TIM10->SR &= ~TIM_SR_UIF;
    }
}

uint16_t pbArray[] = {1,0,0,0,0,0,0};
uint16_t paArray[] = {0,128,64,32,16,8,4};



int main(void)
{
	
    initRcc(ON_HSE);
    ledInit();

	for(;;){
        
        for(int i = dir ? 0: 7;dir? i <7:i > 0; dir? i++: i--){
            
            GPIOA->ODR = paArray[i];
            GPIOB->ODR = pbArray[i];
            customDelay(100);
        }
	}
}

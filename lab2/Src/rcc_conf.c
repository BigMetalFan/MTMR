#include "rcc_conf.h"

uint32_t initRcc(uint8_t mode){
    
    if(mode == 0){ //HSI
        return 16000000;
    }
    if(mode == 1){  //HSE
        RCC->CR |= 1<<RCC_CR_HSEON_Pos;
        while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos))){
        }
        RCC->CFGR |= RCC_CFGR_SW_HSE;
        return 25000000;
    }
    
    if(mode == 2){  //PLL
        RCC->CR |= 1<<RCC_CR_HSEON_Pos;
        while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos))){
        }
        RCC->PLLCFGR = (1 << RCC_PLLCFGR_PLLSRC_HSE_Pos) |
        (25UL << RCC_PLLCFGR_PLLM_Pos) | (280UL << RCC_PLLCFGR_PLLN_Pos) | (3 << RCC_PLLCFGR_PLLP_Pos);
        RCC->CR |= RCC_CR_PLLON;
        RCC->CFGR = RCC_CFGR_SW_1;
        
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1WS;

        while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_1WS);
        
        return 35000000;
    }
    
    return -1;

}
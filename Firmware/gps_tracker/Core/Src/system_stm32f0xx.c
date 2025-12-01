/**
  ******************************************************************************
  * @file    system_stm32f0xx.c
  * @brief   STM32F0xx system startup and clock configuration
  ******************************************************************************
  */

#include "stm32f0xx.h"
#include "stm32f0xx_hal_conf.h"

/* ===== CLOCK CONFIGURATION ===== */

uint32_t SystemCoreClock = 48000000UL;  // 48 MHz default

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

/* ===== SYSTEM INITIALIZATION ===== */

void SystemInit(void)
{
    // Set HSION bit
    RCC->CR |= RCC_CR_HSION;

    // Wait for HSI to be ready
    while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    {
    }

    // Reset CFGR register
    RCC->CFGR &= ~(RCC_CFGR_SW);

    // Reset CFGR2, CFGR3, and CIR registers
    RCC->CFGR2 = 0;
    RCC->CFGR3 = 0;
    RCC->CIR = 0;

    // Update SystemCoreClock
    SystemCoreClockUpdate();
}

void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0;
    uint32_t pllmul = 0;
    uint32_t plldiv = 0;

    // Get PLL multiplier
    tmp = RCC->CFGR & RCC_CFGR_PLLMUL;
    pllmul = (tmp >> RCC_CFGR_PLLMUL_Pos) + 2;

    // Get PLL divider
    tmp = RCC->CFGR2 & RCC_CFGR2_PREDIV;
    plldiv = (tmp >> RCC_CFGR2_PREDIV_Pos) + 1;

    // Calculate core clock
    if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
        // PLL is used as clock source
        SystemCoreClock = (HSE_VALUE / plldiv) * pllmul;
    } else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI) {
        // HSI is used as clock source
        SystemCoreClock = HSI_VALUE;
    } else {
        // HSE or other source
        SystemCoreClock = HSE_VALUE;
    }

    // Adjust for AHB prescaler
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
    SystemCoreClock >>= tmp;
}

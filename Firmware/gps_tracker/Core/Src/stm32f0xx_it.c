/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines
  ******************************************************************************
  */

#include "stm32f0xx_it.h"
#include "main.h"

/* ===== CORTEX-M0 PROCESSOR EXCEPTIONS ===== */

void NMI_Handler(void)
{
    // Non-maskable interrupt - system issue
    while (1)
    {
    }
}

void HardFault_Handler(void)
{
    // Hard fault - critical error
    while (1)
    {
    }
}

void SVC_Handler(void)
{
    // Supervisor call
}

void PendSV_Handler(void)
{
    // Pending supervisor call
}

void SysTick_Handler(void)
{
    // System tick - handles HAL_GetTick()
    HAL_IncTick();
}

/* ===== PERIPHERAL INTERRUPTS ===== */

void USART1_IRQHandler(void)
{
    // Debug/SIM808 UART
    HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void)
{
    // GPS UART
    HAL_UART_IRQHandler(&huart2);
}

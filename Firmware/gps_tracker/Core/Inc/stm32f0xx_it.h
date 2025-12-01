/**
 * @file stm32f0xx_it.h
 * @brief STM32F0xx interrupt handler declarations
 */

#ifndef STM32F0XX_IT_H
#define STM32F0XX_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ===== CORTEX-M0 PROCESSOR EXCEPTIONS ===== */

void NMI_Handler(void);

void HardFault_Handler(void);

void SVC_Handler(void);

void PendSV_Handler(void);

void SysTick_Handler(void);

/* ===== PERIPHERAL INTERRUPTS ===== */

void USART1_IRQHandler(void);

void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32F0XX_IT_H */

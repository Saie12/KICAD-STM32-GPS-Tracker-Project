/**
 * @file main.h
 * @brief Main header - GPIO pins and peripheral definitions
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

/* ===== GPIO PIN DEFINITIONS ===== */

#define LED1_Pin          GPIO_PIN_13
#define LED1_GPIO_Port    GPIOC

#define BUTTON1_Pin       GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOF

#define BUTTON2_Pin       GPIO_PIN_1
#define BUTTON2_GPIO_Port GPIOF

/* ===== PERIPHERAL HANDLES ===== */

extern UART_HandleTypeDef huart1;  /* Debug/SIM808 UART */
extern UART_HandleTypeDef huart2;  /* GPS UART */

/* ===== ERROR HANDLER ===== */

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

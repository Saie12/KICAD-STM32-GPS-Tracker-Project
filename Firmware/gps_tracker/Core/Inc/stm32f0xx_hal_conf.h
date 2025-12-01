/**
 * @file stm32f0xx_hal_conf.h
 * @brief STM32F0xx HAL Configuration - Module enablement and settings
 */

#ifndef STM32F0XX_HAL_CONF_H
#define STM32F0XX_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ===== HAL MODULES ===== */

#define HAL_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

/* ===== SYSTEM CLOCK CONFIGURATION ===== */

#define HSE_VALUE           8000000U  /* External oscillator frequency */
#define HSE_STARTUP_TIMEOUT 100U      /* HSE startup timeout in ms */
#define HSI_VALUE           8000000U  /* Internal oscillator frequency */

/* ===== UART CONFIGURATION ===== */

#define UART_BAUDRATE_USART1 115200U
#define UART_BAUDRATE_USART2 115200U

/* ===== HAL TICK TIMER ===== */

#define USE_HAL_UART_REGISTER_CALLBACKS 0U

/* ===== ASSERT MACRO ===== */

#define assert_param(expr) ((void)0U)

/* ===== DEBUG MACROS ===== */

#define __ALIGN_BEGIN
#define __ALIGN_END

#ifdef __cplusplus
}
#endif

#endif /* STM32F0XX_HAL_CONF_H */

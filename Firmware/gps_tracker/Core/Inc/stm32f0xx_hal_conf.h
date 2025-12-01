/**
 * @file stm32f0xx_hal_conf.h
 * @brief STM32F0xx HAL Configuration
 *        This file contains the system and HAL module enablement settings
 */

#ifndef STM32F0XX_HAL_CONF_H
#define STM32F0XX_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ===== EXTERNAL CLOCK CONFIGURATION ===== */

#define HSE_VALUE           8000000U   /* External oscillator frequency (8 MHz) */
#define HSE_STARTUP_TIMEOUT 100U       /* HSE startup timeout in milliseconds */
#define HSI_VALUE           8000000U   /* Internal oscillator frequency (8 MHz) */
#define LSI_VALUE           40000U     /* Low speed internal oscillator (40 kHz) */

/* ===== SYSTEM CLOCK CONFIGURATION ===== */

#define VDD_VALUE           3300U      /* Supply voltage in mV */
#define TICK_INT_PRIORITY   3U         /* SysTick interrupt priority (0-3, 3=lowest) */
#define USE_RTOS            0U         /* RTOS not used */

/* ===== CACHE & PREFETCH ===== */

#define PREFETCH_ENABLE             1U  /* Prefetch buffer enabled */
#define INSTRUCTION_CACHE_ENABLE    0U  /* Not available on STM32F0 */
#define DATA_CACHE_ENABLE           0U  /* Not available on STM32F0 */

/* ===== SPI CRC ===== */

#define USE_SPI_CRC 0U  /* SPI CRC disabled */

/* ===== HAL MODULE REGISTER CALLBACKS (disabled by default) ===== */

#define USE_HAL_ADC_REGISTER_CALLBACKS        0U
#define USE_HAL_CAN_REGISTER_CALLBACKS        0U
#define USE_HAL_COMP_REGISTER_CALLBACKS       0U
#define USE_HAL_CEC_REGISTER_CALLBACKS        0U
#define USE_HAL_DAC_REGISTER_CALLBACKS        0U
#define USE_HAL_I2C_REGISTER_CALLBACKS        0U
#define USE_HAL_SMBUS_REGISTER_CALLBACKS      0U
#define USE_HAL_UART_REGISTER_CALLBACKS       0U
#define USE_HAL_USART_REGISTER_CALLBACKS      0U
#define USE_HAL_IRDA_REGISTER_CALLBACKS       0U
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS  0U
#define USE_HAL_WWDG_REGISTER_CALLBACKS       0U
#define USE_HAL_RTC_REGISTER_CALLBACKS        0U
#define USE_HAL_SPI_REGISTER_CALLBACKS        0U
#define USE_HAL_I2S_REGISTER_CALLBACKS        0U
#define USE_HAL_TIM_REGISTER_CALLBACKS        0U
#define USE_HAL_TSC_REGISTER_CALLBACKS        0U
#define USE_HAL_PCD_REGISTER_CALLBACKS        0U

/* ===== ASSERT MACRO ===== */

/* Uncomment to enable assert_param() checks */
/* #define USE_FULL_ASSERT 1U */

/* ===== HAL MODULES - INCLUDE HEADERS ===== */

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f0xx_hal_rcc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f0xx_hal_gpio.h"
#endif

#ifdef HAL_EXTI_MODULE_ENABLED
#include "stm32f0xx_hal_exti.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f0xx_hal_dma.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f0xx_hal_cortex.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f0xx_hal_adc.h"
#endif

#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f0xx_hal_can.h"
#endif

#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f0xx_hal_cec.h"
#endif

#ifdef HAL_COMP_MODULE_ENABLED
#include "stm32f0xx_hal_comp.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f0xx_hal_crc.h"
#endif

#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f0xx_hal_dac.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f0xx_hal_flash.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f0xx_hal_i2c.h"
#endif

#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f0xx_hal_i2s.h"
#endif

#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f0xx_hal_irda.h"
#endif

#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f0xx_hal_iwdg.h"
#endif

#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f0xx_hal_pcd.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f0xx_hal_pwr.h"
#endif

#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f0xx_hal_rtc.h"
#endif

#ifdef HAL_SERIAL_WIRE_DEBUG_MODULE_ENABLED
#include "stm32f0xx_hal_swd.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f0xx_hal_spi.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f0xx_hal_tim.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f0xx_hal_uart.h"
#endif

#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f0xx_hal_usart.h"
#endif

#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f0xx_hal_wwdg.h"
#endif

#ifdef HAL_TSC_MODULE_ENABLED
#include "stm32f0xx_hal_tsc.h"
#endif

/* ===== ASSERT IMPLEMENTATION ===== */

#ifdef USE_FULL_ASSERT
    #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
    void assert_failed(uint8_t *file, uint32_t line);
#else
    #define assert_param(expr) ((void)0U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* STM32F0XX_HAL_CONF_H */

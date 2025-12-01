/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - STM32F030C8 GPS Tracker
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <unistd.h>
#include "sim808_config.h"
#include "sim808_driver.h"
#include "sim808_gps.h"
#include "sim808_gprs.h"
#include "sim808_power.h"
#include "gps_parser.h"
#include "gps_uart.h"

/* Private includes ----------------------------------------------------------*/

/* Demo NMEA sentences for self-test */
const char *demo_nmea_sentences[] = {
    "$GPRMC,092750.000,A,5321.6802,N,00630.3372,W,0.295,,121696,,,A*78",
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A",
    "$GPRMC,211353.570,A,3723.2475,N,12158.3416,W,0.546,153.1,210307,11.6,W*7F",
    "$GPRMC,000000,V,0000.0000,N,00000.0000,E,0.0,0.0,010101,0.0,E*7A",
};
#define DEMO_NMEA_COUNT (sizeof(demo_nmea_sentences) / sizeof(demo_nmea_sentences[0]))

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;  /* GPS UART */
UART_HandleTypeDef huart1;  /* Debug/SIM808 UART */

static gps_fix_t g_gps_fix;
static uint8_t sim808_gps_retry_count = 0;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void demo_gps_parser(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU Configuration */
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* Initialize peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART1_UART_Init();
    gps_uart_init();

    /* USER CODE BEGIN 2 */

    /* Initialize SIM808 subsystems */
    sim808_init();
    sim808_gps_init();
    sim808_gprs_init();
    sim808_power_init();
    sim808_power_set_watchdog(1);

    /* Print welcome message */
    printf("\r\n");
    printf("=============================================\r\n");
    printf("  STM32F030C8 GPS Tracker - Phases 1-7\r\n");
    printf("  Hardware-Free Testing Mode (SIM808 Mock)\r\n");
    printf("=============================================\r\n");
    printf("\r\n");

    /* Run GPS parser demo */
    demo_gps_parser();

    printf("System ready. Polling for data...\r\n");

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* Drive core tasks */
        sim808_task();
        sim808_power_task();

        /* ===== PERIODIC MONITORING (every 2 seconds) ===== */
        static uint32_t last_sim808_tick = 0;
        uint32_t now = HAL_GetTick();

        if (now - last_sim808_tick >= 2000U) {
            last_sim808_tick = now;

            /* --- SIM808 State Report --- */
            sim808_state_t st = sim808_get_state();
            printf("[SIM808] State: %s\r\n", sim808_state_to_str(st));

            if (st == SIM808_STATE_READY) {

                /* --- GPS Request with Retry Logic --- */
                sim808_result_t res = sim808_request_gps_info();
                if (res == SIM808_OK) {
                    printf("[SIM808] Requested GPS info (AT+CGNSINF)\r\n");
                } else {
                    sim808_gps_retry_count++;
                }

                /* Check error status for timeouts or rejections */
                sim808_error_status_t es;
                sim808_get_error_status(&es);
                if (es.last_error != SIM808_ERR_NONE) {
                    printf("[SIM808 ERROR] code=%d, count=%lu\r\n",
                           es.last_error, (unsigned long)es.error_count);

                    // Track consecutive failures
                    if (es.last_error == SIM808_ERR_TIMEOUT_OP ||
                        es.last_error == SIM808_ERR_CMD_REJECTED) {
                        sim808_gps_retry_count++;
                    }
                }

                // Auto-recovery: reinit after 3 failures
                if (sim808_gps_retry_count >= 3) {
                    printf("[SIM808] Too many GPS errors, reinitializing...\r\n");
                    sim808_init();
                    sim808_gps_init();
                    sim808_gprs_init();
                    sim808_power_init();
                    sim808_power_set_watchdog(1);
                    sim808_gps_retry_count = 0;
                }

                /* Try to fetch latest GPS fix from SIM808 */
                gps_fix_t sim_fix;
                if (sim808_gps_get_fix(&sim_fix) && sim_fix.valid_fix) {
                    printf("[SIM808 GPS] Lat: %.6f  Lon: %.6f\r\n",
                           sim_fix.latitude, sim_fix.longitude);
                } else {
                    printf("[SIM808 GPS] No valid fix yet\r\n");
                }
            }

            /* --- GPRS/TCP Demo (runs once) --- */
            static int gprs_test_done = 0;
            if (st == SIM808_STATE_READY && !gprs_test_done) {
                static uint32_t gprs_step_tick = 0;
                static int gprs_step = 0;

                if (gprs_step == 0) {
                    printf("[SIM808] Initiating GPRS attach...\r\n");
                    sim808_gprs_attach("internet");
                    gprs_step = 1;
                    gprs_step_tick = now;
                } else if (gprs_step == 1 && now - gprs_step_tick >= 1000U) {
                    printf("[SIM808] GPRS State: %s\r\n",
                           sim808_gprs_state_to_str(sim808_gprs_get_state()));
                    if (sim808_gprs_get_state() == SIM808_GPRS_CONNECTED) {
                        printf("[SIM808] Opening TCP connection...\r\n");
                        sim808_gprs_open_tcp("example.com", 80);
                        gprs_step = 2;
                        gprs_step_tick = now;
                    }
                } else if (gprs_step == 2 && now - gprs_step_tick >= 1000U) {
                    printf("[SIM808] Sending test payload...\r\n");
                    sim808_gprs_send_data("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
                    gprs_step = 3;
                    gprs_step_tick = now;
                } else if (gprs_step == 3 && now - gprs_step_tick >= 1000U) {
                    printf("[SIM808] Closing TCP connection...\r\n");
                    sim808_gprs_close();
                    gprs_test_done = 1;
                    printf("[SIM808 GPRS] Test complete!\r\n");
                }
            }

            /* --- Power Management Demo --- */
            static int power_demo_step = 0;
            if (power_demo_step == 0 && st == SIM808_STATE_READY) {
                printf("[SIM808 POWER] Current mode: %s\r\n",
                       sim808_power_mode_to_str(sim808_power_get_mode()));

                sim808_battery_t bat;
                if (sim808_power_get_battery(&bat) == SIM808_OK) {
                    printf("[SIM808 BATTERY] Voltage: %u mV, Charge: %d%%, Charging: %s\r\n",
                           bat.voltage_mv, bat.charge_percent,
                           bat.charging ? "Yes" : "No");
                }

                // Demo: cycle through power modes
                static uint32_t power_mode_tick = 0;
                if (power_mode_tick == 0U) {
                    power_mode_tick = now;
                }

                uint32_t elapsed = now - power_mode_tick;
                if (elapsed < 3000U) {
                    sim808_power_set_mode(SIM808_POWER_ACTIVE);
                } else if (elapsed < 6000U) {
                    sim808_power_set_mode(SIM808_POWER_IDLE);
                } else if (elapsed < 9000U) {
                    sim808_power_set_mode(SIM808_POWER_SLEEP);
                } else {
                    sim808_power_set_mode(SIM808_POWER_STANDBY);
                    power_demo_step = 1;  // Stop cycling
                }
            }
        }

        /* ===== LED HEARTBEAT ===== */
        static uint32_t last_blink = 0;
        if (HAL_GetTick() - last_blink >= 500U) {
            last_blink = HAL_GetTick();
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        }

        /* ===== REAL GPS DATA (if hardware connected) ===== */
        char nmea_line[GPS_LINE_BUFFER_SIZE];
        if (gps_uart_get_line(nmea_line, sizeof(nmea_line))) {
            gps_fix_t fix;
            if (gps_parse_nmea(nmea_line, &fix)) {
                if (fix.valid_fix) {
                    g_gps_fix = fix;
                    printf("[LIVE GPS] Lat: %.6f  Lon: %.6f\r\n",
                           g_gps_fix.latitude, g_gps_fix.longitude);
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                } else {
                    printf("[NO FIX]\r\n");
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                }
            }
        }

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief USART2 Initialization Function (GPS UART)
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief USART1 Initialization Function (Debug/SIM808)
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = BUTTON1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BUTTON2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

    /* USART1 pins: PA9 (TX), PA10 (RX) */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief Demo: Test GPS parser with sample NMEA sentences
  */
static void demo_gps_parser(void)
{
    printf("\n=== GPS Parser Demo Mode ===\n");
    printf("Testing %d NMEA sentences...\n\n", DEMO_NMEA_COUNT);

    gps_fix_t fix;

    for (int i = 0; i < (int)DEMO_NMEA_COUNT; i++) {
        printf("Sentence %d: %s\n", i + 1, demo_nmea_sentences[i]);

        if (gps_parse_nmea(demo_nmea_sentences[i], &fix)) {
            if (fix.valid_fix) {
                printf("  [VALID] Lat: %.6f  Lon: %.6f\n\n",
                       fix.latitude, fix.longitude);
            } else {
                printf("  [INVALID FIX] No GPS lock\n\n");
            }
        } else {
            printf("  [PARSE ERROR] Failed to parse\n\n");
        }
    }

    printf("=== Demo Complete ===\n\n");
}

/* USER CODE BEGIN 4 */

/* printf redirection to USART1 */
int _write(int file, char *ptr, int len)
{
    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 1000);
        return len;
    }
    return -1;
}

/* UART RX interrupt callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        gps_uart_on_rx_complete();
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif /* USE_FULL_ASSERT */

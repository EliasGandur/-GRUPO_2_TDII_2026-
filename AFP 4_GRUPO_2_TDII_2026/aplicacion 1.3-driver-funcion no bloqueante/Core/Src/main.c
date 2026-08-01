/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - Máquina de estados para leds y botón
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "API_GPIO.h"
#include "API_Delay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    MODO_APAGADO = 0,
    MODO_SECUENCIA_150MS, // Modo 1: Alternancia de los 3 LEDs a 150ms
    MODO_TODOS_600MS,     // Modo 2: Parpadeo simultáneo a 600ms
    MODO_INDEPENDIENTE,   // Modo 3: LED1 (100ms), LED2 (300ms), LED3 (600ms)
    MODO_INVERSO_150MS    // Modo 4: LED1 y 3 en fase, LED2 en contrafase a 150ms
} modo_secuencia_t;
/* USER CODE END PTD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
modo_secuencia_t modoActual = MODO_APAGADO;

// Delays para los diferentes modos
delay_t delayModo1;
delay_t delayModo2;
delay_t delayLed1, delayLed2, delayLed3;
delay_t delayModo4;

// Variables de estado interno para secuencias
uint8_t pasoSecuencia = 0;
bool_t estadoInverso = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void apagarTodosLosLeds(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void apagarTodosLosLeds(void) {
    writePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    writePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    writePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  API_GPIO_Init();

  // Inicialización de delays no bloqueantes
  delayInit(&delayModo1, 150);
  delayInit(&delayModo2, 600);
  delayInit(&delayLed1, 100);
  delayInit(&delayLed2, 300);
  delayInit(&delayLed3, 600);
  delayInit(&delayModo4, 150);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    // 1. Detección del botón para cambio de modo
    if (API_GPIO_IsButtonPressed()) {
        apagarTodosLosLeds();
        pasoSecuencia = 0;
        estadoInverso = false;

        modoActual++;
        if (modoActual > MODO_INVERSO_150MS) {
            modoActual = MODO_SECUENCIA_150MS; // Vuelve al primer modo
        }
    }

    // 2. Ejecución del modo seleccionado (Síncrono y No Bloqueante)
    switch (modoActual) {

        case MODO_APAGADO:
            apagarTodosLosLeds();
            break;

        /* MODO 1: Alternancia secuencial de los LEDs cada 150ms */
        case MODO_SECUENCIA_150MS:
            if (delayRead(&delayModo1)) {
                apagarTodosLosLeds();
                switch (pasoSecuencia) {
                    case 0:
                        writePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
                        pasoSecuencia = 1;
                        break;
                    case 1:
                        writePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                        pasoSecuencia = 2;
                        break;
                    case 2:
                        writePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                        pasoSecuencia = 0;
                        break;
                }
            }
            break;

        /* MODO 2: Parpadeo simultáneo de los 3 LEDs a 600ms */
        case MODO_TODOS_600MS:
            if (delayRead(&delayModo2)) {
                API_GPIO_ToggleAllLeds();
            }
            break;

        /* MODO 3: Tiempos independientes para cada LED (100ms, 300ms, 600ms) */
        case MODO_INDEPENDIENTE:
            if (delayRead(&delayLed1)) {
                HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
            }
            if (delayRead(&delayLed2)) {
                HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            }
            if (delayRead(&delayLed3)) {
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            }
            break;

        /* MODO 4: LED 1 y 3 en fase, LED 2 en contrafase a 150ms */
        case MODO_INVERSO_150MS:
            if (delayRead(&delayModo4)) {
                estadoInverso = !estadoInverso;
                if (estadoInverso) {
                    writePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
                    writePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                    writePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
                } else {
                    writePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
                    writePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
                    writePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                }
            }
            break;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /* Botón con PULLDOWN para evitar entradas flotantes */
  GPIO_InitStruct.Pin = boton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(boton_GPIO_Port, &GPIO_InitStruct);

  /* LEDs */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

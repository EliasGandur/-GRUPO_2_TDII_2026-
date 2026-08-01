/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
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
    ESTADO_LED1_ON,  // LED1 encendido
    ESTADO_LED2_ON,  // LED2 encendido
    ESTADO_LED3_ON   // LED3 encendido
} estadoSecuencia_t;

typedef enum {
    DIR_FORWARD = 0,
    DIR_BACKWARD
} SequenceDirection_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
delay_t pasoDelay;
estadoSecuencia_t estadoActual = ESTADO_LED1_ON;
SequenceDirection_t direction = DIR_FORWARD;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void checkButtonAndUpdateDirection(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void checkButtonAndUpdateDirection(void) {
    static uint8_t botonAnterior = API_GPIO_PIN_RESET;
    uint8_t botonActual = readPin(button_GPIO_Port, button_Pin);

    // Detectar flanco ascendente: estaba APAGADO y pasa a ENCENDIDO
    if (botonActual == API_GPIO_PIN_SET && botonAnterior == API_GPIO_PIN_RESET) {
        direction = (direction == DIR_FORWARD) ? DIR_BACKWARD : DIR_FORWARD;
    }

    botonAnterior = botonActual;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  delayInit(&pasoDelay, 200);

  // Estado inicial de las salidas
  writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_SET);
  writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_RESET);
  writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 1. Verificar el botón en cada ciclo del bucle
    checkButtonAndUpdateDirection();

    // 2. Un único delayRead para controlar el tiempo de avance
    if (delayRead(&pasoDelay))
    {
        if (direction == DIR_FORWARD)
        {
            /* Secuencia normal: LED1 -> LED2 -> LED3 */
            switch (estadoActual)
            {
                case ESTADO_LED1_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_RESET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_SET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_RESET);
                    estadoActual = ESTADO_LED2_ON;
                    break;

                case ESTADO_LED2_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_RESET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_RESET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_SET);
                    estadoActual = ESTADO_LED3_ON;
                    break;

                case ESTADO_LED3_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_SET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_RESET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_RESET);
                    estadoActual = ESTADO_LED1_ON;
                    break;
            }
        }
        else
        {
            /* Secuencia inversa: LED3 -> LED2 -> LED1 */
            switch (estadoActual)
            {
                case ESTADO_LED3_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_RESET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_SET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_RESET);
                    estadoActual = ESTADO_LED2_ON;
                    break;

                case ESTADO_LED2_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_SET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_RESET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_RESET);
                    estadoActual = ESTADO_LED1_ON;
                    break;

                case ESTADO_LED1_ON:
                    writePin(LD1_GPIO_Port, LD1_Pin, API_GPIO_PIN_RESET);
                    writePin(LD2_GPIO_Port, LD2_Pin, API_GPIO_PIN_RESET);
                    writePin(LD3_GPIO_Port, LD3_Pin, API_GPIO_PIN_SET);
                    estadoActual = ESTADO_LED3_ON;
                    break;
            }
        }
    }
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
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : button_Pin */
  GPIO_InitStruct.Pin = button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN; // Cambiado a PULLDOWN para evitar pin flotante
  HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

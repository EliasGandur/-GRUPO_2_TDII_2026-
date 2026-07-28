/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Secuencia de LEDs (1 -> 2 -> 3) usando un vector y la HAL
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// Estructura para agrupar el Puerto y el Pin de cada LED
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} Led_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_LEDS 3

/* Definiciones de los LEDs de la placa NUCLEO-F429ZI */
#define LED1_PORT           GPIOB
#define LED1_PIN            GPIO_PIN_0    // LED Verde

#define LED2_PORT           GPIOB
#define LED2_PIN            GPIO_PIN_7    // LED Azul

#define LED3_PORT           GPIOB
#define LED3_PIN            GPIO_PIN_14   // LED Rojo
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// Vector con las referencias a los 3 LEDs
static const Led_t leds[NUM_LEDS] = {
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN}
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void turnOffAllLEDs(void);

/* USER CODE BEGIN 0 */

/**
  * @brief Apaga todos los LEDs recorriendo el vector con la HAL
  */
void turnOffAllLEDs(void) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset de todos los periféricos, inicialización de Flash y Systick */
  HAL_Init();

  /* Configuración del reloj del sistema */
  SystemClock_Config();

  /* Inicialización de GPIOs */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  turnOffAllLEDs();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // Recorrido secuencial del vector (LED 1 -> LED 2 -> LED 3)
      for (uint8_t i = 0; i < NUM_LEDS; i++) {

          // 1. Apaga todos los LEDs antes de prender el siguiente
          turnOffAllLEDs();

          // 2. Enciende únicamente el LED activo
          HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_SET);

          // 3. Retardo de 200 ms
          HAL_Delay(200);
      }
  }
  /* USER CODE END WHILE */
}

/**
  * @brief Configuración del periférico GPIO
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Habilitar Clock del Puerto B */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Estado inicial: LEDs apagados */
  HAL_GPIO_WritePin(GPIOB, LED1_PIN | LED2_PIN | LED3_PIN, GPIO_PIN_RESET);

  /* Configuración de pines PB0, PB7 y PB14 como Salidas Push-Pull */
  GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

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
}
#endif

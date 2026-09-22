/*
 * API_GPIO.c
 *
 *  Created on: Jul 27, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */

#include "API_GPIO.h"
#include "main.h"
#include <stdbool.h>
uint8_t estado = 0;


//Desarrollo de las funciones//

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /* Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ------ AGREGAR DESDE AQUÍ ------ */
  /* Configure GPIO pin : button_Pin (B1 / PC13) */
  GPIO_InitStruct.Pin = button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL; // Las placas NUCLEO ya poseen Pull-Up física
  HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);
  /* -------------------------------- */
}

void Boton_GPIO(void) {
    static GPIO_PinState estado_anterior = GPIO_PIN_SET; // NUCLEO: en reposo lee SET (HIGH)
    static uint32_t ultima_marca_tiempo = 0;

    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

    // Detectar flanco de bajada (presión del botón hacia GND / RESET)
    if (estado_actual == GPIO_PIN_RESET && estado_anterior == GPIO_PIN_SET) {
        // Antirrebote no bloqueante de 50 ms
        if ((HAL_GetTick() - ultima_marca_tiempo) > 50) {

            estado = (estado + 1) % 4; // Rota cíclicamente: 0 -> 1 -> 2 -> 3 -> 0

            ultima_marca_tiempo = HAL_GetTick();
        }
    }

    estado_anterior = estado_actual;
}

void LEDencendido_GPIO(led_t LDx){
	HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_SET);
}

void LEDapagado_GPIO(led_t LDx){
	HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_RESET);
}

void toggleLed_GPIO(led_t LDx){
	HAL_GPIO_TogglePin(GPIOB, LDx);
}

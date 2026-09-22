/*
 * API_GPIO.c
 *
 *  Created on: Jul 27, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */

#include "API_GPIO.h"
#include "main.h"
#include <stdbool.h>
bool estado = false;


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

	static GPIO_PinState estado_anterior = GPIO_PIN_RESET;
	    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	    if (estado_actual == GPIO_PIN_SET && estado_anterior == GPIO_PIN_RESET) {
	        HAL_Delay(50); // Antirrebote //
	        if (HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin) == GPIO_PIN_SET) {
	            estado = !estado; // Invierte y conserva el valor //
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


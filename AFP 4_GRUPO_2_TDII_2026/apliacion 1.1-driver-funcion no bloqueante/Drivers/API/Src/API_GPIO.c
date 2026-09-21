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


//---------------Desarrollo-INICIALIZACIÓN----------------//


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
}

//---------------Desarrollo de las funciones----------------//

void LEDencendido_GPIO(led_t LDx){
	HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_SET);
}



void LEDapagado_GPIO(led_t LDx){
	HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_RESET);
}

void toggleLed_GPIO(led_t LDx){
	HAL_GPIO_TogglePin(GPIOB, LDx);
}



//---------------------------------------------------------//

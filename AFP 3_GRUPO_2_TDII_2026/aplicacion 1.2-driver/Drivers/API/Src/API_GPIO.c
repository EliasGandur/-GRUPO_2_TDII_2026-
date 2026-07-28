/*
 * API_GPIO.c
 *
 *  Created on: Jul 27, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
#include "API_GPIO.h"

void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t PinState) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (GPIO_PinState)PinState);
}

void togglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

GPIO_PinState_t readPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return (GPIO_PinState_t)HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}


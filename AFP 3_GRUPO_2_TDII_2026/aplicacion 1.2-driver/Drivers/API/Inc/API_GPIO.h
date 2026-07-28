/*
 * API_GPIO.h
 *
 *  Created on: Jul 27, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
#ifndef API_GPIO_H_
#define API_GPIO_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef enum {
    API_GPIO_PIN_RESET = 0,
    API_GPIO_PIN_SET
} GPIO_PinState_t;

/* Prototipos de la API */
void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t PinState);
void togglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
GPIO_PinState_t readPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* API_GPIO_H_ */

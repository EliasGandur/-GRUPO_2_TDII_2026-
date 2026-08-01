/*
 * API_GPIO.h
 *
 *  Created on: Jul 26, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_
#include <stdbool.h>
#include "main.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_led_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_button_t;

typedef enum {
    API_GPIO_PIN_RESET = 0,
    API_GPIO_PIN_SET
} GPIO_PinState_t;
void MX_GPIO_Init_Custom(void);
void API_GPIO_Init(void);
void API_GPIO_ToggleAllLeds(void);
bool API_GPIO_IsButtonPressed(void);
void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t PinState);
void API_GPIO_ParpadeoConDelay(void);
void API_GPIO_ParpadeoInverso(void);

#endif /* API_INC_API_GPIO_H_ */


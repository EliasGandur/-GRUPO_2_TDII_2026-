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

void MX_GPIO_Init_Custom(void);
void API_GPIO_Init(void);
void API_GPIO_ToggleAllLeds(void);
bool API_GPIO_IsButtonPressed(void);


#endif /* API_INC_API_GPIO_H_ */


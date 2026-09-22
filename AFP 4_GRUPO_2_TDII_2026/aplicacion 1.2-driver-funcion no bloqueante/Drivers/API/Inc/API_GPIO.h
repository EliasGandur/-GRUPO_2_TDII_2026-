/*
 * API_GPIO.h
 *
 *  Created on: Jul 27, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */

#ifndef API_GPIO_H_
#define API_GPIO_H_

//------Bibliotecas-----//

#include "stm32f4xx_hal.h"
#include <stdbool.h>

//---------------------//

typedef uint16_t led_t;//Enteros de 16 bits//
extern bool estado;

//------INICIALIZAMOS-----//

void MX_GPIO_Init(void);

//-----------------------//

//declaramos las funciones a utilizar//

void LEDencendido_GPIO(led_t LDx);
void LEDapagado_GPIO(led_t LDx);
void toggleLed_GPIO(led_t LDx);
void Boton_GPIO(void);

//----------------------------------//

#endif /* API_GPIO_H_ */

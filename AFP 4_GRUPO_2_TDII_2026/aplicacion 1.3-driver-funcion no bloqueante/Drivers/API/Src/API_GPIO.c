/*
 * API_GPIO.c
 *
 *  Created on: Jul 26, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
 //-----------Includes---------------- //
#include "main.h"
#include "API_GPIO.h"
#include "API_Delay.h"

//-------------Declaración de variables---------//

//---------------------------------------------//

static const gpio_led_t leds[3] = {
    { .port = LD1_GPIO_Port, .pin = LD1_Pin },
    { .port = LD2_GPIO_Port, .pin = LD2_Pin },
    { .port = LD3_GPIO_Port, .pin = LD3_Pin }
};

static const gpio_button_t button = {
    .port = boton_GPIO_Port,
    .pin = boton_Pin
};
//-------------Desarrollo de Funciones-----------//

void API_GPIO_Init(void) {
    for (int i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }
}

void API_GPIO_ToggleAllLeds(void) {
    for (int i = 0; i < 3; i++) {
        HAL_GPIO_TogglePin(leds[i].port, leds[i].pin);
    }
}

void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t PinState) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (GPIO_PinState)PinState);
}


void API_GPIO_ParpadeoConDelay(void){
	        HAL_GPIO_TogglePin(leds[0].port, leds[0].pin);
	        HAL_Delay(100);
	        HAL_GPIO_TogglePin(leds[1].port, leds[1].pin);
	        HAL_Delay(300);
	        HAL_GPIO_TogglePin(leds[2].port, leds[2].pin);
	        HAL_Delay(600);
}
void API_GPIO_ParpadeoInverso(void){
	    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	    HAL_Delay(150);
	    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	    HAL_Delay(150);
}

bool API_GPIO_IsButtonPressed(void) {
    static bool prev_state = false;


    bool current_state = (HAL_GPIO_ReadPin(button.port, button.pin) == GPIO_PIN_SET);

    // Detectar flanco ascendente (transición de no presionado a presionado)
    if (current_state && !prev_state) {
        HAL_Delay(50); // Antirrebote básico
        prev_state = true;
        return true;
    } else if (!current_state) {
        prev_state = false;
    }

    return false;
}
//----------------------Fin----------------//

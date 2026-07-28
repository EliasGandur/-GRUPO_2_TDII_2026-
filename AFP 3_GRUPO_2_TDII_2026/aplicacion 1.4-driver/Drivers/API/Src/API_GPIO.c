/*
 * API_GPIO.c
 *
 *  Created on: Jul 26, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
 //-----------Includes---------------- //
#include "main.h"
#include "API_GPIO.h"

//-------------Declaración de variables---------//


//---------------------------------------------//

static const gpio_led_t leds[3] = {
    { .port = LD1_GPIO_Port, .pin = LD1_Pin },
    { .port = LD2_GPIO_Port, .pin = LD2_Pin },
    { .port = LD3_GPIO_Port, .pin = LD3_Pin }
};

static const gpio_button_t button = {
    .port = button_GPIO_Port,
    .pin = button_Pin
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

/*
 * API_Debounce.c
 *
 *  Created on: oct 1, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
#include "API_debounce.h"
#include "API_GPIO.h"
#include "main.h"

// -----------------------Definiciones Privadas-------------------------------//

#define DEBOUNCE_TIME_MS 40
#define LED1 LD1_Pin //led verde
#define LED2 LD2_Pin //led azul
#define LED3 LD3_Pin // led rojo

// ----------------------Variables Estáticas del Driver-----------------------//

static debounceState_t currentState;

static uint32_t startTimeMs;

static bool isPressed = false;

// ---------------------------------------------------------------------------//

int estado = 0;

// -----------------Funciones Estáticas Auxiliares (Internas al Driver)-------//


static bool checkDebounceTime() {
    // El desbordamiento se maneja automáticamente en aritmética de enteros sin signo
    // (siempre que el período no sea mayor a la mitad del rango de la variable).
    return (getCurrentMs() - startTimeMs >= DEBOUNCE_TIME_MS);
}

// -----------------------------------------------------------------------------//

// -----------Implementación de las Funciones de la API Pública---------------//

void debounceFSM_init() {

    // 1. Carga el estado inicial de la MEF.
    currentState = BUTTON_UP;

    // 2. Carga el tiempo de inicio para el delay.
    startTimeMs = 0;

    // 3. Carga el estado de la variable de lectura del pulsador en no activo.
    isPressed = false;
}

//-------------Actualiza el estado de la MEF de anti-rebote.------------//

void debounceFSM_update() {
    // 1. Leer las entradas (estado crudo del pin)
    bool currentInput = readKey();

    // 2. Resolver la lógica de transición de estados y actualizar las salidas.
    switch (currentState) {
        case BUTTON_UP:
            // Estado estable: Botón Liberado
            if (currentInput == true) {
                // Se detecta presión (posible rebote)
                currentState = BUTTON_FALLING;
                startTimeMs = getCurrentMs(); // Cargar el tiempo de inicio
            }
            break;

        case BUTTON_FALLING:
            // Rebote en curso: Esperando 40 ms para confirmar presión
            if (currentInput == false) {
                // Se libera antes de los 40 ms (falso positivo/rebote)
                currentState = BUTTON_UP;
            } else if (checkDebounceTime()) {
                // Botón presionado por 40 ms (Confirmado)
                currentState = BUTTON_DOWN;
                isPressed = true;
                // 3. Actualizar salidas (Función de Acción)
                buttonPressed(); // Ejecuta la acción del usuario para la pulsación (ej. invertir LED1)
            }
            break;

        case BUTTON_DOWN:
            // Estado estable: Botón Presionado
            if (currentInput == false) {
                // Se detecta liberación (posible rebote)
                currentState = BUTTON_RISING;
                startTimeMs = getCurrentMs(); // Cargar el tiempo de inicio
            }
            break;

        case BUTTON_RISING:
            // Rebote en curso: Esperando 40 ms para confirmar liberación
            if (currentInput == true) {
                // Se vuelve a presionar antes de los 40 ms (falso positivo/rebote)
                currentState = BUTTON_DOWN;
            } else if (checkDebounceTime()) {
                // Botón liberado por 40 ms (Confirmado)
                currentState = BUTTON_UP;
                isPressed = false;
                // 3. Actualizar salidas (Función de Acción)
                buttonReleased(); // Ejecuta la acción del usuario para la liberación (ej. invertir LED3)
            }
            break;

        default:
            // Estado no válido: Reiniciar MEF
            debounceFSM_init();
            break;
    }
}

void buttonPressed(void) {
	estado = (estado + 1) % 4;

}

void buttonReleased(void) {

}

bool readKey(void) {
    return (readButton_GPIO() == false);
}

uint32_t getCurrentMs(void) {
    return HAL_GetTick();
}


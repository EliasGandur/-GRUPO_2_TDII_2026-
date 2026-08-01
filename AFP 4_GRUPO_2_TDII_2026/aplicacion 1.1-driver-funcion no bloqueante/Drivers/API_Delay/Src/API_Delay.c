/*
 * API_Delay.c
 *
 *  Created on: Jul 31, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */
#include "API_Delay.h"
#include "stm32f4xx_hal.h"

void delayInit(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;

    delay->duration = duration;  // Carga la duración requerida //
    delay->running = false;      // No inicia el conteo todavía //
}

bool_t delayRead(delay_t * delay) {
    if (delay == NULL) return false;

    // Si el retardo no está corriendo, toma la marca de tiempo actual y lo activa //
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    }
    // Si ya está corriendo, verifica si transcurrió el tiempo configurado //
    else {
        if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
            delay->running = false; // Restablece el flag al cumplir el plazo
            return true;            // Retorna 'true' indicando que se cumplió el tiempo
        }
        return false;
    }
}

void delayWrite(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;

    delay->duration = duration;  // Actualiza la duración del tiempo de retardo //
}



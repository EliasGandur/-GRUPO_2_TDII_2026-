/*
 * API_Delay.h
 *
 *  Created on: Jul 31, 2026
 *      Author: Gandur Elias , Gandur Solana , Gandur Juan Ignacio.
 */

#ifndef API_DELAY_INC_API_DELAY_H_
#define API_DELAY_INC_API_DELAY_H_

#include <stdint.h>  //BIBLIOTECAS requeridas para los tipos de variables//
#include <stdbool.h> //BIBLIOTECAS requeridas para los tipos de variables//


//------------------variables------------------//

typedef uint32_t tick_t;
typedef bool bool_t;

//--------------------------------------------//


//-----------Estructura de control para cada retardo-----------//


typedef struct {
    tick_t startTime;  // Marca de tiempo de inicio //
    tick_t duration;   // Duración del retardo en ms //
    bool_t running;    // Estado del retardo (activo / inactivo) //
} delay_t;

//------------------------------------------------------------//


//------------------Declaración de funciones---------------------//

void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);

//---------------------------------------------------------------//

#endif /* API_DELAY_INC_API_DELAY_H_ */

/**
 * @file    Conecta3.h
 * @brief   Cabeceras del motor lógico del juego Conecta 3.
 * @details Define los estados del tablero y expone la API pública para
 * la interacción entre el hardware (teclado/matriz) y el juego.
 */

#ifndef CONECTA3_H_
#define CONECTA3_H_

#include <stdint.h>

/* Definiciones de los estados de cada celda del tablero */
#define VACIO      0
#define JUGADOR_1  1  // Fichas representadas con color Rojo
#define JUGADOR_2  2  // Fichas representadas con color Verde

/* API Pública del Juego */
void Juego_Init(void);
uint8_t Juego_ObtenerTurno(void);
void Juego_Jugar(uint8_t columna);
void Juego_Actualizar(void);

#endif /* CONECTA3_H_ */

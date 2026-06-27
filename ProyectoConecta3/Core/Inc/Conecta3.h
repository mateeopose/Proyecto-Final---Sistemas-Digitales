/**
 * @file    Conecta3.h
 * @brief   Cabeceras del motor lógico y máquina de estados de Conecta 3.
 * @details Define las estructuras de estado, modos de juego y la API pública
 * para la gestión de la lógica de control.
 */

#ifndef CONECTA3_H_
#define CONECTA3_H_

#include <stdint.h>

/* Definiciones de celdas del tablero virtual */
#define VACIO      0
#define JUGADOR_1  1  /**< Identificador para el Jugador 1 (Fichas Rojas) */
#define JUGADOR_2  2  /**< Identificador para el Jugador 2 o la Máquina (Fichas Verdes) */

/* Enumeración formal de los estados principales del sistema (FSM) */
typedef enum {
    ESTADO_MENU = 0,  /**< Espera de selección de modo de juego */
    ESTADO_JUEGO,    /**< Partida en curso */
    ESTADO_VICTORIA   /**< Partida finalizada con un ganador */
} EstadoJuego_t;

/* Enumeración de los modos de juego soportados */
typedef enum {
    MODO_PVP = 0,   /**< Jugador contra Jugador (Local) */
    MODO_PVM          /**< Jugador contra Máquina (IA) */
} ModoJuego_t;

/* Público para el flujo principal del programa */
void Juego_Init(void);
void Juego_ProcesarEntrada(uint8_t tecla);
void Juego_Actualizar(void);

#endif /* CONECTA3_H_ */

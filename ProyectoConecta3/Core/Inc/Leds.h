#ifndef LEDS_H
#define LEDS_H

#include "main.h"
#include <stdint.h>

#define MATRIZ_FILAS 8
#define MATRIZ_COLS  4
#define NUM_LEDS     32

// Prototipos
void Matriz_Init(void);
void Matriz_SetLed(uint8_t fila, uint8_t columna, uint8_t red, uint8_t green, uint8_t blue);
void Matriz_ApagarTodo(void);
void Matriz_Update(void);

#endif /* LEDS_H */

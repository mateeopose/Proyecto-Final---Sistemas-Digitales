#ifndef TECLADO_H
#define TECLADO_H

#include "main.h" // Para importar las definiciones de pines y HAL
#include <stdint.h>

// Prototipos de funciones
void Teclado_Init(void);
void Teclado_Update(void);
void Teclado_EXTI_Callback(uint16_t GPIO_Pin);

// Función para obtener la última tecla leída (opcional, según cómo manejes la lógica)
uint8_t Teclado_GetUltimaTecla(void);

#endif /* TECLADO_H */

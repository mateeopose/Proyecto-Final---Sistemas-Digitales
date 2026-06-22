#include "teclado.h"

// --- DEFINICIONES DE ESTADOS ---
typedef enum {
    ESTADO_REPOSO,
    ESTADO_ANTIRREBOTE_PRESION,
    ESTADO_BARRIDO,
    ESTADO_ESPERA_SOLTAR,
    ESTADO_ANTIRREBOTE_SOLTAR
} Teclado_Estado_t;

// --- VARIABLES PRIVADAS ---
static Teclado_Estado_t estado_actual = ESTADO_REPOSO;
static uint32_t tiempo_referencia = 0;
static const uint32_t TIEMPO_ANTIRREBOTE = 20; // 20 milisegundos

static char ultima_tecla = '\0';
static uint8_t fila_activa = 0;
static uint16_t col_activa_pin = 0;
static GPIO_TypeDef* col_activa_port = NULL;

// Mapa físico del teclado matricial 4x4
static const uint8_t mapa_teclas[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

// Arreglos utilizando  etiquetas del CubeMX
static GPIO_TypeDef* puertos_filas[4] = {R1_TECLADO_GPIO_Port, R2_TECLADO_GPIO_Port, R3_TECLADO_GPIO_Port, R4_TECLADO_GPIO_Port};
static uint16_t pines_filas[4]       = {R1_TECLADO_Pin, R2_TECLADO_Pin, R3_TECLADO_Pin, R4_TECLADO_Pin};

static GPIO_TypeDef* puertos_cols[4]  = {C1_COLUMNA_GPIO_Port, C2_COLUMNA_GPIO_Port, C3_COLUMNA_GPIO_Port, C4_COLUMNA_GPIO_Port};
static uint16_t pines_cols[4]        = {C1_COLUMNA_Pin, C2_COLUMNA_Pin, C3_COLUMNA_Pin, C4_COLUMNA_Pin};

// --- FUNCIONES ---

void Teclado_Init(void) {
    // Opción B: Todas las filas (R) en 0 (GND) para esperar la interrupción
    for(int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(puertos_filas[i], pines_filas[i], GPIO_PIN_RESET);
    }
    estado_actual = ESTADO_REPOSO;
}

// Se llama desde HAL_GPIO_EXTI_Callback en main.c
void Teclado_EXTI_Callback(uint16_t GPIO_Pin) {
    if(estado_actual == ESTADO_REPOSO) {

    	// Nos aseguramos de que el pin que interrumpió sea una de las Columnas (C)
    	        if(GPIO_Pin == C1_COLUMNA_Pin || GPIO_Pin == C2_COLUMNA_Pin ||
    	           GPIO_Pin == C3_COLUMNA_Pin || GPIO_Pin == C4_COLUMNA_Pin) {

            tiempo_referencia = HAL_GetTick();
            estado_actual = ESTADO_ANTIRREBOTE_PRESION;
        }
    }
}

// Máquina de estados principal. Va dentro del while(1) en main.c
void Teclado_Update(void) {
    switch(estado_actual) {
        case ESTADO_REPOSO:
            break;

        case ESTADO_ANTIRREBOTE_PRESION:
            if((HAL_GetTick() - tiempo_referencia) >= TIEMPO_ANTIRREBOTE) {
                estado_actual = ESTADO_BARRIDO;
            }
            break;

        case ESTADO_BARRIDO: {
            uint8_t tecla_detectada = 0;

            // 1. Ponemos todas las filas (R) en 1 (Alta Impedancia / Open Drain)
            for(int i = 0; i < 4; i++) {
                HAL_GPIO_WritePin(puertos_filas[i], pines_filas[i], GPIO_PIN_SET);
            }

            // 2. Desplazamos un 0 fila por fila
            for(int f = 0; f < 4; f++) {
                HAL_GPIO_WritePin(puertos_filas[f], pines_filas[f], GPIO_PIN_RESET);

                // Micro-retardo para estabilización eléctrica
                for(volatile int nop = 0; nop < 50; nop++) { __NOP(); }

                // Leemos las columnas (C)
                for(int c = 0; c < 4; c++) {
                    if(HAL_GPIO_ReadPin(puertos_cols[c], pines_cols[c]) == GPIO_PIN_RESET) {
                        tecla_detectada = mapa_teclas[f][c];
                        fila_activa = f;
                        col_activa_pin = pines_cols[c];
                        col_activa_port = puertos_cols[c];
                        break;
                    }
                }

                if(tecla_detectada != 0) break;

                // Devolvemos la fila (R) a 1
                HAL_GPIO_WritePin(puertos_filas[f], pines_filas[f], GPIO_PIN_SET);
            }

            if(tecla_detectada != 0) {
                ultima_tecla = tecla_detectada;

                // LÓGICA DE JUEGO (Ej: procesar el movimiento)


                estado_actual = ESTADO_ESPERA_SOLTAR;
            } else {
                Teclado_Init(); // Falso contacto
            }
            break;
        }

        case ESTADO_ESPERA_SOLTAR:
            // Chequeamos que la columna (C) haya vuelto a 1 (Pull-Up) -> se soltó la tecla
            if(HAL_GPIO_ReadPin(col_activa_port, col_activa_pin) == GPIO_PIN_SET) {
                tiempo_referencia = HAL_GetTick();
                estado_actual = ESTADO_ANTIRREBOTE_SOLTAR;
            }
            break;

        case ESTADO_ANTIRREBOTE_SOLTAR:
            if((HAL_GetTick() - tiempo_referencia) >= TIEMPO_ANTIRREBOTE) {
                if(HAL_GPIO_ReadPin(col_activa_port, col_activa_pin) == GPIO_PIN_SET) {
                    // Limpiamos banderas EXTI pendientes por si rebotó al soltar
                	           __HAL_GPIO_EXTI_CLEAR_IT(C1_COLUMNA_Pin | C2_COLUMNA_Pin | C3_COLUMNA_Pin | C4_COLUMNA_Pin);
                    Teclado_Init(); // Volvemos todas las filas a 0 y estado a REPOSO
                } else {
                    estado_actual = ESTADO_ESPERA_SOLTAR;
                }
            }
            break;
    }
}

uint8_t Teclado_GetUltimaTecla(void) {
    uint8_t temp = ultima_tecla;
    ultima_tecla = 0;
    return temp;
}

#include "Leds.h"

// Variable externa de Timer
extern TIM_HandleTypeDef htim4;
// 1 significa "Libre", 0 significa "Transmitiendo"
volatile uint8_t dma_listo = 1;
// Almacenamiento para cada LED [R, G, B]
static uint8_t LED_Data[NUM_LEDS][3];

// Arreglo PWM que el DMA mandará al Timer
// 24 pulsos por LED + 50 pulsos en 0 para hacer el "Reset"
#define RESET_PULSES 1
static uint16_t pwmData[(24 * NUM_LEDS) + RESET_PULSES];

// Valores del ciclo de trabajo PWM (Basados en un ARR de 15)
// Un tercio y dos tercios del ARR
#define PWM_CERO 16  // ~32%
#define PWM_UNO  32  // ~64%

// Inicializa apagando todo
void Matriz_Init(void) {
    Matriz_ApagarTodo();
    //Matriz_Update();
}

// Función matemática para convertir Fila (0-7) y Columna (0-3) al número de LED en la tira (0-31)
// Traduce la coordenada lógica del juego al LED físico real de la tira
static uint8_t MapearCoordenada(uint8_t fila, uint8_t columna) {

    // Matriz visual del tablero
    // Fila 0 es arriba, Fila 7 es abajo
    // Columna 0 es la izquierda, Columna 3 es la derecha
    static const uint8_t mapa_leds[8][4] = {
        { 31,  27,  23,  19 },  // Fila 0 (Topee del tablero)
        { 30,  26,  22,  18 },  // Fila 1
        { 29,  25,  21,  17 },  // Fila 2
        { 28,  24,  20,  16 },  // Fila 3
        { 15,  11,   7,   3 },  // Fila 4
        { 14,  10,   6,   2 },  // Fila 5
        { 13,   9,   5,   1 },  // Fila 6
        { 12,   8,   4,   0 }   // Fila 7 (Fondo del tablero)
    };

    return mapa_leds[fila][columna];
}
// Guarda un color RGB en la memoria de un LED específico
void Matriz_SetLed(uint8_t fila, uint8_t columna, uint8_t red, uint8_t green, uint8_t blue) {
    if(fila >= MATRIZ_FILAS || columna >= MATRIZ_COLS) return; // Protección para tomar solo los datos que corresponden

    uint8_t id = MapearCoordenada(fila, columna);

    LED_Data[id][0] = red;
    LED_Data[id][1] = green;
    LED_Data[id][2] = blue;
}

void Matriz_ApagarTodo(void) {
    for(int i = 0; i < NUM_LEDS; i++) {
        LED_Data[i][0] = 0;
        LED_Data[i][1] = 0;
        LED_Data[i][2] = 0;
    }
}

// El "motor" de la matriz. Convierte los colores a tiempos PWM y dispara el DMA
void Matriz_Update(void) {
	// Si el DMA todavía está mandando datos, salimos y no hacemos nada
	// En lugar de un "if" con un "return", usamos un "while" vacío.
	    // Esto hace que el micro se quede dando vueltas acá unos milisegundos
	    // hasta que el DMA termine y la bandera vuelva a ser 1.
	    if (dma_listo == 0) {
	        return;
	    }

	    // Bajamos bandera: El DMA ahora está OCUPADO
	    dma_listo = 0;
	    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    uint32_t indx = 0;
    uint32_t color24;

    for (int i = 0; i < NUM_LEDS; i++) {
        // WS2812 espera los colores en formato GRB (Verde, Rojo, Azul)
        color24 = ((LED_Data[i][1] << 16) | (LED_Data[i][0] << 8) | (LED_Data[i][2]));

        // Lee cada uno de los 24 bits (del más significativo al menos)
        for (int b = 23; b >= 0; b--) {
            if (color24 & (1 << b)) {
                pwmData[indx] = PWM_UNO;  // Bit en 1
            } else {
                pwmData[indx] = PWM_CERO; // Bit en 0
            }
            indx++;
        }
    }

    // ceros al final para la señal de Reset/Latch
    for (int i = 0; i < RESET_PULSES; i++) {
        pwmData[indx] = 0;
        indx++;
    }

    // Se dispara la transferencia DMA apuntando al registro CCR1 del Timer 4
    HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
}


// Hay que detener el DMA cuando termina, si no se puede trabar para la siguiente llamada
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM4) {
        HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_1);
        TIM4->CCR1 = 0;
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

        // Se levanta la bandera para permitir una nueva transmisión
        dma_listo = 1;
    }
}
// Esta función atrapa el error si el DMA colapsa y libera el programa, muchos problemas sin esto
void HAL_TIM_PWM_ErrorCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM4) {
        // Se apaga el DMA para que no siga haciendo lio
        HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_1);

        // Se libera la bandera para que el teclado siga funcionando
        dma_listo = 1;
    }
}

/**
 * @file    Conecta3.c
 * @brief   Implementación de la máquina de estados, modo versus máquina e indicadores de UI.
 * @details Centraliza las reglas de dominio del juego, el procesamiento de periféricos de entrada
 * y el refresco periódico del frame buffer visual.
 */

#include "Conecta3.h"
#include "Leds.h"
#include "stm32f4xx_hal.h"
#define VELOCIDAD_CAIDA_MS 100

/* Variables de control y contexto de la FSM */
static EstadoJuego_t estado_sistema = ESTADO_MENU;
static ModoJuego_t modo_seleccionado = MODO_PVP;
static uint8_t tablero[8][4];
static uint8_t turno_actual = JUGADOR_1;

/* Contexto del subsistema de animación de caída */
static uint8_t animacion_activa = 0;
static uint8_t anim_fila_actual = 0;
static uint8_t anim_fila_destino = 0;
static uint8_t render_necesario = 1; // Bandera de estado sucio
static uint8_t anim_columna = 0;
static uint8_t anim_jugador = 0;
static uint32_t anim_tiempo_anterior = 0;

/* Temporizador no bloqueante para la toma de decisiones de la máquina */
static uint32_t ia_tiempo_anterior = 0;
static uint8_t ia_esperando_movimiento = 0;

/* Declaración de funciones estáticas privadas */
static uint8_t VerificarGanador(void);
static void IniciarAnimacionCaida(uint8_t columna, uint8_t jugador);
static void EjecutarJugadaMaquina(void);
static void RenderizarTablero(void);

/**
 * @brief  Inyecta y procesa los eventos del teclado matricial según el estado actual de la FSM.
 * @param  tecla Código numérico de la tecla presionada (1 a 16).
 */
void Juego_ProcesarEntrada(uint8_t tecla) {
    /* Estado de Menú: Selección de modo mediante la fila inferior del teclado */
    if (estado_sistema == ESTADO_MENU) {
        if (tecla == 13 || tecla == 14) {
            modo_seleccionado = MODO_PVP;
            estado_sistema = ESTADO_JUEGO;
            render_necesario = 1;
        }
        else if (tecla == 15 || tecla == 16) {
            modo_seleccionado = MODO_PVM;
            estado_sistema = ESTADO_JUEGO;
            render_necesario = 1;
        }
        return;
    }

    /* Estado de Juego: Procesamiento de comandos de entrada de columnas */
    if (estado_sistema == ESTADO_JUEGO) {
        /* Tecla de Reinicio (Reset) asignada al botón 5 */
        if (tecla == 5) {
            Juego_Init();
            return;
        }

        /* Si es el turno de la máquina en modo PvM, se ignoran las entradas del usuario */
        if (modo_seleccionado == MODO_PVM && turno_actual == JUGADOR_2) {
            return;
        }

        /* Conversión de rango de entrada (Teclas 1-4) a índices de columnas lógicas (0-3) */
        if (tecla >= 1 && tecla <= 4 && !animacion_activa) {
            IniciarAnimacionCaida(tecla - 1, turno_actual);
        }
    }

    /* Estado de Victoria: Únicamente se permite la salida mediante la tecla de Reset */
    if (estado_sistema == ESTADO_VICTORIA && tecla == 5) {
        Juego_Init();
    }
}

/**
 * @brief Inicializa o reinicia el estado lógico del sistema.
 * @details Restablece las variables de control al estado de menú original,
 * aborta cualquier hilo de animación activo y limpia el contenido
 * de la matriz de celdas lógicas (tablero virtual).
 */
void Juego_Init(void) {
    estado_sistema = ESTADO_MENU;
    turno_actual = JUGADOR_1;
    animacion_activa = 0;
    ia_esperando_movimiento = 0;
    render_necesario = 1;

    /* Limpieza de la estructura de datos del tablero virtual */
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 4; col++) {
            tablero[fila][col] = VACIO;
        }
    }

    /* El vaciado físico de los diodos LED se delega al planificador periódico */
}

/**
 * @brief Planificador periódico no bloqueante encargado del refresco gráfico y la lógica temporal.
 * @note  Debe ser llamada de forma continua en el bucle principal (super-loop).
 */
void Juego_Actualizar(void) {

    /* 1. ESTADO MENÚ */
    if (estado_sistema == ESTADO_MENU) {
        if (render_necesario == 1) {
            for (int f = 0; f < 8; f++) {
                for (int c = 0; c < 4; c++) {
                    Matriz_SetLed(f, c, 0, 0, 0);
                }
            }
            Matriz_Update();
            render_necesario = 0;
        }
        return;
    }

    /* 2. TURNO DE LA MÁQUINA */
    if (estado_sistema == ESTADO_JUEGO && modo_seleccionado == MODO_PVM &&
        turno_actual == JUGADOR_2 && !animacion_activa) {

        if (!ia_esperando_movimiento) {
            ia_tiempo_anterior = HAL_GetTick();
            ia_esperando_movimiento = 1;
        }

        if (HAL_GetTick() - ia_tiempo_anterior >= 600) {
            ia_esperando_movimiento = 0;
            EjecutarJugadaMaquina();
        }
    }

    /* 3. ESTADO ANIMACIÓN ACTIVA */
    if (animacion_activa && (HAL_GetTick() - anim_tiempo_anterior >= VELOCIDAD_CAIDA_MS)) {
        anim_tiempo_anterior = HAL_GetTick();

        // Evaluamos primero si la ficha YA LLEGÓ a su destino
        if (anim_fila_actual == anim_fila_destino) {
            // Se consolida en el tablero virtual
            tablero[anim_fila_destino][anim_columna] = anim_jugador;
            animacion_activa = 0; // Apagamos el motor de caída

            // Verificamos si la jugada resultó en victoria
            if (VerificarGanador() != 0) {
                estado_sistema = ESTADO_VICTORIA;
            } else {
                // Alternamos el turno
                turno_actual = (turno_actual == JUGADOR_1) ? JUGADOR_2 : JUGADOR_1;
            }

            // ¡ATENCIÓN ACÁ! No mandamos nada al DMA todavía.
            // Solo levantamos la bandera para que el bloque de abajo dibuje
            // el tablero nuevo Y el LED 19 actualizado AL MISMO TIEMPO.
            render_necesario = 1;
        }
        else {
            // Todavía está cayendo. Dibujamos el fotograma de transición.
            RenderizarTablero();

            if (anim_jugador == JUGADOR_1) {
                Matriz_SetLed(anim_fila_actual, anim_columna, 100, 0, 0);
            } else {
                Matriz_SetLed(anim_fila_actual, anim_columna, 0, 100, 0);
            }

            Matriz_Update(); // Enviamos la caída al DMA

            anim_fila_actual++; // Preparamos la fila para el próximo frame
        }
    }
    /* 4. ESTADO JUEGO PASIVO (Tablero estático) */
    else if (!animacion_activa) {
        if (render_necesario == 1) {
            RenderizarTablero();
            Matriz_Update();
            render_necesario = 0; // Bajamos la bandera
        }
    }
}
/**
 * @brief  Calcula la física de la columna e inicializa los parámetros de la animación.
 * @param  columna Índice horizontal de destino (0-3).
 * @param  jugador Entidad que realiza el movimiento.
 */
static void IniciarAnimacionCaida(uint8_t columna, uint8_t jugador) {
    int8_t destino = -1;

    /* Escaneo vertical invertido omitiendo la fila de menú (Fila 0) */
    for (int8_t fila = 7; fila >= 1; fila--) {
        if (tablero[fila][columna] == VACIO) {
            destino = fila;
            break;
        }
    }

    /* Si se determinó una coordenada de reposo válida, se concede el inicio del renderizado */
    if (destino != -1) {
        anim_fila_destino = destino;
        anim_columna = columna;
        anim_jugador = jugador;
        anim_fila_actual = 1; /* Comienzo de la trayectoria por debajo del menú */
        anim_tiempo_anterior = HAL_GetTick();
        animacion_activa = 1;
    }
}

/**
 * @brief Algoritmo predictivo básico para la selección de columna de la máquina.
 */
static void EjecutarJugadaMaquina(void) {
    /* Selección pseudoaleatoria basada en el registro de tiempo del sistema */
    uint8_t col_elegida = HAL_GetTick() % 4;

    /* Bucle de mitigación por desbordamiento en caso de que la columna seleccionada esté llena */
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t col_evaluar = (col_elegida + i) % 4;
        if (tablero[1][col_evaluar] == VACIO) {
            IniciarAnimacionCaida(col_evaluar, JUGADOR_2);
            return;
        }
    }
}

/**
 * @brief Procesa el volcado estático de la memoria de celdas hacia el buffer de hardware.
 */
static void RenderizarTablero(void) {
    /* 1. Volcado estático del tablero lúdico (Filas 1 a 7) */
    for (int f = 1; f < 8; f++) {
        for (int c = 0; c < 4; c++) {
            if (tablero[f][c] == JUGADOR_1) {
                Matriz_SetLed(f, c, 100, 0, 0);
            } else if (tablero[f][c] == JUGADOR_2) {
                Matriz_SetLed(f, c, 0, 100, 0);
            } else {
                Matriz_SetLed(f, c, 0, 0, 0);
            }
        }
    }

    /* 2. Actualización de la barra de estado superior (Fila 0) */
    for (int c = 0; c < 4; c++) {
        Matriz_SetLed(0, c, 0, 0, 0);
    }

    /* Mapeo Lógico-Físico: (Fila 0, Columna 3) se traduce al LED físico 19 para indicar el turno */
    if (turno_actual == JUGADOR_1) {
        Matriz_SetLed(0, 3, 100, 0, 0); /* Turno entrante: Rojo */
    } else {
        Matriz_SetLed(0, 3, 0, 100, 0); /* Turno entrante: Verde */
    }
}

/**
 * @brief  Escanea el tablero en busca de patrones de tres celdas adyacentes homólogas.
 * @retval ID del jugador ganador, o 0 si no se cumple ninguna condición de cierre.
 */
static uint8_t VerificarGanador(void) {
    /* Barrido Vertical */
    for (int col = 0; col < 4; col++) {
        for (int fila = 1; fila <= 5; fila++) {
            uint8_t ficha = tablero[fila][col];
            if (ficha != VACIO && ficha == tablero[fila + 1][col] && ficha == tablero[fila + 2][col]) {
                return ficha;
            }
        }
    }
    /* Barrido Horizontal */
    for (int fila = 1; fila < 8; fila++) {
        for (int col = 0; col <= 1; col++) {
            uint8_t ficha = tablero[fila][col];
            if (ficha != VACIO && ficha == tablero[fila][col + 1] && ficha == tablero[fila][col + 2]) {
                return ficha;
            }
        }
    }
    /* Barrido Diagonal Descendente (\) */
    for (int fila = 1; fila <= 5; fila++) {
        for (int col = 0; col <= 1; col++) {
            uint8_t ficha = tablero[fila][col];
            if (ficha != VACIO && ficha == tablero[fila + 1][col + 1] && ficha == tablero[fila + 2][col + 2]) {
                return ficha;
            }
        }
    }
    /* Barrido Diagonal Ascendente (/) */
    for (int fila = 1; fila <= 5; fila++) {
        for (int col = 2; col < 4; col++) {
            uint8_t ficha = tablero[fila][col];
            if (ficha != VACIO && ficha == tablero[fila + 1][col - 1] && ficha == tablero[fila + 2][col - 2]) {
                return ficha;
            }
        }
    }
    return 0;
}

 Conecta Tres — STM32F407G

Implementación embebida del juego **Conecta Tres** sobre la placa **STM32F407G-DISC1**, desarrollada como proyecto final de la materia **Sistemas Digitales** (Ingeniería Electrónica — UNMDP, 2026).

El sistema implementa una versión electrónica del clásico Connect Four reducida a 3 en línea, con visualización en matriz de LEDs RGB, control por teclado matricial y retroalimentación sonora por buzzer.

---

 Hardware utilizado

| Componente | Descripción |
|---|---|
| **STM32F407G-DISC1** | Placa de desarrollo con microcontrolador STM32F407VGT6 (ARM Cortex-M4, 168 MHz) |
| **Matriz LEDs WS2812B** | 32 LEDs RGB direccionables (4 columnas × 8 filas), comunicación por un solo conductor |
| **Teclado matricial 4×4** | Interfaz de entrada del jugador con antirrebote por software |
| **Buzzer pasivo** | Retroalimentación sonora, frecuencia de resonancia ~2 kHz |
| **Conversor de nivel lógico** | Adaptación 3.3V ↔ 5V entre la placa STM32 y los LEDs WS2812B |

---

 Periféricos STM32 utilizados

- **GPIO** — lectura del teclado matricial y control general
- **Timers + PWM** — generación de tonos para el buzzer pasivo
- **Timers + DMA** — control de la señal de datos para los LEDs WS2812B
- **EXTI (interrupciones externas)** — detección de teclas presionadas
- **SysTick / Timers** — gestión de animaciones y tiempos de juego

---

 Funcionalidades implementadas

- **Tablero 4×8** con mecánica de gravedad por columnas (las fichas caen a la posición libre más baja)
- **Modo 2 jugadores** con alternancia de turnos
- **Modo 1 jugador vs CPU** con selección de nivel de dificultad
- **Detección de victoria** en línea horizontal, vertical y diagonal (3 en raya)
- **Detección de empate** cuando el tablero se completa sin ganador
- **Animación de caída** de fichas sobre la matriz de LEDs
- **Indicación visual** de victoria y fin de partida
- **Menú principal** con selección de modo de juego
- **Señalización sonora** para jugada válida, jugada inválida, cambio de turno, victoria y empate
- **Máquina de estados finita (FSM)** que gestiona toda la lógica del juego

---

 Estructura del proyecto

```
ProyectoConecta3/
├── Core/
│   ├── Inc/
│   │   ├── Buzzer.h        # Interfaz del módulo de señalización sonora
│   │   ├── Conecta3.h      # Lógica del juego y máquina de estados
│   │   ├── Leds.h          # Control de la matriz WS2812B
│   │   ├── Teclado.h       # Lectura del teclado matricial
│   │   └── main.h
│   └── Src/
│       ├── Buzzer.c        # Generación de tonos por PWM
│       ├── Conecta3.c      # FSM del juego, detección de victoria/empate
│       ├── Leds.c          # Driver WS2812B vía DMA
│       ├── Teclado.c       # Barrido del teclado, antirrebote, interrupciones
│       └── main.c
```

El proyecto sigue un diseño modular: cada subsistema (LEDs, teclado, buzzer, lógica del juego) está implementado en su propio par `.h`/`.c`, facilitando la integración, las pruebas independientes y el mantenimiento del código.

 Equipo

Mateo Pose y Cristian Septien 
Ingeniería Electrónica — Facultad de Ingeniería, UNMDP  
Materia: Sistemas Digitales — Ciclo 2026, 1° cuatrimestre
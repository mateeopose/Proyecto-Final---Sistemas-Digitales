Conecta Tres — STM32F407G

An embedded implementation of the **Connect Three** game on the **STM32F407G-DISC1** development board, built as the final project for the **Digital Systems** course (Electronic Engineering — UNMDP, 2026).

The system implements an electronic version of the classic Connect Four, reduced to 3 in a row, with RGB LED matrix display, matrix keypad input, and buzzer audio feedback.

---

Hardware

| Component | Description |
|---|---|
| **STM32F407G-DISC1** | Development board with STM32F407VGT6 microcontroller (ARM Cortex-M4, 168 MHz) |
| **WS2812B LED Matrix** | 32 addressable RGB LEDs (4 columns × 8 rows), single-wire communication |
| **4×4 Matrix Keypad** | Player input interface with software debouncing |
| **Passive Buzzer** | Audio feedback, resonance frequency ~2 kHz |
| **Logic Level Converter** | 3.3V ↔ 5V bidirectional adapter between STM32 and WS2812B LEDs |

---

STM32 Peripherals Used

- **GPIO** — keypad scanning and general control
- **Timers + PWM** — tone generation for the passive buzzer
- **Timers + DMA** — WS2812B data signal generation
- **EXTI (external interrupts)** — keypress detection
- **SysTick / Timers** — animation timing and game event management

---

Features

- **4×8 board** with column gravity mechanics (pieces fall to the lowest available position)
- **2-player mode** with turn alternation
- **1-player vs CPU mode** with difficulty levels
- **Win detection** in horizontal, vertical and diagonal directions (3 in a row)
- **Draw detection** when the board is full with no winner
- **Piece drop animation** on the LED matrix
- **Visual win/end-of-game indication**
- **Main menu** with game mode selection
- **Audio feedback** for valid move, invalid move, turn change, win and draw
- **Finite State Machine (FSM)** managing all game logic

---

Project Structure

```
ProyectoConecta3/
├── Core/
│   ├── Inc/
│   │   ├── Buzzer.h        # Audio feedback module interface
│   │   ├── Conecta3.h      # Game logic and FSM
│   │   ├── Leds.h          # WS2812B matrix driver
│   │   ├── Teclado.h       # Matrix keypad reader
│   │   └── main.h
│   └── Src/
│       ├── Buzzer.c        # PWM-based tone generation
│       ├── Conecta3.c      # FSM, win/draw detection
│       ├── Leds.c          # WS2812B DMA driver
│       ├── Teclado.c       # Keypad scanning, debounce, interrupts
│       └── main.c
```

The project follows a **fully modular design**: each subsystem (LEDs, keypad, buzzer, game logic) is implemented in its own `.h`/`.c` pair, enabling independent testing and easy integration.

---

Build & Flash

**Requirements:**
- STM32CubeIDE
- STM32F407G-DISC1 board connected via USB

**Steps:**
1. Clone the repository:
   ```bash
   git clone https://github.com/mateeopose/Proyecto-Final---Sistemas-Digitales.git
   ```
2. Open STM32CubeIDE and import the project: `File → Import → Existing Projects into Workspace`
3. Select the cloned directory and choose `ProyectoConecta3`
4. Build with `Project → Build Project`
5. Flash with `Run → Debug` or `Run → Run`

---

Team

**Mateo Pose** and **Cristian Septien**  
Electronic Engineering — Faculty of Engineering, UNMDP  
Course: Digital Systems — 2026, 1st semester

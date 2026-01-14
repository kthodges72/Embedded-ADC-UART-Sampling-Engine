# Embedded ADC–UART Audio Visualization System

This repository contains my personal STM32 embedded systems project.

### Project Overview
The final project samples an analog audio signal using ADC, triggered by a software timer interrupt at a set sample rate of 20kHz. The samples are processed in real time using an envelope smoothing filter, implementing a low-pass filter algorithm on the individual samples. The display is updated by another software timer interrupt at a set rate of 30Hz. Each update, the current processed sample value is taken, and compared to output an ASCII bar 'interval level.' This ASCII bar is pushed into a 256 byte circular buffer, which is being continuously drained and outputted through UART over DMA. On the host side (e.g., PuTTY), this will appear as a smooth ASCII volume bar, refreshing at 30Hz and reflecting the volume level near the microphone of my microphone-amp circuit.



https://github.com/user-attachments/assets/a94b6217-c14e-4d83-930d-37559b328074



### Project goal
I built the project strictly using STM32’s LL (low-level) driver library, with the goal of interacting as closely with the hardware as possible. By removing the layers of abstraction provided by libraries such as STM32’s HAL or Arduino’s libraries, I was able to gain a deeper understanding of the STM32’s architecture. As a result, I developed stronger knowledge of computer architecture and gained practical experience writing low-level drivers, directly supporting my interest in a future career in firmware or systems software engineering.

### Microphone-Amplifier Circuit
This circuit utilizes a simple electret microphone and the TI-LM358 integrated circuit to provide the STM32's ADC1 peripheral with an amplified AC signal.
![IMG_6451](https://github.com/user-attachments/assets/59a61215-80dc-403d-a182-4b7dd931ddad)
![IMG_6453](https://github.com/user-attachments/assets/ab354cad-037d-42f7-9358-fac500c82f3d)


### Project Structure 
This repository follows the standard STM32CubeIDE project layout.  
- `Core/` contains all user-modifiable source files, including the `Inc/` and `Src/` folders where the custom UART and circular buffer modules live.
- `Drivers/` holds the auto-generated STM32 HAL/LL and CMSIS libraries.  
- `Debug/` holds binary artifacts and build outputs placed there by STM32CubeIDE.
- The root folder contains build configurations, linker scripts, IDE metadata (`.project`, `.cproject`, `.mxproject`), and the CubeMX configuration file (`adc_project.ioc`).  



# Core/Inc (Header Files)

This folder contains the public interfaces for modules included in the project.

- **circbuf.h**  
  - Header for the circular buffer module.  
  - Defines the `CircBuf` struct, buffer size, and public API (`circbuf_init`, `circbuf_write_byte`, `circbuf_peek_contiguous`, etc.).

- **uart.h**  
  - Header for the USART2 DMA TX driver.  
  - Defines the UART handle structure, initialization routines, DMA transmit API, and the DMA IRQ callback.

- **adc.h**   
  - Header for the adc module
  - Defines the ADC handle structure and ADC API
  
- **timer.h**
  - Header for the timer module
  - Defines the timer handle structure and interrupt functions

- **display.h**
  - Header for the display module
  - Defines the display handle structure and functions

- **main.h**
  - Header for main.c
  - Global definitions or handles shared between modules

- **stm32f4xx_it.h**, **system_stm32f4xx.h**, etc.
  - Provide MCU configuration
  - Contain interrupt definitions
  - Contain system initialization prototypes

---

# Core/Src (Source Files)

This folder contains firmware logic and low-level drivers.

### Modules

- **circbuf.c**  
  Implements a 256-byte circular buffer for transmitting outgoing data.  
  Features:
  - contiguous block discovery for DMA,
  - wrap-aware pointer movement,
  - efficient writes,
  - overwrite-on-full behavior for real-time streaming.

  This module acts as the data staging layer between ADC sampling (future) and UART transmission.

- **uart.c**  
  Implements a non-blocking USART2 DMA transmit engine using LL drivers.  
  Features:
  - DMA1 Stream 6 configuration for TX,
  - interrupt-driven transfer completion,
  - safe enqueueing of outgoing bytes,
  - support for formatted printing and raw byte streaming,
  - seamless integration with `circbuf`.

  This module forms the backbone of the outbound data pipeline.

- **adc.c**
  Implements ADC conversion and interrupt handling using LL drivers.
  Features:
  - ADC end of completion flag for interrupt handling,
  - sample reading, handling and storage.

  This module handles the ADC sampling for processing and output

- **timer.c**
  Implements two hardware-driven timers for ADC sampling and display updating.
  Features:
  - timer initalization for TIM2 and TIM3, setting prescaler and autoreload values,
  - interrupt handling, setting software flags for ADC sampling and display updating.

  This module controls the timing of adc and display modules
  
- **display.c**
  Implements envelope-smoothed volume bar display
  Features:
  - smoothing envelope follower for smooth human-readable volume bar
  - volume bar level determined by processed samples

- **main.c**  
  Integrates and initializes the modules.  
  Currently:
  - initializes USART and DMA,
  - initializes the circular buffer,
  - contains testing output for validating the transmit pipeline.
  
  ### CubeMX-Generated Source Files

- **stm32f4xx_it.c**  
  Contains interrupt service routines, including the DMA1 Stream 6 IRQ used by the UART module.

- **stm32f4xx_hal_msp.c**, **system_stm32f4xx.c**  
  MCU clock configuration, peripheral setup, and low-level HAL/LL infrastructure.

- **syscalls.c**, **sysmem.c**  
  Newlib stubs for memory allocation and system calls.

- **adc.c**, **timer.c**  
  Not yet implemented.  
  These will be populated with sampling and timer logic.
  
 ### Key Technical Highlights
- Interrupt-driven ADC sampling at 20 kHz
- Real-time envelope follower implemented via IIR low-pass filtering
- Double-timer architecture separating signal acquisition and UI refresh
- Zero-copy UART DMA transmission using a circular buffer staging layer
- Entire project implemented using STM32 LL drivers (no HAL abstractions)
 

---




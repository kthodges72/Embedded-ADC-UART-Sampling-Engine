# Embedded ADC–UART Sampling Engine (Work in Progress)

This repository contains my personal STM32 embedded systems project.

The project is a work in progress, and this README will continue to grow as additional modules (ADC, timer) are implemented. 

At the current stage, the project structure is complete, and two core modules are implemented: the UART driver (DMA-based USART2 TX) and the circular buffer module.

### Project goal
The final application will continuously sample an analog audio signal using the ADC (driven by a timer interrupt), run the samples through a low-pass filter accumulator, push each sample into a circular buffer, and stream the processed values over UART using DMA. On the host side (e.g., PuTTY), this will appear as a scrolling readout of real-time amplitude or decibel-equivalent measurements printed line by line. Once the core engine is complete, future improvements could include adding an RMS-based loudness calculation or implementing a live ASCII “volume bar” for terminal visualization. The audio signal is going to come from a an external microphone passed through a simple op-amp conditioning circuit that adds a DC bias and provides the appropriate gain for clean sampling on the STM32.



### Project Structure Overview
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
  - Not implemented yet.
  
- **timer.h**
  - Header for the timer module
  - Not implemented yet

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

### Completed Modules

- **circbuf.c**  
  Implements a 256-byte circular buffer for transmitting outgoing data.  
  Supports:
  - contiguous block discovery for DMA,
  - wrap-aware pointer movement,
  - efficient ISR-friendly writes,
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

---

# Modules Implemented (2)

## 1. Circular Buffer (`circbuf`)

This module provides a fixed-size (256-byte) FIFO ring buffer used to store outgoing data before DMA transmission. It provides:

- `circbuf_write_byte` for writing to buffer. 
- `circbuf_peek_contiguous` to expose a DMA-ready memory region  
- wrap-aware index advancement  
- overwrite-on-full behavior to prevent data stalls  
- fast full/empty checks and byte-accurate count queries

It is used as the intermediate staging area for streaming ADC samples and UART transmission.

---

## 2. UART DMA Transmission (`uart`)

This module implements a non-blocking USART2 transmit driver using DMA1 Stream 6. It includes:

- LL-based USART2 configuration  
- DMA TX setup and memory-to-peripheral configuration  
- a custom `UART_Handle_t` for driver state  
- an interrupt-driven DMA completion handler  
- `uart_send_dma` and `uart_DMA_printf` for pushing data into the TX pipeline  
- integration with the circular buffer for continuous streaming

This module is fully operational and verified.

---

# Modules To Be Completed

These modules have no logic yet:

## 3. ADC Sampling Module (Planned)
- Configure ADC1 using LL drivers  
- Set up timer trigger for stable sampling frequency  
- Implement conversion complete callback  
- Push 16-bit samples into the circular buffer as two bytes  

## 4. Timer Module (Planned)
- Configure TIMx for periodic triggers  
- Drive ADC sampling  
- Provide timing hooks for future tasks

These modules will be documented as they are implemented.

---

# Current Status Summary

At this stage of development:

1. The file structure is organized.  
2. The UART DMA-TX module is complete and tested.  
3. The circular buffer module is complete and tested.  
4. `main.c` integrates both modules and supports basic output.

This README will be updated as progress continues.

# STM32F407ZGT6 Pin Allocation & Hardware Interface Map

## 1. Overview
The primary system microcontroller is an **STM32F407ZGT6** manufactured by STMicroelectronics. It features an ARM 32-bit Cortex-M4 core with hardware Floating Point Unit (FPU), running at a maximum frequency of 168 MHz with 1 MB of on-chip Flash memory and 192 KB of SRAM in an LQFP-144 package.

---

## 2. Silicon & Package Architecture (LQFP-144)

### 2.1 Package Overview
- **Device**: STM32F407ZGT6
- **Package**: LQFP-144 (20.0 mm × 20.0 mm × 1.4 mm, 0.5 mm pitch, 144 pins)
- **Core**: ARM 32-bit Cortex-M4 with FPU and Adaptive Real-Time (ART) Accelerator
- **Operating Voltage**: 1.8 V to 3.6 V (Nominal system rail: 3.3 V)
- **Operating Frequency**: Up to 168 MHz (HSE external crystal + Main PLL)

### 2.2 Dedicated System & Control Pins (Confirmed)
- **NRST (Pin 25)**: Active-low asynchronous hardware reset. Filtered with 100 nF MLCC to GND.
- **BOOT0 (Pin 138)**: Boot mode selection pin. Pulled down to GND via 10 kΩ resistor for normal execution from main User Flash memory.
- **PA13 / JTMS / SWDIO (Pin 105)**: Serial Wire Debug Data I/O. Dedicated hardware SWD debug port.
- **PA14 / JTCK / SWCLK (Pin 109)**: Serial Wire Debug Clock. Dedicated hardware SWD debug clock.
- **PH0 / OSC_IN (Pin 23)**: High-Speed External (HSE) crystal oscillator input (8 MHz or 25 MHz reference).
- **PH1 / OSC_OUT (Pin 24)**: High-Speed External (HSE) crystal oscillator output.
- **VDD / VSS (Multiple Pins)**: Decoupled with 100 nF ceramic capacitors adjacent to each pin pair.
- **VDDA / VSSA (Pins 33/32)**: Analog supply and ground rails filtered with ferrite bead and decoupling caps for low ADC noise.

---

## 3. Peripheral Interface Mapping & Engineering Status

> [!NOTE]
> While the physical MCU package and part marking are authoritatively confirmed as **STM32F407ZGT6 (LQFP-144)**, specific GPIO pin breakout routes on the multi-layer PCB represent **Candidate / Pending Physical Trace Verification** allocations. In accordance with strict engineering standards, certainty is not fabricated where direct trace continuity measurements are pending.

| Subsystem | Signal Name | STM32 Alternate Function | Candidate Pin (LQFP-144) | Electrical Characteristic | Status |
|:---|:---|:---|:---|:---|:---|
| **Debug / ST-LINK** | `SWDIO` | SWD Data | PA13 (Pin 105) | 3.3V Logic, Pull-up | Confirmed |
| **Debug / ST-LINK** | `SWCLK` | SWD Clock | PA14 (Pin 109) | 3.3V Logic, Pull-down | Confirmed |
| **System Reset** | `NRST` | Master Reset | NRST (Pin 25) | Active-Low Filtered | Confirmed |
| **Boot Mode** | `BOOT0` | Boot Strapping | BOOT0 (Pin 138) | Pull-Down to GND | Confirmed |
| **Sensor I2C Bus** | `I2C_SCL` | I2C1_SCL | PB6 / PB8 | 3.3V Open-Drain, 4.7k Pull-up | Candidate / Pending Verification |
| **Sensor I2C Bus** | `I2C_SDA` | I2C1_SDA | PB7 / PB9 | 3.3V Open-Drain, 4.7k Pull-up | Candidate / Pending Verification |
| **Particulate Sensor** | `PM_UART_TX` | USART2_RX | PA3 / PD6 | 3.3V UART In (9600 Baud) | Candidate / Pending Verification |
| **Particulate Sensor** | `PM_UART_RX` | USART2_TX | PA2 / PD5 | 3.3V UART Out (9600 Baud) | Candidate / Pending Verification |
| **Particulate Sensor** | `PM_SET` | GPIO Output | PC4 / PD3 | 3.3V Push-Pull Active-High | Candidate / Pending Verification |
| **Particulate Sensor** | `PM_RESET` | GPIO Output | PC5 / PD4 | 3.3V Push-Pull Active-Low | Candidate / Pending Verification |
| **Display SPI Bus** | `DISP_SCK` | SPI1_SCK / SPI2_SCK | PA5 / PB13 | 3.3V SPI Clock (Up to 40 MHz) | Candidate / Pending Verification |
| **Display SPI Bus** | `DISP_MOSI`| SPI1_MOSI / SPI2_MOSI| PA7 / PB15 | 3.3V SPI Data Out | Candidate / Pending Verification |
| **Display Control** | `DISP_CS` | GPIO Output | PA4 / PB12 | 3.3V Active-Low Chip Select | Candidate / Pending Verification |
| **Display Control** | `DISP_DC` | GPIO Output | PC1 / PE2 | 3.3V Data/Command Select | Candidate / Pending Verification |
| **Display Control** | `DISP_RST` | GPIO Output | PC2 / PE3 | 3.3V Active-Low Reset | Candidate / Pending Verification |
| **Display Backlight** | `DISP_BL` | TIMx_CHx PWM | PB0 / PB1 / PA8 | 3.3V Timer PWM (5 kHz) | Candidate / Pending Verification |
| **Capacitive Touch** | `TOUCH_INT` | EXTI Line | PC0 / PE4 | 3.3V Edge-Triggered Interrupt | Candidate / Pending Verification |
| **Capacitive Touch** | `TOUCH_RST` | GPIO Output | PC3 / PE5 | 3.3V Active-Low Reset | Candidate / Pending Verification |
| **Power Telemetry** | `BAT_SENSE`| ADC1_INx | PA0 / PA1 (ADC1) | 0.0V–3.3V Resistor Divider | Candidate / Pending Verification |
| **Power Telemetry** | `CHG_STAT` | GPIO Input | PB10 / PE6 | Active-Low Charger Status | Candidate / Pending Verification |
| **User Pushbutton** | `PWR_KEY` | EXTI Line | PA0 (WKUP) / PE0 | Active-Low Button / Wakeup | Candidate / Pending Verification |

---

## 4. Bus Allocations & Contention Safeguards
1. **I2C Sensor Bus Addressing**:
   - `0x44`: Sensirion SHT41 (Temperature & Humidity)
   - `0x62`: Sensirion SCD41 (Carbon Dioxide)
   - `0x15`: CST816S Capacitive Touch Controller
   All sensor slave addresses are mutually exclusive, operating at Fast-Mode (400 kHz).
2. **SPI Display Bus**:
   - High-throughput display rendering over dedicated SPI peripheral with DMA stream transfer.

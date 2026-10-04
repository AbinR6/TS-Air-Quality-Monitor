# Hardware Subsystem Specification

## 1. Overview
The Air Monitor hardware comprises a 64 mm × 64 mm 4-layer printed circuit board integrating an STMicroelectronics STM32F407ZGT6, high-efficiency power conversion, laser particulate sensing, optical carbon dioxide sensing, precision temperature/humidity sensing, an SPI color IPS display, and a capacitive touch slider.

---

## 2. Microcontroller & Memories
- **MCU**: STMicroelectronics STM32F407ZGT6.
- **Core**: ARM® 32-bit Cortex®-M4 CPU with FPU running at 168 MHz (210 DMIPS).
- **Embedded Flash**: 1024 KB (1 MB) Flash memory.
- **Embedded SRAM**: 192 KB total system SRAM (128 KB general SRAM + 64 KB CCM data RAM).
- **Package**: LQFP-144 (20 mm × 20 mm, 0.5 mm pitch).
- **Clocking**: 8 MHz / 25 MHz High-Speed External (HSE) crystal oscillator driving internal PLL to 168 MHz core clock.

---

## 3. Power Architecture
- **Input**: USB Type-C 5.0 V nominal (4.5 V to 5.5 V).
- **Battery**: Single-cell 18650 Lithium-Ion cylindrical cell (3.7 V nominal, 2500 mAh).
- **Linear Charger (U3)**: Candidate MCP73831 / BQ24040 programmed to 500 mA charging current with thermal foldback and status output.
- **System Buck Converter (U2)**: Candidate TPS62088 synchronous step-down converter providing 3.30 V regulated rail up to 1.5 A continuous, coupled with 2.2 µH shielded inductor (marked `2R2` in E001) and MLCC filtering.

---

## 4. Sensor Transducers
- **PM Sensor (M1)**: Candidate Plantower PMS5003-compatible optical laser dust sensor communicating over UART (9600 baud) with dedicated active-high sleep control (`PM_SET`) and reset (`PM_RESET`).
- **CO₂ Sensor (M2)**: Candidate Sensirion SCD41 photoacoustic NDIR sensor on shared 400 kHz I²C bus (`0x62`).
- **Climate Sensor (M3)**: Candidate Sensirion SHT41 precision temperature and humidity sensor on I²C bus (`0x44`), isolated on a slotted PCB corner.

---

## 5. User Interface & Display
- **Display**: 2.1-inch color IPS TFT LCD (240 × 320 resolution) powered by Sitronix ST7789V driver, connected via 31-pin 0.5 mm pitch bottom-contact FPC connector (`J1`).
- **Backlight**: Low-side N-channel MOSFET switch driven by STM32 Timer PWM output at 5 kHz for flicker-free brightness dimming.
- **Capacitive Touch**: `DANY_TOUCH` top slider bar connected via 6-pin 0.5 mm pitch FPC connector (`J2`), supporting tap, swipe, and long-press gestures.

---

## 6. KiCad Design Files
The complete schematic, PCB layout, symbol library, and manufacturing footprints are located in [`02_Hardware/kicad/`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/02_Hardware/kicad/).
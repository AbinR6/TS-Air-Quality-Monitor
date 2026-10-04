#!/usr/bin/env python3
"""
Generate high-resolution professional figures for the Air Monitor PBL Project Report.
Outputs to: report_figures/
"""

import os
import shutil
from PIL import Image
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches

OUTPUT_DIR = "report_figures"
os.makedirs(OUTPUT_DIR, exist_ok=True)

# -------------------------------------------------------------
# 1. Process Hardware Inspection Photos
# -------------------------------------------------------------
photo_sources = [
    ("pictures/WhatsApp Image 2026-10-03 at 22.04.59.jpeg", "fig_photo_main_pcb.png"),
    ("pictures/WhatsApp Image 2026-10-03 at 22.04.59 (1).jpeg", "fig_photo_chassis_frame.png"),
    ("pictures/WhatsApp Image 2026-10-03 at 22.05.00.jpeg", "fig_photo_enclosure_interior.png"),
    ("pictures/WhatsApp Image 2026-10-03 at 22.05.00 (1).jpeg", "fig_photo_display_panel.png"),
]

for src, dst in photo_sources:
    if os.path.exists(src):
        img = Image.open(src)
        # Ensure RGB
        if img.mode != 'RGB':
            img = img.convert('RGB')
        dst_path = os.path.join(OUTPUT_DIR, dst)
        img.save(dst_path, format="PNG", quality=95)
        print(f"Processed: {dst}")

# -------------------------------------------------------------
# Helper: Matplotlib diagram setup
# -------------------------------------------------------------
plt.rcParams['font.sans-serif'] = 'Arial'
plt.rcParams['font.family'] = 'sans-serif'

# -------------------------------------------------------------
# 2. System Block Diagram
# -------------------------------------------------------------
def make_system_block_diagram():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')
    
    # Title banner
    ax.text(50, 66, "AIR MONITORING SYSTEM — HIGH-LEVEL BLOCK DIAGRAM", 
            ha='center', va='center', fontsize=13, fontweight='bold', color='#1A365D')
    
    # Domains:
    # Compute Core (Center)
    mcu_box = patches.FancyBboxPatch((36, 20), 28, 38, boxstyle="round,pad=1.5", 
                                     fc='#EBF8FF', ec='#2B6CB0', lw=2.5)
    ax.add_patch(mcu_box)
    ax.text(50, 54, "MAIN CONTROLLER\nSTM32F407ZGT6", ha='center', va='center', 
            fontsize=11, fontweight='bold', color='#2B6CB0')
    ax.text(50, 43, "• ARM Cortex-M4 @ 168 MHz\n• 1024 KB Embedded Flash\n• 192 KB System SRAM\n• Hardware FPU & DSP\n• LQFP-144 Package\n• Multi-Channel DMA", 
            ha='center', va='center', fontsize=8.5, color='#2D3748', linespacing=1.3)
    ax.text(50, 24, "FreeRTOS & STM32 HAL\nPreemptive Multi-Tasking", ha='center', va='center',
            fontsize=8, fontweight='bold', color='#4A5568')

    # Power Subsystem (Left Top)
    pwr_box = patches.FancyBboxPatch((4, 45), 24, 16, boxstyle="round,pad=1", 
                                     fc='#FEFCBF', ec='#D69E2E', lw=2)
    ax.add_patch(pwr_box)
    ax.text(16, 57, "POWER SUBSYSTEM", ha='center', va='center', fontsize=9.5, fontweight='bold', color='#B7791F')
    ax.text(16, 50, "• USB Type-C 5V Input\n• 18650 Li-Ion (3.7V / 2500mAh)\n• MCP73831 Linear Charger\n• TPS62088 Buck (3.3V / 1.5A)", 
            ha='center', va='center', fontsize=7.5, color='#744210', linespacing=1.2)

    # Power Arrow to MCU
    ax.annotate("", xy=(36, 51), xytext=(28, 51),
                arrowprops=dict(arrowstyle="->", color='#D69E2E', lw=2.5))
    ax.text(32, 53, "3.3V VDD", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#B7791F')

    # Environmental Sensors (Left Bottom)
    sens_box = patches.FancyBboxPatch((4, 8), 24, 30, boxstyle="round,pad=1", 
                                      fc='#F0FFF4', ec='#38A169', lw=2)
    ax.add_patch(sens_box)
    ax.text(16, 34, "SENSOR SUBSYSTEM", ha='center', va='center', fontsize=9.5, fontweight='bold', color='#2F855A')
    ax.text(16, 23, "• Plantower PMS5003 (PM)\n  Laser Scattering (PM1/2.5/10)\n• Sensirion SCD41 (CO₂)\n  Photoacoustic NDIR (400-5000ppm)\n• Sensirion SHT41 (Climate)\n  Precision Temp & Humidity", 
            ha='center', va='center', fontsize=7.5, color='#22543D', linespacing=1.3)

    # Sensor Interface Arrows to MCU
    ax.annotate("", xy=(36, 32), xytext=(28, 32),
                arrowprops=dict(arrowstyle="<->", color='#38A169', lw=2))
    ax.text(32, 33.5, "I2C1 Bus", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#2F855A')
    
    ax.annotate("", xy=(36, 18), xytext=(28, 18),
                arrowprops=dict(arrowstyle="<->", color='#38A169', lw=2))
    ax.text(32, 19.5, "USART2", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#2F855A')

    # User Interface (Right Top)
    ui_box = patches.FancyBboxPatch((72, 40), 24, 21, boxstyle="round,pad=1", 
                                    fc='#FAF5FF', ec='#805AD5', lw=2)
    ax.add_patch(ui_box)
    ax.text(84, 57, "USER INTERFACE (UI)", ha='center', va='center', fontsize=9.5, fontweight='bold', color='#6B46C1')
    ax.text(84, 48, "• 2.1\" IPS Color TFT LCD\n  240 × 320 Resolution (ST7789)\n• 31-Pin 0.5mm FPC Tail\n• PWM Backlight Dimming\n• DANY_TOUCH Capacitive Slider", 
            ha='center', va='center', fontsize=7.5, color='#44337A', linespacing=1.2)

    # UI Arrows from MCU
    ax.annotate("", xy=(72, 51), xytext=(64, 51),
                arrowprops=dict(arrowstyle="<->", color='#805AD5', lw=2))
    ax.text(68, 52.5, "SPI1 / DMA", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#6B46C1')

    ax.annotate("", xy=(72, 43), xytext=(64, 43),
                arrowprops=dict(arrowstyle="<->", color='#805AD5', lw=2))
    ax.text(68, 44.5, "Touch / PWM", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#6B46C1')

    # Telemetry & Storage (Right Bottom)
    tele_box = patches.FancyBboxPatch((72, 8), 24, 24, boxstyle="round,pad=1", 
                                      fc='#EDFDFD', ec='#319795', lw=2)
    ax.add_patch(tele_box)
    ax.text(84, 28, "STORAGE & TELEMETRY", ha='center', va='center', fontsize=9.5, fontweight='bold', color='#285E61')
    ax.text(84, 18, "• Flash NVS Key-Value Store\n• 256-Record Circular Ring Buffer\n• Telemetry Serializer (JSON)\n• External Communication Port\n  (Candidate USART / USB OTG)", 
            ha='center', va='center', fontsize=7.5, color='#234E52', linespacing=1.2)

    # Telemetry Arrow
    ax.annotate("", xy=(72, 20), xytext=(64, 20),
                arrowprops=dict(arrowstyle="<->", color='#319795', lw=2))
    ax.text(68, 21.5, "Async Comm", ha='center', va='bottom', fontsize=7.5, fontweight='bold', color='#285E61')

    # Footer note
    ax.text(50, 2, "Figure 3.1: Complete Architectural Block Diagram of the STM32F407ZGT6 Air Monitor", 
            ha='center', va='center', fontsize=9, fontstyle='italic', color='#4A5568')

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_system_block_diagram.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_system_block_diagram.png")

# -------------------------------------------------------------
# 3. Hardware Architecture & Bus Topology Diagram
# -------------------------------------------------------------
def make_hardware_architecture():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')
    
    ax.text(50, 67, "HARDWARE BUS ARCHITECTURE & PIN INTERFACE MAP", 
            ha='center', va='center', fontsize=12.5, fontweight='bold', color='#1A365D')

    # Central MCU block
    mcu = patches.Rectangle((35, 12), 30, 50, fc='#EBF8FF', ec='#2B6CB0', lw=2.5)
    ax.add_patch(mcu)
    ax.text(50, 58, "STM32F407ZGT6\nLQFP-144", ha='center', va='center', fontsize=11, fontweight='bold', color='#2B6CB0')
    ax.text(50, 52, "Internal 32-bit Multi-AHB Matrix", ha='center', va='center', fontsize=8, fontstyle='italic', color='#4A5568')

    # Buses shown inside MCU
    buses = [
        ("APB1 (42 MHz)", 43, "#BEE3F8"),
        ("APB2 (84 MHz)", 33, "#C3DAFE"),
        ("AHB1 (168 MHz)", 23, "#E9D8FD")
    ]
    for name, y, color in buses:
        rect = patches.Rectangle((37, y-3), 26, 6, fc=color, ec='#4A5568', lw=1, ls='--')
        ax.add_patch(rect)
        ax.text(50, y, name, ha='center', va='center', fontsize=8.5, fontweight='bold', color='#2D3748')

    # Peripheral Left: I2C1 and USART2
    # I2C1
    p1 = patches.Rectangle((5, 41), 22, 10, fc='#F0FFF4', ec='#38A169', lw=1.5)
    ax.add_patch(p1)
    ax.text(16, 47, "I2C1 BUS (400 kHz)", ha='center', va='center', fontsize=8.5, fontweight='bold', color='#22543D')
    ax.text(16, 43, "• PB6 (SCL)  • PB7 (SDA)\nSCD41 (0x62) & SHT41 (0x44)", ha='center', va='center', fontsize=7, color='#276749')
    ax.annotate("", xy=(35, 43), xytext=(27, 43), arrowprops=dict(arrowstyle="<->", color='#38A169', lw=2))

    # USART2
    p2 = patches.Rectangle((5, 23), 22, 10, fc='#F0FFF4', ec='#38A169', lw=1.5)
    ax.add_patch(p2)
    ax.text(16, 29, "USART2 (9600 Baud)", ha='center', va='center', fontsize=8.5, fontweight='bold', color='#22543D')
    ax.text(16, 25, "• PA2 (TX)  • PA3 (RX)\nPlantower PMS5003 Laser PM", ha='center', va='center', fontsize=7, color='#276749')
    ax.annotate("", xy=(35, 25), xytext=(27, 25), arrowprops=dict(arrowstyle="<->", color='#38A169', lw=2))

    # Peripheral Right: SPI1 and TIM/GPIO
    # SPI1
    p3 = patches.Rectangle((73, 41), 22, 10, fc='#FAF5FF', ec='#805AD5', lw=1.5)
    ax.add_patch(p3)
    ax.text(84, 47, "SPI1 BUS (40 MHz)", ha='center', va='center', fontsize=8.5, fontweight='bold', color='#553C9A')
    ax.text(84, 43, "• PA5 (SCK) • PA7 (MOSI)\n• PA4 (CS)  • PC4 (DC)\nST7789 IPS LCD (240x320)", ha='center', va='center', fontsize=7, color='#6B46C1')
    ax.annotate("", xy=(65, 43), xytext=(73, 43), arrowprops=dict(arrowstyle="<->", color='#805AD5', lw=2))

    # TIM & Touch
    p4 = patches.Rectangle((73, 23), 22, 10, fc='#FAF5FF', ec='#805AD5', lw=1.5)
    ax.add_patch(p4)
    ax.text(84, 29, "TIM PWM & EXTI", ha='center', va='center', fontsize=8.5, fontweight='bold', color='#553C9A')
    ax.text(84, 25, "• PB1: TIM3_CH4 (LEDK PWM)\n• PB5: EXTI (TOUCH_INT)\nDANY_TOUCH Capacitive Flex", ha='center', va='center', fontsize=7, color='#6B46C1')
    ax.annotate("", xy=(65, 25), xytext=(73, 25), arrowprops=dict(arrowstyle="<->", color='#805AD5', lw=2))

    # SWD & Power at bottom
    p5 = patches.Rectangle((20, 3), 60, 6, fc='#EDF2F7', ec='#718096', lw=1.5)
    ax.add_patch(p5)
    ax.text(50, 6, "DEBUG & SYSTEM POWER: PA13 (SWDIO), PA14 (SWCLK), NRST | VDD = 3.3V, VSS = GND", 
            ha='center', va='center', fontsize=8, fontweight='bold', color='#2D3748')

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_hardware_architecture.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_hardware_architecture.png")

# -------------------------------------------------------------
# 4. Firmware Architecture Diagram
# -------------------------------------------------------------
def make_firmware_architecture():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')

    ax.text(50, 66, "STM32F407 MODULAR FIRMWARE ARCHITECTURE", 
            ha='center', va='center', fontsize=13, fontweight='bold', color='#1A365D')

    layers = [
        ("APPLICATION & UI LAYER", 54, 8, "#EBF8FF", "#2B6CB0", 
         "Main Dashboard Carousel • PM / CO₂ / Climate Screens • Diagnostics • Alert Threshold Manager"),
        ("MEASUREMENT PROCESSING & ALGORITHM ENGINE", 43, 8, "#F0FFF4", "#38A169", 
         "Exponential Moving Average (EMA) Smoothing • US EPA AQI Piecewise Interpolator • Outlier Rejection"),
        ("SYSTEM SERVICES & MIDDLEWARE", 32, 8, "#FAF5FF", "#805AD5", 
         "FreeRTOS Preemptive Kernel • Non-Volatile Flash Key-Value Store • 256-Record Circular Buffer"),
        ("HARDWARE ABSTRACTION LAYER (HAL & DRIVERS)", 21, 8, "#FEFCBF", "#D69E2E", 
         "PMS5003 UART Driver • SCD41 I2C Driver • SHT41 I2C Driver • ST7789 SPI LCD Driver • CST816 Touch"),
        ("PHYSICAL HARDWARE TARGET", 10, 8, "#EDF2F7", "#4A5568", 
         "STM32F407ZGT6 (ARM Cortex-M4 @ 168 MHz) • 1 MB Flash • 192 KB SRAM • Timers • DMA • Peripherals")
    ]

    for title, y, h, fc, ec, desc in layers:
        rect = patches.FancyBboxPatch((8, y-h/2), 84, h, boxstyle="round,pad=1", fc=fc, ec=ec, lw=2)
        ax.add_patch(rect)
        ax.text(50, y+1.5, title, ha='center', va='center', fontsize=9.5, fontweight='bold', color=ec)
        ax.text(50, y-1.8, desc, ha='center', va='center', fontsize=8, color='#2D3748')

    # Arrows between layers
    for y_arrow in [48.5, 37.5, 26.5, 15.5]:
        ax.annotate("", xy=(50, y_arrow-1.5), xytext=(50, y_arrow+1.5),
                    arrowprops=dict(arrowstyle="<->", color='#4A5568', lw=2))

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_firmware_architecture.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_firmware_architecture.png")

# -------------------------------------------------------------
# 5. Measurement Pipeline & Dataflow Diagram
# -------------------------------------------------------------
def make_dataflow_pipeline():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')

    ax.text(50, 66, "END-TO-END MEASUREMENT PIPELINE & DATA-FLOW", 
            ha='center', va='center', fontsize=12.5, fontweight='bold', color='#1A365D')

    steps = [
        ("1. RAW SENSING", 12, 50, "#EBF8FF", "#2B6CB0", "Periodic Hardware Acquisition\n• PMS5003 UART (32-byte)\n• SCD41 I2C (Periodic)\n• SHT41 I2C (T/RH Single)"),
        ("2. VALIDATION", 37, 50, "#FEFCBF", "#D69E2E", "Integrity & Boundary Check\n• UART Checksum Verify\n• I2C CRC-8 Validation\n• Physical Plausibility Sanity"),
        ("3. FILTERING", 63, 50, "#F0FFF4", "#38A169", "Noise Suppression\n• Exponential Moving Avg\n  EMA = α·X_k + (1-α)·EMA_{k-1}\n• Transient Spike Rejection"),
        ("4. AQI ENGINE", 88, 50, "#FAF5FF", "#805AD5", "Piecewise AQI Calculation\n• US EPA PM2.5 / PM10\n• Linear Interpolation\n• Category Assessment")
    ]

    for name, x, y, fc, ec, desc in steps:
        box = patches.FancyBboxPatch((x-10.5, y-10), 21, 20, boxstyle="round,pad=0.8", fc=fc, ec=ec, lw=2)
        ax.add_patch(box)
        ax.text(x, y+6, name, ha='center', va='center', fontsize=9, fontweight='bold', color=ec)
        ax.text(x, y-2, desc, ha='center', va='center', fontsize=7.5, color='#2D3748', linespacing=1.2)

    # Horizontal arrows between steps 1-4
    for xa in [23, 48.5, 74]:
        ax.annotate("", xy=(xa+3.5, 50), xytext=(xa-1, 50),
                    arrowprops=dict(arrowstyle="->", color='#2B6CB0', lw=2))

    # Step 5 & 6 output branches at bottom
    # Arrow down from AQI
    ax.annotate("", xy=(88, 30), xytext=(88, 38), arrowprops=dict(arrowstyle="->", color='#805AD5', lw=2))
    
    # Split bar
    ax.plot([30, 88], [30, 30], color='#4A5568', lw=2)
    ax.annotate("", xy=(30, 24), xytext=(30, 30), arrowprops=dict(arrowstyle="->", color='#4A5568', lw=2))
    ax.annotate("", xy=(70, 24), xytext=(70, 30), arrowprops=dict(arrowstyle="->", color='#4A5568', lw=2))

    # Branch A: UI Display
    b_ui = patches.FancyBboxPatch((15, 8), 30, 15, boxstyle="round,pad=0.8", fc='#FAF5FF', ec='#805AD5', lw=2)
    ax.add_patch(b_ui)
    ax.text(30, 18, "5A. GRAPHICAL DISPLAY", ha='center', va='center', fontsize=9, fontweight='bold', color='#6B46C1')
    ax.text(30, 12, "• ST7789 2.1\" Color TFT LCD\n• Real-Time Gauge & AQI Badges\n• Dynamic 30 FPS Render Carousel", 
            ha='center', va='center', fontsize=7.5, color='#44337A', linespacing=1.2)

    # Branch B: Storage & Telemetry
    b_tel = patches.FancyBboxPatch((55, 8), 30, 15, boxstyle="round,pad=0.8", fc='#EDFDFD', ec='#319795', lw=2)
    ax.add_patch(b_tel)
    ax.text(70, 18, "5B. STORAGE & TELEMETRY", ha='center', va='center', fontsize=9, fontweight='bold', color='#285E61')
    ax.text(70, 12, "• JSON Serialization Engine\n• Flash NVS Non-Volatile Storage\n• 256-Record Circular Buffer Queue", 
            ha='center', va='center', fontsize=7.5, color='#234E52', linespacing=1.2)

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_dataflow_pipeline.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_dataflow_pipeline.png")

# -------------------------------------------------------------
# 6. PCB Layout & Physical Component Placement
# -------------------------------------------------------------
def make_pcb_layout_diagram():
    fig, ax = plt.subplots(figsize=(8, 8), dpi=300)
    ax.set_xlim(0, 70)
    ax.set_ylim(0, 70)
    ax.axis('off')

    ax.text(35, 67, "MAIN PCB PHYSICAL LAYOUT & COMPONENT TOPOLOGY", 
            ha='center', va='center', fontsize=11, fontweight='bold', color='#1A365D')

    # Main PCB substrate (64x64 mm with rounded top-right corner)
    pcb = patches.FancyBboxPatch((3, 3), 64, 60, boxstyle="round,pad=0.5,rounding_size=3", 
                                 fc='#1C4532', ec='#276749', lw=3)
    ax.add_patch(pcb)
    ax.text(35, 60, "DANY_JPU_MB_P1 (64 mm × 64 mm, 4-Layer FR4)", 
            ha='center', va='center', fontsize=8, color='#C6F6D5', fontweight='bold')

    # MCU in Center/Bottom
    mcu = patches.Rectangle((23, 14), 24, 24, fc='#2D3748', ec='#CBD5E0', lw=1.5)
    ax.add_patch(mcu)
    # Pins around MCU
    for offset in range(2, 23, 3):
        # Top/Bottom
        ax.plot([23+offset, 23+offset], [12, 14], color='#ECC94B', lw=1.5)
        ax.plot([23+offset, 23+offset], [38, 40], color='#ECC94B', lw=1.5)
        # Left/Right
        ax.plot([21, 23], [14+offset, 14+offset], color='#ECC94B', lw=1.5)
        ax.plot([47, 49], [14+offset, 14+offset], color='#ECC94B', lw=1.5)
    ax.text(35, 26, "U1: STM32F407ZGT6\nLQFP-144 (20×20 mm)\n168 MHz / 1MB Flash", 
            ha='center', va='center', fontsize=7.5, color='#F7FAFC', fontweight='bold', linespacing=1.2)

    # Power Section (Top Left)
    pwr = patches.Rectangle((7, 42), 16, 14, fc='#744210', ec='#D69E2E', lw=1)
    ax.add_patch(pwr)
    ax.text(15, 52, "POWER SECTION", ha='center', va='center', fontsize=7, fontweight='bold', color='#FEFCBF')
    ax.text(15, 46, "• U2: Buck (TPS62088)\n• 2R2 Inductor\n• U3: Li-Ion Charger", 
            ha='center', va='center', fontsize=6, color='#FEEBC8', linespacing=1.1)

    # 31-pin FPC Connector J1 (Center Left)
    j1 = patches.Rectangle((6, 22), 6, 16, fc='#4A5568', ec='#ECC94B', lw=1)
    ax.add_patch(j1)
    ax.text(9, 30, "J1\n31-Pin\nFPC", ha='center', va='center', fontsize=6, color='#FEFCBF', fontweight='bold')

    # 6-pin Touch FPC J2 (Center Right)
    j2 = patches.Rectangle((58, 26), 5, 12, fc='#4A5568', ec='#ECC94B', lw=1)
    ax.add_patch(j2)
    ax.text(60.5, 32, "J2\n6-Pin\nFPC", ha='center', va='center', fontsize=5.5, color='#FEFCBF', fontweight='bold')

    # Gasket Optical Sensor Area (Top Right)
    gasket = patches.Rectangle((48, 44), 14, 11, fc='#1A202C', ec='#E2E8F0', lw=1)
    ax.add_patch(gasket)
    ax.text(55, 49.5, "OPTICAL GASKET\nAmbient / IR", ha='center', va='center', 
            fontsize=6, color='#E2E8F0', fontweight='bold')

    # SHT41 Thermal Isolation Slot (Left Bottom)
    slot = patches.Rectangle((6, 8), 12, 10, fc='#0F291E', ec='#68D391', lw=1, ls='--')
    ax.add_patch(slot)
    ax.text(12, 13, "SHT41 T/RH\nThermal\nCutout", ha='center', va='center', 
            fontsize=5.5, color='#9AE6B4', fontweight='bold')

    # Mounting holes H1, H2, H3
    for hx, hy, hname in [(8, 59, "H1"), (35, 42, "H2"), (60, 8, "H3")]:
        c = patches.Circle((hx, hy), 1.8, fc='#A0AEC0', ec='#4A5568', lw=1)
        ax.add_patch(c)
        ax.text(hx, hy, hname, ha='center', va='center', fontsize=5, fontweight='bold', color='#1A202C')

    # Test Pads cluster at bottom
    for i in range(10):
        tp = patches.Circle((18 + i*3.5, 7), 0.7, fc='#ECC94B', ec='#B7791F', lw=0.5)
        ax.add_patch(tp)
    ax.text(35, 4, "TEST PAD CLUSTER (TP1 - TP34): SWD, UART, POWER", 
            ha='center', va='center', fontsize=5.5, color='#CBD5E0')

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_pcb_layout.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_pcb_layout.png")

# -------------------------------------------------------------
# 7. KiCad Schematic Architecture Diagram
# -------------------------------------------------------------
def make_schematic_diagram():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')

    ax.text(50, 66, "SCHEMATIC SHEET ARCHITECTURE & FUNCTIONAL SUB-BLOCKS", 
            ha='center', va='center', fontsize=12.5, fontweight='bold', color='#1A365D')

    sheets = [
        ("SHEET 1: POWER TREE & CHARGER", 28, 50, "#FEFCBF", "#D69E2E",
         "• USB Type-C Input Circuitry (CC1/CC2 5.1k Pull-Downs)\n• MCP73831 1S Li-Ion Linear Battery Charger\n• TPS62088 3.3V / 1.5A Synchronous Buck Regulator\n• Low-ESR Decoupling Capacitor Networks (10µF, 22µF)"),
        
        ("SHEET 2: STM32F407ZGT6 CORE & CLOCK", 72, 50, "#EBF8FF", "#2B6CB0",
         "• STM32F407ZGT6 LQFP-144 Core Power & Ground Pins\n• 8 MHz / 25 MHz High-Speed External (HSE) Crystal\n• 32.768 kHz Low-Speed External (LSE) RTC Oscillator\n• Serial Wire Debug (SWD) Header (PA13/SWDIO, PA14/SWCLK)"),

        ("SHEET 3: ENVIRONMENTAL SENSOR BUSES", 28, 22, "#F0FFF4", "#38A169",
         "• I2C1 Bus: 4.7kΩ Pull-Ups on PB6 (SCL) and PB7 (SDA)\n• Sensirion SCD41 Photoacoustic NDIR CO₂ Interface\n• Sensirion SHT41 Precision Climate Sensor on Slot\n• USART2 Interface for Plantower PMS5003 Laser PM"),

        ("SHEET 4: DISPLAY & USER INTERFACE", 72, 22, "#FAF5FF", "#805AD5",
         "• J1: 31-Pin 0.5mm FPC Connector for ST7789 IPS LCD\n• SPI1 High-Speed Bus: PA5 (SCK), PA7 (MOSI), PA4 (CS)\n• Backlight Low-Side MOSFET Switch driven by TIM PWM\n• J2: 6-Pin FPC Connector for DANY_TOUCH Slider Flex")
    ]

    for title, x, y, fc, ec, desc in sheets:
        b = patches.FancyBboxPatch((x-20, y-10), 40, 20, boxstyle="round,pad=1", fc=fc, ec=ec, lw=2)
        ax.add_patch(b)
        ax.text(x, y+6.5, title, ha='center', va='center', fontsize=8.5, fontweight='bold', color=ec)
        ax.text(x, y-2, desc, ha='center', va='center', fontsize=7.5, color='#2D3748', linespacing=1.25)

    # Interconnect lines between schematic sheets
    ax.annotate("", xy=(48, 50), xytext=(52, 50), arrowprops=dict(arrowstyle="<->", color='#4A5568', lw=2))
    ax.annotate("", xy=(48, 22), xytext=(52, 22), arrowprops=dict(arrowstyle="<->", color='#4A5568', lw=2))
    ax.annotate("", xy=(28, 32), xytext=(28, 40), arrowprops=dict(arrowstyle="<->", color='#4A5568', lw=2))
    ax.annotate("", xy=(72, 32), xytext=(72, 40), arrowprops=dict(arrowstyle="<->", color='#4A5568', lw=2))

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_kicad_schematic.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_kicad_schematic.png")

# -------------------------------------------------------------
# 8. Validation & Testing Workflow
# -------------------------------------------------------------
def make_validation_workflow():
    fig, ax = plt.subplots(figsize=(10, 6.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 70)
    ax.axis('off')

    ax.text(50, 66, "MULTI-TIER ENGINEERING VALIDATION & TESTING WORKFLOW", 
            ha='center', va='center', fontsize=12.5, fontweight='bold', color='#1A365D')

    tiers = [
        ("TIER 1: STATIC CODE & SCHEMATIC AUDIT", 16, 50, "#EDF2F7", "#4A5568",
         "• KiCad Electrical Rules Check (ERC)\n• Design Rules Check (DRC)\n• Secret & Credential Scanning\n• SHA-256 Evidence Hash Audit"),
        
        ("TIER 2: FIRMWARE UNIT VERIFICATION", 50, 50, "#EBF8FF", "#2B6CB0",
         "• US EPA AQI Calculation Suite\n• PMS5003 UART Packet Parser\n• Circular Ring Buffer FIFO Queue\n• Float Outlier Rejection Tests"),

        ("TIER 3: SOFTWARE SIMULATION LAYER", 84, 50, "#F0FFF4", "#38A169",
         "• Virtual Device State Machine\n• Scenarios: NORMAL, HIGH_PM, FAULT\n• Telemetry Receiver & SQLite DB\n• Schema & Boundary Validation"),

        ("TIER 4: BENCH HARDWARE VERIFICATION", 50, 20, "#FEFCBF", "#D69E2E",
         "• 3.3V Power Rail Oscilloscope Ripple Analysis (<35 mVpp)\n• Sensor Zero-Point & Span Calibration in Controlled Chamber\n• 40 MHz SPI Display Frame Rate Benchmark (≥30 FPS)\n• Long-Term Thermal Isolation Chamber Test (SHT41 Drift < 0.8°C)")
    ]

    for title, x, y, fc, ec, desc in tiers[:3]:
        b = patches.FancyBboxPatch((x-15, y-10), 30, 20, boxstyle="round,pad=1", fc=fc, ec=ec, lw=2)
        ax.add_patch(b)
        ax.text(x, y+6.5, title, ha='center', va='center', fontsize=8, fontweight='bold', color=ec)
        ax.text(x, y-2, desc, ha='center', va='center', fontsize=7.5, color='#2D3748', linespacing=1.2)

    # Big Tier 4 at bottom
    title, x, y, fc, ec, desc = tiers[3]
    b = patches.FancyBboxPatch((15, y-10), 70, 20, boxstyle="round,pad=1", fc=fc, ec=ec, lw=2)
    ax.add_patch(b)
    ax.text(x, y+6.5, title, ha='center', va='center', fontsize=8.5, fontweight='bold', color=ec)
    ax.text(x, y-2, desc, ha='center', va='center', fontsize=8, color='#744210', linespacing=1.3)

    # Arrows leading to Tier 4
    for xt in [20, 50, 80]:
        ax.annotate("", xy=(xt, 30), xytext=(xt, 40),
                    arrowprops=dict(arrowstyle="->", color='#4A5568', lw=2))

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, "fig_validation_workflow.png"), dpi=300)
    plt.close(fig)
    print("Generated: fig_validation_workflow.png")

if __name__ == "__main__":
    make_system_block_diagram()
    make_hardware_architecture()
    make_firmware_architecture()
    make_dataflow_pipeline()
    make_pcb_layout_diagram()
    make_schematic_diagram()
    make_validation_workflow()
    print("All report figures successfully generated in report_figures/")

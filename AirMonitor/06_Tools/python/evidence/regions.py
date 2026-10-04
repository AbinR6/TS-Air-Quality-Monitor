"""Single source of truth for image regions (crops) used in Phase 2.

Coordinates are pixel boxes (x0, y0, x1, y1) in the ORIGINAL image (900x1600 for all four).
'cls' sets the annotation colour per spec §7:
    OBSERVED (green), IDENTIFIED (blue), INFERRED (orange), UNCERTAIN (red).
The label must not state more than the class allows.

'redact' lists boxes (original-image coords) that are blurred in every derived image
(privacy: module MAC address and QR label). Originals are untouched.
"""

REDACT = {
    "E001": [(80, 935, 345, 980), (340, 795, 560, 990)],   # MAC line, QR label
    "E004": [(320, 60, 420, 150)],                          # QR label (small, top of frame)
}

REGIONS = [
    # ---------------- E001 main PCB, side A ----------------
    dict(id="E001-C01", img="E001", box=(70, 740, 565, 1110), cls="IDENTIFIED",
         label="U1 STM32F407ZGT6 MCU (marking: ARM Cortex-M4, LQFP144)"),
    dict(id="E001-C02", img="E001", box=(450, 785, 680, 1145), cls="OBSERVED",
         label="PCB edge routing / cut-out"),
    dict(id="E001-C03", img="E001", box=(110, 395, 385, 595), cls="OBSERVED",
         label="J? 31-pos FPC connector (pins 1..31)"),
    dict(id="E001-C04", img="E001", box=(80, 160, 275, 405), cls="INFERRED",
         label="DC-DC stage: SOT-23-6 + inductor + caps"),
    dict(id="E001-C05", img="E001", box=(385, 405, 485, 560), cls="UNCERTAIN",
         label="DFN/SON-8 IC, function unknown"),
    dict(id="E001-C06", img="E001", box=(455, 580, 560, 730), cls="OBSERVED",
         label="~6-pos FPC connector"),
    dict(id="E001-C07", img="E001", box=(555, 540, 692, 780), cls="UNCERTAIN",
         label="2x SMD parts in gasket (LED? / light sensor?)"),
    dict(id="E001-C08", img="E001", box=(40, 150, 115, 560), cls="OBSERVED",
         label="Silkscreen 'DANY_JPU_MB_P1_2022063?'"),
    dict(id="E001-C09", img="E001", box=(305, 150, 365, 265), cls="OBSERVED",
         label="Silkscreen '2604'"),
    dict(id="E001-C10", img="E001", box=(95, 1095, 525, 1200), cls="OBSERVED",
         label="Bottom-edge pads TP34/TP2x/TX?/RX?"),
    dict(id="E001-C11", img="E001", box=(40, 95, 125, 180), cls="OBSERVED",
         label="Mounting hole H1 (plated)"),
    dict(id="E001-C12", img="E001", box=(365, 615, 450, 700), cls="OBSERVED",
         label="Mounting hole H2 (plated, centre)"),
    dict(id="E001-C13", img="E001", box=(18, 1125, 95, 1205), cls="OBSERVED",
         label="Mounting hole H3 (plated)"),
    dict(id="E001-C14", img="E001", box=(40, 475, 175, 645), cls="UNCERTAIN",
         label="Left cluster near TP18 (red SMD part)"),
    dict(id="E001-C15", img="E001", box=(515, 465, 645, 550), cls="OBSERVED",
         label="TP1 / TP4 / TP5"),
    dict(id="E001-C16", img="E001", box=(230, 300, 365, 410), cls="OBSERVED",
         label="TP2 / TP25 / TP27 + SMD"),
    dict(id="E001-C17", img="E001", box=(455, 670, 560, 730), cls="OBSERVED",
         label="4x 0402/0603 passives below FPC2"),
    # ---------------- E003 enclosure interior ----------------
    dict(id="E003-C01", img="E003", box=(25, 695, 215, 875), cls="OBSERVED",
         label="Touch flex 'DANY_TOUCH'"),
    dict(id="E003-C02", img="E003", box=(275, 795, 415, 1150), cls="INFERRED",
         label="Cylindrical cell (Li-ion form factor)"),
    dict(id="E003-C03", img="E003", box=(635, 225, 880, 1115), cls="INFERRED",
         label="PM-sensor form factor (model unknown)"),
    dict(id="E003-C04", img="E003", box=(515, 335, 665, 865), cls="UNCERTAIN",
         label="Secondary PCB: 2xN header + crystal"),
    dict(id="E003-C05", img="E003", box=(415, 835, 625, 1045), cls="OBSERVED",
         label="3-wire connector red/black/white"),
    dict(id="E003-C06", img="E003", box=(405, 535, 560, 790), cls="UNCERTAIN",
         label="Black finned block (NDIR? heatsink? guide?)"),
    # ---------------- E004 display panel ----------------
    dict(id="E004-C01", img="E004", box=(235, 475, 605, 910), cls="UNCERTAIN",
         label="Display/front panel (technology unknown)"),
    dict(id="E004-C02", img="E004", box=(315, 390, 425, 560), cls="OBSERVED",
         label="Panel FPC tail + stiffener"),
    dict(id="E004-C03", img="E004", box=(170, 0, 505, 245), cls="OBSERVED",
         label="Main PCB (corroborates E001)"),
]

COLOURS = {
    "OBSERVED": (40, 200, 80),
    "IDENTIFIED": (40, 120, 255),
    "INFERRED": (255, 150, 30),
    "UNCERTAIN": (235, 50, 50),
}

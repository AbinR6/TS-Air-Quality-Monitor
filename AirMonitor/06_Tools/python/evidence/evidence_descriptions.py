"""Hand-maintained descriptive metadata for each evidence image.

These are agent/human judgements (OBSERVED-level descriptions), kept separate from the
measured values computed in ingest.py. Edit here, then re-run ingest.py.
"""

EVIDENCE = {
    "E001": {
        "original_filename": "WhatsApp Image 2026-10-03 at 22.04.59.jpeg",
        "slug": "main_pcb_side_a",
        "orientation": "Main PCB, one side ('side A'), photographed roughly top-down; module at bottom, "
                       "rounded corner at upper right",
        "source_description": "Main PCB removed from enclosure, on a wooden desk",
        "visible_components": "ESP32-WROVER-B module; 31-position FPC connector; ~6-position FPC connector; "
                              "SOT-23-6-class IC + shielded inductor + MLCCs; DFN/SON-8-class IC; two SMD parts in "
                              "black gasket; red SMD part near TP18; test pads TP1-TP34; 3 mounting holes",
        "readable_markings": "'ESP32-WROVER-B'; 'PN:T900YE2E01G34' (partially legible); CE; FCC ID 2AC7Z-ESP32...; "
                             "CMIIT ID 2018DP3125? ; silkscreen 'DANY_JPU_MB_P1_ESP32_2022063?' ; '2604'; "
                             "'TP1'...'TP34' (subset legible); FPC pin labels '1','31'",
        "connectors": "31-pos FPC (centre-left); ~6-pos FPC (centre-right); module PCB antenna overhang",
        "estimated_region": "Whole board, side A",
        "confidence": "HIGH",
        "usable_for": "marking reading (limited); geometry; layout; TP labels",
        "notes": "WhatsApp-recompressed. Reverse side (side B) not photographed. Module MAC/QR label present: "
                 "redacted in public derived images.",
    },
    "E002": {
        "original_filename": "WhatsApp Image 2026-10-03 at 22.04.59 (1).jpeg",
        "slug": "frame_display_fpc_blurred",
        "orientation": "Black internal frame/lid, FPC passing through a slot; severe motion blur",
        "source_description": "Enclosure sub-assembly with display flex",
        "visible_components": "Black plastic frame; gold/amber FPC; rectangular window/slot on right",
        "readable_markings": "none",
        "connectors": "FPC tail (end not resolvable)",
        "estimated_region": "Front/display side of internal frame (inferred)",
        "confidence": "LOW",
        "usable_for": "context only",
        "notes": "Motion blur prevents component-level claims.",
    },
    "E003": {
        "original_filename": "WhatsApp Image 2026-10-03 at 22.05.00.jpeg",
        "slug": "enclosure_interior",
        "orientation": "Looking into opened white enclosure from the open (rear/bottom) face",
        "source_description": "Enclosure interior with internal black frame and sub-assemblies installed",
        "visible_components": "Blue cylindrical cell; blue ribbed metal rectangular module (PM-sensor form factor); "
                              "secondary vertical PCB with dual-row 2.54 mm header and HC-49-style crystal; "
                              "white wire-to-board connector with red/black/white wires; black finned/slotted block; "
                              "amber flex with text 'DANY_TOUCH' (approx.)",
        "readable_markings": "flex text approx. 'DANY_TOUCH' (partially obscured, mirrored orientation)",
        "connectors": "white 3-wire housing; 2xN pin header; touch flex",
        "estimated_region": "Full device interior",
        "confidence": "HIGH",
        "usable_for": "architecture; mechanics; airflow context",
        "notes": "No labels visible on cell or PM module.",
    },
    "E004": {
        "original_filename": "WhatsApp Image 2026-10-03 at 22.05.00 (1).jpeg",
        "slug": "display_panel_and_pcb",
        "orientation": "Display/front panel face-up, rotated ~10 deg; main PCB partially at top edge",
        "source_description": "Display (or display+touch) panel removed, with FPC tail",
        "visible_components": "Rectangular glass panel with reflective active area and concentric rectangular "
                              "pattern; amber FPC with black stiffener; part of main PCB (module + gasket)",
        "readable_markings": "none legible",
        "connectors": "panel FPC tail",
        "estimated_region": "Display sub-assembly",
        "confidence": "MEDIUM",
        "usable_for": "display geometry; FPC context; corroboration of E001",
        "notes": "Panel technology not determinable from this image alone.",
    },
}

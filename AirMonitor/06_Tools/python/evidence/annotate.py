"""Phase 2 — annotated overlays per spec §7.

Colours: OBSERVED green, IDENTIFIED blue, INFERRED orange, UNCERTAIN red.
UNCERTAIN/INFERRED boxes are drawn dashed so weak evidence is not visually implied as certain.
Each output carries a watermark that it is a derived analysis image.
"""
from __future__ import annotations

import sys
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

sys.path.insert(0, str(Path(__file__).resolve().parent))
from make_crops import load  # noqa: E402
from regions import COLOURS, REGIONS  # noqa: E402

REPO = Path(__file__).resolve().parents[3]
OUT = REPO / "01_Evidence" / "annotated"


def font(size: int) -> ImageFont.ImageFont:
    for name in ("arial.ttf", "DejaVuSans.ttf"):
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            continue
    return ImageFont.load_default()


def dashed_rect(d: ImageDraw.ImageDraw, box, colour, width=3, dash=10):
    x0, y0, x1, y1 = box
    for x in range(x0, x1, dash * 2):
        d.line([(x, y0), (min(x + dash, x1), y0)], fill=colour, width=width)
        d.line([(x, y1), (min(x + dash, x1), y1)], fill=colour, width=width)
    for y in range(y0, y1, dash * 2):
        d.line([(x0, y), (x0, min(y + dash, y1))], fill=colour, width=width)
        d.line([(x1, y), (x1, min(y + dash, y1))], fill=colour, width=width)


def main() -> int:
    OUT.mkdir(parents=True, exist_ok=True)
    f_lbl, f_wm = font(15), font(18)
    for eid in sorted({r["img"] for r in REGIONS}):
        base = load(eid)
        # Extra right-hand panel for the legend + label list, so labels don't hide evidence.
        canvas = Image.new("RGB", (base.width + 520, base.height), (20, 20, 24))
        canvas.paste(base, (0, 0))
        d = ImageDraw.Draw(canvas)
        regs = [r for r in REGIONS if r["img"] == eid]
        for n, r in enumerate(regs, 1):
            col = COLOURS[r["cls"]]
            if r["cls"] in ("INFERRED", "UNCERTAIN"):
                dashed_rect(d, r["box"], col)
            else:
                d.rectangle(r["box"], outline=col, width=3)
            tx, ty = r["box"][0] + 3, max(r["box"][1] - 20, 0)
            d.rectangle((tx - 2, ty, tx + 30, ty + 19), fill=(0, 0, 0))
            d.text((tx, ty), f"{n}", fill=col, font=f_lbl)
        # side panel
        x, y = base.width + 15, 15
        d.text((x, y), f"{eid} — DERIVED ANALYSIS IMAGE", fill=(255, 255, 255), font=f_wm)
        y += 26
        d.text((x, y), "Feature inspection and optical analysis overlay", fill=(200, 200, 200), font=f_lbl)
        y += 30
        for cls, col in COLOURS.items():
            d.rectangle((x, y + 3, x + 14, y + 17), fill=col)
            d.text((x + 22, y), f"[{cls}]" + (" (dashed)" if cls in ("INFERRED", "UNCERTAIN") else ""),
                   fill=col, font=f_lbl)
            y += 22
        y += 15
        for n, r in enumerate(regs, 1):
            d.text((x, y), f"{n}. {r['id']}", fill=COLOURS[r["cls"]], font=f_lbl)
            y += 19
            d.text((x + 18, y), r["label"][:58], fill=(230, 230, 230), font=f_lbl)
            y += 24
        y += 10
        d.text((x, y), "Privacy: MAC/QR regions blurred.", fill=(160, 160, 160), font=f_lbl)
        canvas.save(OUT / f"{eid}_annotated.png")
        print(f"annotated {eid}: {len(regs)} regions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

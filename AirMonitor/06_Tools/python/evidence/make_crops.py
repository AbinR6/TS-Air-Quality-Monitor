"""Phase 2 — deterministic crop generation.

For each region in regions.py writes:
  01_Evidence/crops/<id>.png           raw crop (lossless, privacy-redacted)
  01_Evidence/crops/<id>_enh4x.png     DERIVED: 4x Lanczos + autocontrast + unsharp mask

Enhancement is classical and deterministic only. Generative upscaling is forbidden
because it invents detail (see plan, Phase 2 task 5).
"""
from __future__ import annotations

import csv
import sys
from pathlib import Path

from PIL import Image, ImageFilter, ImageOps

sys.path.insert(0, str(Path(__file__).resolve().parent))
from regions import REDACT, REGIONS  # noqa: E402

REPO = Path(__file__).resolve().parents[3]
ORIG = REPO / "01_Evidence" / "original"
OUT = REPO / "01_Evidence" / "crops"


def load(eid: str) -> Image.Image:
    path = next(ORIG.glob(f"{eid}_*.jpeg"))
    im = Image.open(path).convert("RGB")
    for box in REDACT.get(eid, []):
        region = im.crop(box).filter(ImageFilter.GaussianBlur(12))
        im.paste(region, box[:2])
    return im


def main() -> int:
    OUT.mkdir(parents=True, exist_ok=True)
    cache: dict[str, Image.Image] = {}
    index = []
    for r in REGIONS:
        im = cache.setdefault(r["img"], load(r["img"]))
        crop = im.crop(r["box"])
        crop.save(OUT / f"{r['id']}.png")
        enh = crop.resize((crop.width * 4, crop.height * 4), Image.LANCZOS)
        enh = ImageOps.autocontrast(enh, cutoff=1)
        enh = enh.filter(ImageFilter.UnsharpMask(radius=3, percent=120, threshold=2))
        enh.save(OUT / f"{r['id']}_enh4x.png")
        index.append({"crop_id": r["id"], "image": r["img"], "x0": r["box"][0], "y0": r["box"][1],
                      "x1": r["box"][2], "y1": r["box"][3], "class": r["cls"], "label": r["label"]})
    with (OUT / "crop_index.csv").open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(index[0]))
        w.writeheader()
        w.writerows(index)
    print(f"Wrote {len(index)} crops (+ enhanced variants) to {OUT.relative_to(REPO)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

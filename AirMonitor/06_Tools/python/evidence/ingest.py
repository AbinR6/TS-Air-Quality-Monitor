"""Phase 1 — Evidence ingestion.

Copies every surviving image byte-for-byte into 01_Evidence/original/ under an evidence ID,
computes SHA-256, extracts basic metadata (resolution, EXIF presence, file size) and an
objective sharpness metric (variance of a Laplacian), and writes image_manifest.csv.

Descriptive columns (orientation, visible components...) come from a hand-maintained
table in evidence_descriptions.py, so that human/agent judgement is kept separate
from measured values.

Re-running is idempotent. If an original already exists with a different hash, the script aborts.
"""
from __future__ import annotations

import csv
import hashlib
import shutil
import sys
from pathlib import Path

import numpy as np
from PIL import Image

sys.path.insert(0, str(Path(__file__).resolve().parent))
from evidence_descriptions import EVIDENCE  # noqa: E402

REPO = Path(__file__).resolve().parents[3]
SOURCE_DIR = REPO.parent / "pictures"
ORIG_DIR = REPO / "01_Evidence" / "original"
MANIFEST = REPO / "01_Evidence" / "image_manifest.csv"
SUMS = ORIG_DIR / "SHA256SUMS"

FIELDS = [
    "evidence_id", "filename", "original_filename", "sha256", "bytes", "resolution",
    "exif_present", "sharpness_laplacian_var", "orientation", "source_description",
    "visible_components", "readable_markings", "connectors", "estimated_region",
    "confidence", "usable_for", "notes",
]


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 16), b""):
            h.update(chunk)
    return h.hexdigest()


def laplacian_variance(img: Image.Image) -> float:
    """Variance of a 4-neighbour Laplacian on the greyscale image (higher = sharper)."""
    g = np.asarray(img.convert("L"), dtype=np.float32)
    lap = (-4 * g[1:-1, 1:-1] + g[:-2, 1:-1] + g[2:, 1:-1] + g[1:-1, :-2] + g[1:-1, 2:])
    return float(lap.var())


def main() -> int:
    ORIG_DIR.mkdir(parents=True, exist_ok=True)
    rows, sums = [], []
    for eid, meta in EVIDENCE.items():
        src = SOURCE_DIR / meta["original_filename"]
        if not src.exists():
            print(f"[ERROR] missing source for {eid}: {src}")
            return 1
        dst_name = f"{eid}_{meta['slug']}{src.suffix.lower()}"
        dst = ORIG_DIR / dst_name
        src_hash = sha256(src)
        if dst.exists() and sha256(dst) != src_hash:
            print(f"[ERROR] {dst} exists with different content. Originals must never change.")
            return 1
        if not dst.exists():
            shutil.copy2(src, dst)
        with Image.open(dst) as im:
            res = f"{im.width}x{im.height}"
            exif_present = bool(im.getexif())
            sharp = round(laplacian_variance(im), 1)
        rows.append({
            "evidence_id": eid, "filename": dst_name, "original_filename": src.name,
            "sha256": src_hash, "bytes": dst.stat().st_size, "resolution": res,
            "exif_present": exif_present, "sharpness_laplacian_var": sharp,
            **{k: meta[k] for k in FIELDS if k in meta and k not in ("original_filename",)},
        })
        sums.append(f"{src_hash}  {dst_name}")
        print(f"{eid}: {dst_name} {res} sharpness={sharp} exif={exif_present}")

    with MANIFEST.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=FIELDS)
        w.writeheader()
        w.writerows(rows)
    SUMS.write_text("\n".join(sums) + "\n", encoding="utf-8")
    print(f"Wrote {MANIFEST.relative_to(REPO)} and {SUMS.relative_to(REPO)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

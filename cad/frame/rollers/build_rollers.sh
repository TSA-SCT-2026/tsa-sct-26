#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="$ROOT_DIR/stl"

mkdir -p "$OUT_DIR"

if ! command -v openscad >/dev/null 2>&1; then
  echo "OpenSCAD CLI is not installed."
  echo "Install with: brew install --cask openscad"
  exit 1
fi

OPENSCAD_CMD=(openscad)
if [[ "$(uname -m)" == "arm64" ]] && file "$(command -v openscad)" | grep -q "x86_64"; then
  OPENSCAD_CMD=(arch -x86_64 openscad)
fi

echo "[1/7] validate shared parameters"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/_validate_params.stl" -D 'build_target="validate"' "$ROOT_DIR/drive_pulley.scad"
rm -f "$OUT_DIR/_validate_params.stl"

echo "[2/7] dimension report"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/_dimension_report.stl" -D 'build_target="report"' "$ROOT_DIR/drive_pulley.scad"
rm -f "$OUT_DIR/_dimension_report.stl"

echo "[3/7] render drive pulley"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/drive_pulley_proto_v1.stl" -D 'build_target="model"' "$ROOT_DIR/drive_pulley.scad"

echo "[4/7] render idler roller"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/idler_roller_proto_v1.stl" -D 'build_target="model"' "$ROOT_DIR/idler_roller.scad"

echo "[5/7] render bearing coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_bearing_fit_v1.stl" -D 'build_target="bearing"' "$ROOT_DIR/roller_coupons.scad"

echo "[6/7] render D bore coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_d_bore_fit_v1.stl" -D 'build_target="dbore"' "$ROOT_DIR/roller_coupons.scad"

echo "[7/7] render crown coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_crown_v1.stl" -D 'build_target="crown"' "$ROOT_DIR/roller_coupons.scad"

echo "Done. STL outputs in $OUT_DIR"

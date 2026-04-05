#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
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

echo "[1/8] validate shared parameters"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/_validate_params.stl" -D 'build_target="validate"' "$SRC_DIR/motor_pulley.scad"
rm -f "$OUT_DIR/_validate_params.stl"

echo "[2/8] dimension report"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/_dimension_report.stl" -D 'build_target="report"' "$SRC_DIR/motor_pulley.scad"
rm -f "$OUT_DIR/_dimension_report.stl"

echo "[3/8] render motor pulley"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/motor_pulley_proto_v1.stl" -D 'build_target="model"' "$SRC_DIR/motor_pulley.scad"

echo "[4/8] render drive roller"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/drive_roller_proto_v1.stl" -D 'build_target="model"' "$SRC_DIR/drive_roller.scad"

echo "[5/8] render idler roller"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/idler_roller_proto_v1.stl" -D 'build_target="model"' "$SRC_DIR/idler_roller.scad"

echo "[6/8] render bearing coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_bearing_fit_v1.stl" -D 'build_target="bearing"' "$SRC_DIR/roller_coupons.scad"

echo "[7/8] render shaft fit coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_shaft_fit_v1.stl" -D 'build_target="shaftfit"' "$SRC_DIR/roller_coupons.scad"

echo "[8/8] render crown coupon"
"${OPENSCAD_CMD[@]}" -o "$OUT_DIR/coupon_crown_v1.stl" -D 'build_target="crown"' "$SRC_DIR/roller_coupons.scad"

echo "Done. STL outputs in $OUT_DIR"

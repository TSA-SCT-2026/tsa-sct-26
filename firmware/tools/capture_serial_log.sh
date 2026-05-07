#!/bin/zsh
set -euo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
  echo "usage: $0 <serial_port> [label]" >&2
  echo "example: $0 /dev/cu.usbserial-0001 phase2" >&2
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
LOG_DIR="$REPO_ROOT/docs/runs/logs"
PORT="$1"
LABEL="${2:-serial}"
TIMESTAMP="$(date +%Y%m%d_%H%M%S)"
OUTFILE="$LOG_DIR/${TIMESTAMP}_${LABEL}.log"

mkdir -p "$LOG_DIR"

echo "Capturing $PORT to $OUTFILE"
echo "Press Ctrl+C to stop."

stty -f "$PORT" 115200 raw -echo -crtscts
cat "$PORT" | tee "$OUTFILE"

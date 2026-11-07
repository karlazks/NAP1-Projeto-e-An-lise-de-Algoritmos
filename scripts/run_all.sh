#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
make all
make bench
make figuras
echo "OK: results/ e figures/ prontos"

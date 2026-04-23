#!/usr/bin/env bash
###############################################################
# build.sh
#
# Builds Pico Launcher using the official BlocksDS Docker
# image. This avoids the need to install BlocksDS and the
# Wonderful Toolchain natively.
#
# Prerequisites:
#   - Docker
#   - libtwl submodule (auto-initialized if missing)
#
# Usage:
#   ./build.sh
#
# Output:
#   LAUNCHER.nds
###############################################################

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BLOCKSDS_IMAGE="skylyrac/blocksds:slim-latest"
DOCKER="docker"

# --- Init submodules ---
if [ ! -d "$SCRIPT_DIR/libs/libtwl/.git" ]; then
    echo "[>] Initializing libtwl submodule..."
    git submodule update --init libs/libtwl
fi

# --- Validate Docker ---
echo "[>] Checking Docker..."
if ! command -v "$DOCKER" &> /dev/null; then
    echo "[!] Error: Docker is not installed or not in PATH"
    echo "    Install Docker: https://docs.docker.com/get-docker/"
    exit 1
fi

if ! $DOCKER info &> /dev/null 2>&1; then
    echo "[!] Error: Docker daemon is not running"
    exit 1
fi

# --- Build ---
echo ""
echo "[>] Building Pico Launcher with BlocksDS..."
echo "    Image: $BLOCKSDS_IMAGE"
echo ""

$DOCKER run --rm \
    -v "$SCRIPT_DIR:/work" \
    -w /work \
    -u "$(id -u):$(id -g)" \
    "$BLOCKSDS_IMAGE" \
    bash -c 'make'

mv $SCRIPT_DIR/LAUNCHER.nds $SCRIPT_DIR/_picoboot.nds

echo ""
echo "[>] Build complete: $SCRIPT_DIR/_picoboot.nds"

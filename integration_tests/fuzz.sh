#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

mkdir -p "$SCRIPT_DIR/.cache/"
touch "$SCRIPT_DIR/.cache/ptok_out"
cd "$REPO_ROOT" && v run "$SCRIPT_DIR/toktest.vsh" > "$SCRIPT_DIR/.cache/ptok_out"

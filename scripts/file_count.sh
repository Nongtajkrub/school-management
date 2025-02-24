#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CURRENT_DIR="$(pwd)"

if [ "$SCRIPT_DIR" = "$CURRENT_DIR" ]; then
	cd ..
fi

find include src -type f \( -name '*.c' -o -name '*.h' \) | wc -l

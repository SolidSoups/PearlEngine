#!/bin/bash

set -e

echo "|========== 📝 Compiling... ==========|"
cmake -B build -S . -G Ninja -Wno-dev -Wno-deprecated -DCMAKE_COLOR_DIAGNOSTIC=ON
echo ""

./build_run.sh


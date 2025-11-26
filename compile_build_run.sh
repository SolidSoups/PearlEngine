#!/bin/bash

set -e

echo "|========== 📝 Compiling... ==========|"
cmake -B build -S . -G Ninja -Wno-dev -Wno-deprecated
echo ""

./build_run.sh


#!/bin/bash

set -e

echo "|========== 📝 Compiling... ==========|"
cmake -B build -S . -G Ninja
echo ""

./build_run.sh


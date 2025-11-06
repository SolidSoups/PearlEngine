#!/bin/bash

set -e

echo "|========== 🔨 Building... ===========|"
cmake --build build
echo ""

echo "|===== 🚂 Running executable... ======|"
./build/game_engine



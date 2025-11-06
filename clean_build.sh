#!/bin/bash

set -e

echo "|========= 🛖 Renovating... ==========|"
rm -rf ./build/
mkdir build
echo ""

echo "|========= 📝 Compiling... ===========|"
cmake -B build -S . -G Ninja
echo ""

echo "|========== 🔨 Building... ===========|"
cmake --build build
echo ""

echo "|===== 🚂 Running executable... ======|"
./build/game_engine

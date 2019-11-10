#!/bin/bash

# Init submodules
git submodule update --init --recursive

# Build directory
mkdir -p build_ninja
pushd "build_ninja"

# Configure and build
cmake .. -G "Ninja"
cmake --build . --config Release --parallel 4

popd

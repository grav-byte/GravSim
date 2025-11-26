#!/bin/bash
# this script generates a Visual Studio 2022 solution using CMake.
# It will only work on Windows systems with Visual Studio 2022 installed.

cmake -S . -B build_vs -G "Visual Studio 17 2022"
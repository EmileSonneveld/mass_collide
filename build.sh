#!/usr/bin/env bash
set -euxo pipefail

# Initialize submodules that were never pulled (leave already checked-out ones untouched)
cd "$(dirname "$0")"
git submodule status | awk '/^-/ {print $2}' | while read -r path; do
	git submodule update --init --recursive -- "$path"
done

mkdir -p cmake_build
cd cmake_build
cmake .. -DCMAKE_CONFIGURATION_TYPES="Debug;RelWithDebInfo"
cmake --build . --config Debug

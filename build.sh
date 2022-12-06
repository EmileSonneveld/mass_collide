mkdir cmake_build
cd cmake_build
cmake .. -DCMAKE_CONFIGURATION_TYPES="Debug;RelWithDebInfo"
cmake --build . --config Debug

#!/bin/sh
mkdir -p build
echo "-> building..."
cd build
cmake ..
make -j9
cd ..
echo "-> running..."
./build/lk_array_test
echo "-> done!"

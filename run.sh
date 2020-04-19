#!/bin/sh
mkdir -p build
echo "-> building..."
cd build
cmake ..
make -j 9
cd ..
echo "-> running..."
./build/lk_array
echo "-> done!"

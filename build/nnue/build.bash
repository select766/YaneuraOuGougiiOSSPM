#!/bin/bash
set -e
set -x

if [ ! -e .dl/suishopetite_20211123.k_p.nnue.cpp.gz ]; then
curl --create-dirs -RLo .dl/suishopetite_20211123.k_p.nnue.cpp.gz https://github.com/mizar/YaneuraOu/releases/download/resource/suishopetite_20211123.k_p.nnue.cpp.gz
fi
if [ ! -e .dl/embedded_nnue.cpp ]; then
gzip -cd .dl/suishopetite_20211123.k_p.nnue.cpp.gz > .dl/tmp.cpp
echo "#include <cstddef>" > .dl/embedded_nnue.cpp
echo "namespace YANEURAOU_GOUGI_NAMESPACE {" >> .dl/embedded_nnue.cpp
tail -n +2 .dl/tmp.cpp >> .dl/embedded_nnue.cpp
echo "}" >> .dl/embedded_nnue.cpp
rm .dl/tmp.cpp
fi

cmake -Sios_x86_64 -G Xcode ./ios_x86_64 -B bin/ios_x86_64 -DPLATFORM=SIMULATOR64  -DDEPLOYMENT_TARGET=15.0 -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake
cmake --build ./bin/ios_x86_64
cmake -Sios_arm64 -G Xcode ./ios_arm64 -B bin/ios_arm64 -DPLATFORM=OS64  -DDEPLOYMENT_TARGET=15.0 -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake
cmake --build ./bin/ios_arm64

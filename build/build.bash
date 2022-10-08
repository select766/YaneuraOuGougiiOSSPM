#!/bin/bash
set -e
set -x

cmake -Sios_x86_64 -G Xcode ./ios_x86_64 -B bin/ios_x86_64 -DPLATFORM=SIMULATOR64 -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake
cmake --build ./bin/ios_x86_64
cmake -Sios_arm64 -G Xcode ./ios_arm64 -B bin/ios_arm64 -DPLATFORM=OS64 -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake
cmake --build ./bin/ios_arm64
rm -r ../Frameworks/libyaneuraou.xcframework
xcodebuild -create-xcframework \
    -library bin/ios_arm64/Debug-iphoneos/libyaneuraou.a -headers ./include \
    -library bin/ios_x86_64/Debug-iphonesimulator/libyaneuraou.a -headers ./include \
    -output ../Frameworks/libyaneuraou.xcframework

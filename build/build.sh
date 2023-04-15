#!/bin/bash
set -e
set -x

for ENGINE in deep nnue; do
pushd ${ENGINE}
./build.bash
popd
done

for ARCHDIR in ios_arm64/Debug-iphoneos ios_x86_64/Debug-iphonesimulator; do

for ENGINE in deep nnue; do
rm -r bin/${ENGINE} || true
mkdir -p bin/${ENGINE}
pushd bin/${ENGINE}
ar x ../../${ENGINE}/bin/${ARCHDIR}/libyaneuraou.a
for file in *; do mv "$file" "deep_$file"; done
popd
done

mkdir -p bin/${ARCHDIR}
rm bin/${ARCHDIR}/libyaneuraou.a || true
ar rcs bin/${ARCHDIR}/libyaneuraou.a bin/deep/*.o bin/nnue/*.o

done

rm -r ../Frameworks/libyaneuraou.xcframework || true
xcodebuild -create-xcframework \
    -library bin/ios_arm64/Debug-iphoneos/libyaneuraou.a -headers ./include \
    -library bin/ios_x86_64/Debug-iphonesimulator/libyaneuraou.a -headers ./include \
    -output ../Frameworks/libyaneuraou.xcframework

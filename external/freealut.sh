#!/bin/sh

set -e

rm -rf freealut

git clone https://github.com/vancegroup/freealut
cd freealut
git checkout fc814e3

cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_INSTALL_PREFIX=.. \
                -DBUILD_STATIC=OFF \
                -DBUILD_TESTS=OFF \
                -DBUILD_EXAMPLES=OFF \
                -DBUILD_PROFILE=OFF \
                -DBUILD_OPTIMIZATION=ON \
                -DBUILD_WARNINGS=OFF \
                -DBUILD_MORE_WARNINGS=OFF \
                -DBUILD_USE_WERROR=OFF
cmake --build build
cmake --build build --target install

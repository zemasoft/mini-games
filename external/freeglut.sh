#!/bin/sh

set -e

rm -rf freeglut

git clone https://github.com/dcnieho/freeglut
cd freeglut
# git checkout FG_3_2_1

cd freeglut/freeglut
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_INSTALL_PREFIX=../../.. \
                -DFREEGLUT_BUILD_DEMOS=OFF \
                -DFREEGLUT_BUILD_SHARED_LIBS=ON \
                -DFREEGLUT_BUILD_STATIC_LIBS=OFF \
                -DFREEGLUT_GLES=OFF \
                -DFREEGLUT_WAYLAND=OFF \
                -DFREEGLUT_PRINT_ERRORS=OFF \
                -DFREEGLUT_PRINT_WARNINGS=OFF \
                -DFREEGLUT_REPLACE_GLUT=OFF
cmake --build build
cmake --build build --target install

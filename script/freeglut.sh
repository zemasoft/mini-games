#!/bin/bash

set -e

./common.sh

rm -rf freeglut

git clone https://github.com/dcnieho/freeglut
cd freeglut
git checkout bbd69d6

cd freeglut/freeglut
cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR} \
                -DOpenGL_GL_PREFERENCE=LEGACY \
                -DFREEGLUT_BUILD_STATIC_LIBS=OFF \
                -DFREEGLUT_PRINT_ERRORS=${print_errors} \
                -DFREEGLUT_PRINT_WARNINGS=${print_warnings} \
                -DFREEGLUT_REPLACE_GLUT=OFF \
                -DFREEGLUT_BUILD_DEMOS=OFF
cmake --build build
cmake --build build --target install

echo "All OK"

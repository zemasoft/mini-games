#!/bin/bash

set -e

if [[ ! -v BUILD_TYPE ]]; then
  echo "'BUILD_TYPE' not set. Set it using 'export BUILD_TYPE=type'" >&2
  exit 1
fi

if [[ ${BUILD_TYPE} == Debug ]]; then
  print_errors=ON
  print_warnings=ON
elif [[ ${BUILD_TYPE} == RelWithDebInfo ]]; then
  print_errors=OFF
  print_warnings=OFF
elif [[ ${BUILD_TYPE} == Release ]]; then
  print_errors=OFF
  print_warnings=OFF
else
  echo "'BUILD_TYPE' set to unknown value '${BUILD_TYPE}'. Use 'Debug', 'RelWithDebInfo', or 'Release'" >&2
  exit 1
fi

rm -rf freeglut

git clone https://github.com/dcnieho/freeglut
cd freeglut
git checkout bbd69d6

cd freeglut/freeglut
cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=../../.. \
                -DOpenGL_GL_PREFERENCE=LEGACY \
                -DFREEGLUT_BUILD_STATIC_LIBS=OFF \
                -DFREEGLUT_PRINT_ERRORS=${print_errors} \
                -DFREEGLUT_PRINT_WARNINGS=${print_warnings} \
                -DFREEGLUT_REPLACE_GLUT=OFF \
                -DFREEGLUT_BUILD_DEMOS=OFF
cmake --build build
cmake --build build --target install

echo "All OK"

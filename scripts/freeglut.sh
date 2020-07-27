#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/common.sh
mkdir -p ${DEPS_DIR}

rm -rf freeglut

git clone https://github.com/dcnieho/freeglut
cd freeglut

cd freeglut/freeglut
cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=${DEPS_DIR} \
                -DOpenGL_GL_PREFERENCE=LEGACY \
                -DFREEGLUT_BUILD_STATIC_LIBS=OFF \
                -DFREEGLUT_PRINT_ERRORS=${FREEGLUT_PRINT_ERRORS} \
                -DFREEGLUT_PRINT_WARNINGS=${FREEGLUT_PRINT_WARNINGS} \
                -DFREEGLUT_REPLACE_GLUT=OFF \
                -DFREEGLUT_BUILD_DEMOS=OFF
cmake --build build
cmake --build build --target install

cd ../../..
if [[ ${CLEANUP} == true ]]
then
  rm -rf freeglut
fi

echo "${SCRIPT_BASE}: All OK"

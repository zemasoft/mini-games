#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

source ${SCRIPT_DIR}/setenv.sh

rm -rf build

cmake -S ${SCRIPT_DIR}/../.. -B build \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DZGE_USE_FREEGLUT=${USE_FREEGLUT} \
      -DZGE_USE_GLFW=${USE_GLFW} \
      -DZGE_USE_SDL2=${USE_SDL2} \
      -DZGE_USE_FREEGLUT_FOR_TEXT=${USE_FREEGLUT_FOR_TEXT} \
      -DZGE_USE_FREEALUT_FOR_AUDIO=${USE_FREEALUT_FOR_AUDIO} \
      -DZGE_USE_SDL2_FOR_AUDIO=${USE_SDL2_FOR_AUDIO} \
      -DSLIDINGPUZZLE_WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS} \
      -DSNAKE_WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS} \
      -DVIDEOSTOP_WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS}
cmake --build build
cmake --build build --target package

echo "${SCRIPT_BASE}: All OK"

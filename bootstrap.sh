#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/scripts/bootstrap/whereami.sh

if [[ ! -v ${USE_FREEALUT_FOR_AUDIO} || ${USE_FREEALUT_FOR_AUDIO} == ON ]]
then
  ${SCRIPT_DIR}/scripts/bootstrap/freealut.sh
fi

if [[ ! -v ${USE_FREEGLUT} || ${USE_FREEGLUT} == ON || ${USE_FREEGLUT_FOR_TEXT} == ON ]]
then
  ${SCRIPT_DIR}/scripts/bootstrap/freeglut.sh
fi

if [[ ! -v ${USE_GLFW} || ${USE_GLFW} == ON ]]
then
  ${SCRIPT_DIR}/scripts/bootstrap/glfw.sh
fi

if [[ ! -v ${USE_SDL2} || ${USE_SDL2} == ON || ${USE_SDL2_FOR_AUDIO} == ON ]]
then
  ${SCRIPT_DIR}/scripts/bootstrap/sdl2.sh
fi

echo "${SCRIPT_BASE}: All OK"

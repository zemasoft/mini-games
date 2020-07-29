#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/scripts/whereami.sh

if [[ ! -v ${USE_FREEALUT} || ${USE_FREEALUT} == ON ]]
then
  ${SCRIPT_DIR}/scripts/freealut.sh
fi

if [[ ! -v ${USE_FREEGLUT} || ${USE_FREEGLUT} == ON || ${USE_FREEGLUT_FOR_TEXT} == ON ]]
then
  ${SCRIPT_DIR}/scripts/freeglut.sh
fi

if [[ ! -v ${USE_GLFW} || ${USE_GLFW} == ON ]]
then
  ${SCRIPT_DIR}/scripts/glfw.sh
fi

if [[ ! -v ${USE_SDL2} || ${USE_SDL2} == ON ]]
then
  ${SCRIPT_DIR}/scripts/sdl2.sh
fi

echo "${SCRIPT_BASE}: All OK"

#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/scripts/build/build_all_with_freeglut.sh
${SCRIPT_DIR}/scripts/build/build_all_with_glfw.sh
${SCRIPT_DIR}/scripts/build/build_all_with_sdl2.sh

echo "${SCRIPT_BASE}: All OK"

#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

export USE_FREEGLUT=OFF
export USE_GLFW=ON
export USE_SDL2=OFF

export USE_FREEGLUT_FOR_TEXT=ON;  export USE_FREEALUT_FOR_AUDIO=ON;  export USE_SDL2_FOR_AUDIO=OFF

${SCRIPT_DIR}/build.sh

export USE_FREEGLUT_FOR_TEXT=OFF; export USE_FREEALUT_FOR_AUDIO=ON;  export USE_SDL2_FOR_AUDIO=OFF

${SCRIPT_DIR}/build.sh

export USE_FREEGLUT_FOR_TEXT=ON;  export USE_FREEALUT_FOR_AUDIO=OFF; export USE_SDL2_FOR_AUDIO=OFF

${SCRIPT_DIR}/build.sh

export USE_FREEGLUT_FOR_TEXT=OFF; export USE_FREEALUT_FOR_AUDIO=OFF; export USE_SDL2_FOR_AUDIO=OFF

${SCRIPT_DIR}/build.sh

export USE_FREEGLUT_FOR_TEXT=ON;  export USE_FREEALUT_FOR_AUDIO=OFF; export USE_SDL2_FOR_AUDIO=ON

${SCRIPT_DIR}/build.sh

export USE_FREEGLUT_FOR_TEXT=OFF; export USE_FREEALUT_FOR_AUDIO=OFF; export USE_SDL2_FOR_AUDIO=ON

${SCRIPT_DIR}/build.sh

echo "${SCRIPT_BASE}: All OK"

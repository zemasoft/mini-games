#!/bin/bash

set -e

if [[ ! -v DEPS_DIR ]]
then
  echo "'DEPS_DIR' not set. Set it using 'export DEPS_DIR=/absolute/path/to/dir'" >&2
  exit 1
fi

echo "DEPS_DIR=${DEPS_DIR}"

if [[ ! -v BUILD_TYPE ]]
then
  echo "'BUILD_TYPE' not set. Set it using 'export BUILD_TYPE=type'" >&2
  exit 1
fi

if [[ ${BUILD_TYPE} == Debug ]]
then
  :
elif [[ ${BUILD_TYPE} == RelWithDebInfo ]]
then
  :
elif [[ ${BUILD_TYPE} == Release ]]
then
  :
else
  echo "'BUILD_TYPE' set to unknown value '${BUILD_TYPE}'. Use 'Debug', 'RelWithDebInfo', or 'Release'" >&2
  exit 1
fi

echo "BUILD_TYPE=${BUILD_TYPE}"

echo "USE_FREEGLUT=${USE_FREEGLUT}"
echo "USE_GLFW=${USE_GLFW}"
echo "USE_SDL2=${USE_SDL2}"
echo "USE_FREEGLUT_FOR_TEXT=${USE_FREEGLUT_FOR_TEXT}"
echo "USE_FREEALUT_FOR_AUDIO=${USE_FREEALUT_FOR_AUDIO}"
echo "USE_SDL2_FOR_AUDIO=${USE_SDL2_FOR_AUDIO}"

if [[ ! -v WARNINGS_AS_ERRORS ]]
then
  export WARNINGS_AS_ERRORS=ON  
fi

echo "WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS}"

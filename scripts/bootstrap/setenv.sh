#!/bin/bash

set -e

if [[ ! -v BUILD_TYPE ]]
then
  echo "'BUILD_TYPE' not set. Set it using 'export BUILD_TYPE=type'" >&2
  exit 1
fi

if [[ ${BUILD_TYPE} == Debug ]]
then
  FREEALUT_OPTIMIZATION=OFF
  FREEGLUT_PRINT_ERRORS=ON
  FREEGLUT_PRINT_WARNINGS=ON
elif [[ ${BUILD_TYPE} == RelWithDebInfo ]]
then
  FREEALUT_OPTIMIZATION=ON
  FREEGLUT_PRINT_ERRORS=OFF
  FREEGLUT_PRINT_WARNINGS=OFF
elif [[ ${BUILD_TYPE} == Release ]]
then
  FREEALUT_OPTIMIZATION=ON
  FREEGLUT_PRINT_ERRORS=OFF
  FREEGLUT_PRINT_WARNINGS=OFF
else
  echo "'BUILD_TYPE' set to unknown value '${BUILD_TYPE}'. Use 'Debug', 'RelWithDebInfo', or 'Release'" >&2
  exit 1
fi

echo "BUILD_TYPE=${BUILD_TYPE}"

if [[ ! -v DEPS_DIR ]]
then
  echo "'DEPS_DIR' not set. Set it using 'export DEPS_DIR=/absolute/path/to/dir'" >&2
  exit 1
fi

echo "DEPS_DIR=${DEPS_DIR}"

if [[ ! -v CLEANUP ]]
then
  export CLEANUP=TRUE
fi

echo "CLEANUP=${CLEANUP}"

#!/bin/bash

set -e

if [[ ! -v BUILD_TYPE ]]; then
  echo "'BUILD_TYPE' not set. Set it using 'export BUILD_TYPE=type'" >&2
  exit 1
fi

if [[ ${BUILD_TYPE} == Debug ]]; then
  optimization=OFF
elif [[ ${BUILD_TYPE} == RelWithDebInfo ]]; then
  optimization=ON
elif [[ ${BUILD_TYPE} == Release ]]; then
  optimization=ON
else
  echo "'BUILD_TYPE' set to unknown value '${BUILD_TYPE}'. Use 'Debug', 'RelWithDebInfo', or 'Release'" >&2
  exit 1
fi

if [[ ! -v EXTERNAL_DIR ]]; then
  echo "'EXTERNAL_DIR' not set. Set it using 'export EXTERNAL_DIR=type'" >&2
  exit 1
fi

mkdir -p ${EXTERNAL_DIR}

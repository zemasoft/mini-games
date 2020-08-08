#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

source ${SCRIPT_DIR}/setenv.sh
mkdir -p ${DEPS_DIR}

rm -rf whereami

git clone https://github.com/gpakosz/whereami
cd whereami

cp ${SCRIPT_DIR}/whereami_CMakeLists.txt CMakeLists.txt

cmake -S . -Bbuild \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}
cmake --build build
cmake --build build --target install

cd ..
if [[ ${CLEANUP} == true ]]
then
  rm -rf whereami
fi

echo "${SCRIPT_BASE}: All OK"

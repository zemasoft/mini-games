#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

source ${SCRIPT_DIR}/setenv.sh
mkdir -p ${DEPS_DIR}

rm -rf freealut

git clone https://github.com/vancegroup/freealut
cd freealut

cmake -S . -Bbuild \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX=${DEPS_DIR} \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_OPTIMIZATION=${FREEALUT_OPTIMIZATION}
cmake --build build
cmake --build build --target install

cd ..

if [[ ${CLEANUP} == TRUE ]]
then
  rm -rf freealut
fi

echo "${SCRIPT_BASE}: All OK"

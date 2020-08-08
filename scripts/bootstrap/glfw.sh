#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/setenv.sh
mkdir -p ${DEPS_DIR}

rm -rf glfw

git clone https://github.com/glfw/glfw
cd glfw

cmake -S . -Bbuild \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX=${DEPS_DIR} \
      -DBUILD_SHARED_LIBS=ON \
      -DGLFW_BUILD_EXAMPLES=OFF \
      -DGLFW_BUILD_TESTS=OFF \
      -DGLFW_BUILD_DOCS=OFF
cmake --build build
cmake --build build --target install

cd ..
if [[ ${CLEANUP} == true ]]
then
  rm -rf glfw
fi

echo "${SCRIPT_BASE}: All OK"

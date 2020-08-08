#!/bin/bash

set -e

SCRIPT_BASE=$(basename $(readlink -f $0))
SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/setenv.sh
mkdir -p ${DEPS_DIR}

rm -rf allegro5

git clone https://github.com/liballeg/allegro5
cd allegro5

cmake -S . -Bbuild \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}
cmake --build build
cmake --build build --target install

cd ..
if [[ ${CLEANUP} == true ]]
then
  rm -rf allegro5
fi

echo "${SCRIPT_BASE}: All OK"

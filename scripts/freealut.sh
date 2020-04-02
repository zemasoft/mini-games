#!/bin/bash

set -e

SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/common.sh
mkdir -p ${EXTERNAL_DIR}

rm -rf freealut

git clone https://github.com/vancegroup/freealut
cd freealut
git checkout fc814e3

cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR} \
                -DBUILD_EXAMPLES=OFF \
                -DBUILD_TESTS=OFF \
                -DBUILD_OPTIMIZATION=${optimization}
cmake --build build
cmake --build build --target install

cd ..
if [[ ${CLEANUP} == true ]]
then
  rm -rf freealut
fi

echo "All OK"

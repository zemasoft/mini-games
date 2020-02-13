#!/bin/bash

set -e

./common.sh

rm -rf glfw

git clone https://github.com/glfw/glfw
cd glfw
git checkout 76406c7

cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR} \
                -DBUILD_SHARED_LIBS=ON \
                -DGLFW_BUILD_EXAMPLES=OFF \
                -DGLFW_BUILD_TESTS=OFF \
                -DGLFW_BUILD_DOCS=OFF
cmake --build build
cmake --build build --target install

echo "All OK"

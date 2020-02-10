#!/bin/bash

set -e

if [[ ! -v BUILD_TYPE ]]; then
  echo "'BUILD_TYPE' not set. Set it using 'export BUILD_TYPE=type'" >&2
  exit 1
fi

if [[ ${BUILD_TYPE} == Debug ]]; then
  :
elif [[ ${BUILD_TYPE} == RelWithDebInfo ]]; then
  :
elif [[ ${BUILD_TYPE} == Release ]]; then
  :
else
  echo "'BUILD_TYPE' set to unknown value '${BUILD_TYPE}'. Use 'Debug', 'RelWithDebInfo', or 'Release'" >&2
  exit 1
fi

rm -rf glfw

git clone https://github.com/glfw/glfw
cd glfw
git checkout 76406c7

cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=.. \
                -DBUILD_SHARED_LIBS=ON \
                -DGLFW_BUILD_EXAMPLES=OFF \
                -DGLFW_BUILD_TESTS=OFF \
                -DGLFW_BUILD_DOCS=OFF
cmake --build build
cmake --build build --target install

echo "All OK"

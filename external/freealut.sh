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

rm -rf freealut

git clone https://github.com/vancegroup/freealut
cd freealut
git checkout fc814e3

cmake . -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
                -DCMAKE_INSTALL_PREFIX=.. \
                -DBUILD_EXAMPLES=OFF \
                -DBUILD_TESTS=OFF \
                -DBUILD_OPTIMIZATION=${optimization}
cmake --build build
cmake --build build --target install

echo "All OK"

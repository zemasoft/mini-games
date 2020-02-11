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

rm -rf sdl2

mkdir sdl2
wget -c https://www.libsdl.org/release/SDL2-2.0.10.tar.gz -O - | tar -xzv -C sdl2 --strip-components 1

cd sdl2
./configure --prefix=$(pwd)/..
make
make install

echo "All OK"

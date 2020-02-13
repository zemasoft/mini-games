#!/bin/bash

set -e

SCRIPT_DIR=$(dirname $(readlink -f $0))

${SCRIPT_DIR}/common.sh

rm -rf sdl2

mkdir sdl2
wget -c https://www.libsdl.org/release/SDL2-2.0.10.tar.gz -O - | tar -xzv -C sdl2 --strip-components 1

cd sdl2
./configure --prefix=${EXTERNAL_DIR}
make
make install

echo "All OK"

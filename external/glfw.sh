#!/bin/sh

set -e

rm -rf glfw

git clone https://github.com/glfw/glfw
cd glfw
git checkout 76406c7

cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_INSTALL_PREFIX=.. \
                -DBUILD_SHARED_LIBS=ON \
                -DGLFW_BUILD_TESTS=OFF \
                -DGLFW_BUILD_EXAMPLES=OFF \
                -DGLFW_BUILD_DOCS=OFF \
                -DGLFW_INSTALL=ON \
                -DGLFW_VULKAN_STATIC=OFF \
                -DGLFW_USE_OSMESA=OFF \
                -DGLFW_USE_HYBRID_HPG=OFF \
                -DGLFW_USE_WAYLAND=OFF
cmake --build build
cmake --build build --target install

echo "All OK"

cmake_minimum_required(VERSION 3.14)
project(hash_octree)

set(CMAKE_CXX_STANDARD 17)

add_executable(hash_octree tests/tests.cpp src/hash-octree.h src/hash-octree.cpp tests/tests.h src/point-data.h src/point-data.cpp src/importer.cpp src/importer.h src/encryptor.cpp src/encryptor.h src/defs.h src/node.cpp src/node.h src/exporter.cpp src/exporter.h src/data-utils.cpp src/data-utils.h)
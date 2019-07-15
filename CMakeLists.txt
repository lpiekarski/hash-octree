cmake_minimum_required(VERSION 3.14)
project(hash_octree)

set(CMAKE_CXX_STANDARD 17)

set(libraries
        src/defs.h
        src/data-utils.h
        src/hash-octree.h
        src/point-data.h
        src/importer.h
        src/encryptor.h
        src/node.h
        src/exporter.h)

set(sources
        src/data-utils.cpp
        src/point-data.cpp
        src/encryptor.cpp
        src/node.cpp
        src/hash-octree.cpp
        src/importer.cpp
        src/exporter.cpp)

set(test_libraries
        tests/test.h)

# test executables
add_executable(hash_octree_test ${libraries} ${sources} tests/hash-octree-test.cpp)

add_executable(point_data_test ${libraries} ${sources} tests/point-data-test.cpp)

# library
add_library(hash-octree ${libraries} ${sources})
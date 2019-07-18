cmake_minimum_required(VERSION 3.14)
project(hash_octree)

set(CMAKE_CXX_STANDARD 17)

set(libraries
        include/defs.h
        include/data-utils.h
        include/hash-octree.h
        include/point-data.h
        include/importer.h
        include/encryptor.h
        include/node.h
        include/exporter.h
        include/lookup-methods.h)

set(sources
        src/point-data.cpp
        src/encryptor.cpp
        src/node.cpp
        src/importer.cpp
        src/lookup-methods.cpp)

set(test_libraries
        tests/test.h)

# test executables
add_executable(hash_octree_test ${libraries} ${sources} tests/hash-octree-test.cpp)

add_executable(point_data_test ${libraries} ${sources} tests/point-data-test.cpp)

# library
add_library(hash-octree ${libraries} ${sources})
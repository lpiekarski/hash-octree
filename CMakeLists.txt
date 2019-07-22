cmake_minimum_required(VERSION 3.9)
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
        include/lookup-methods/lookup-methods.h
        include/lookup-methods/unordered-map-lookup-method.h)

set(sources
        src/point-data.cpp
        src/encryptor.cpp
        src/node.cpp
        src/lookup-methods/unordered-map-lookup-method.cpp)

set(test_libraries
        tests/test.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# test executables
add_executable(hash_octree_test ${libraries} ${sources} tests/hash-octree-test.cpp)
target_link_libraries(hash_octree_test Threads::Threads)

add_executable(lookup_method_test ${libraries} ${sources} tests/lookup-method-test.cpp)
target_link_libraries(lookup_method_test Threads::Threads)

# library
add_library(hash-octree ${libraries} ${sources})
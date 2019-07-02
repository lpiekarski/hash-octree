cmake_minimum_required(VERSION 3.14)
project(hash_octree)

set(CMAKE_CXX_STANDARD 17)

add_executable(hash_octree main.cpp hash-octree.h hash-octree.cpp)
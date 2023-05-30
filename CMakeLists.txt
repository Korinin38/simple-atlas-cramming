cmake_minimum_required(VERSION 3.25)
project(simple_atlas_cramming)

set(CMAKE_CXX_STANDARD 17)
include_directories(libs)

set(SORUCES
        src/ply_shape.cpp
        src/ply_shape.h
        src/main.cpp)

add_executable(simple_atlas_cramming ${SOURCES})

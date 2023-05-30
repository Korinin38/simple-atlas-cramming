cmake_minimum_required(VERSION 3.25)
project(simple_atlas_cramming)

set(CMAKE_CXX_STANDARD 17)
include_directories(libs)

set(SOURCES
        src/ply_shape.cpp
        src/ply_shape.h
        src/atlas_contour.cpp
        src/atlas_contour.h
        src/main.cpp)

add_executable(simple_atlas_cramming ${SOURCES})

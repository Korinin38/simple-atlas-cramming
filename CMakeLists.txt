cmake_minimum_required(VERSION 3.25)
project(simple_atlas_cramming)

set(CMAKE_CXX_STANDARD 17)
include_directories(libs)

add_executable(simple_atlas_cramming src/main.cpp)

cmake_minimum_required(VERSION 3.8.0)
project(cc_cheat)

add_library(${PROJECT_NAME} STATIC
    src/internal.cpp
)
set(CMAKE_CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

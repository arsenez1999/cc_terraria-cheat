cmake_minimum_required(VERSION 3.8.0)
project(sig-crypt)

add_executable(${PROJECT_NAME} main.cpp)
set(CMAKE_CXX_STANDART 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
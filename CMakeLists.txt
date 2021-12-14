cmake_minimum_required(VERSION 3.8.0)
project(terraria_cheat)

include_directories(C:\\directx\\Include)
link_directories(src/dependencies/minhook)

add_library(${PROJECT_NAME} SHARED
    src/dllmain.cpp
    src/utilities/console.cpp
    src/utilities/menu.cpp
    src/dependencies/kiero/kiero.cpp
)
set(CMAKE_CXX_STANDART 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} libMinHook.x86)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
cmake_minimum_required(VERSION 3.8.0)
project(terraria_cheat)

add_subdirectory(signature_crypt)
add_subdirectory(cc_cheat)

include_directories(cc_cheat/include)
include_directories(C:/directx/Include)
link_directories(src/dependencies/minhook)

file(GLOB imgui "src/dependencies/imgui/*.cpp")

add_library(${PROJECT_NAME} SHARED
    src/dllmain.cpp
    src/utilities/console.cpp
    src/utilities/menu.cpp
    src/utilities/hooks.cpp
    src/dependencies/kiero/kiero.cpp
    ${imgui}
)
set(CMAKE_CXX_STANDART 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} libMinHook.x86 cc_cheat)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
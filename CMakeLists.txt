cmake_minimum_required(VERSION 3.5)
project(rezistenta VERSION 0.2)

find_package(raylib REQUIRED)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(rezistenta 
    src/main.cpp 
    src/application.cpp
)

target_link_libraries(rezistenta PRIVATE raylib)

set_property(TARGET rezistenta PROPERTY CXX_STANDARD 11)
set_property(TARGET rezistenta PROPERTY CXX_STANDARD_REQUIRED ON)


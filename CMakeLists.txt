cmake_minimum_required(VERSION 3.10)
project(rezistenta VERSION 0.1)

# Find Raylib package
find_package(raylib REQUIRED)

# Set the default compiler to Clang
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

# Add executable
add_executable(rezistenta 
    src/main.cpp 
    src/application.cpp
)

# Link Raylib to your executable
target_link_libraries(rezistenta PRIVATE raylib)

# Set C++ standard
set_property(TARGET rezistenta PROPERTY CXX_STANDARD 11)
set_property(TARGET rezistenta PROPERTY CXX_STANDARD_REQUIRED ON)


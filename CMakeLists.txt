cmake_minimum_required(VERSION 3.29.1)
project(rezistenta VERSION 0.3.0)

find_package(raylib 5.0.0 REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(app)

set_property(TARGET rezistenta PROPERTY CXX_STANDARD 11)
set_property(TARGET rezistenta PROPERTY CXX_STANDARD_REQUIRED ON)


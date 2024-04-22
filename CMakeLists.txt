cmake_minimum_required(VERSION 3.5.0)
project(ReziEngine VERSION 0.5.0)

find_package(raylib 5.0.0 REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libReziEngine)
add_subdirectory(ReziEdit)

set_property(TARGET ReziEdit PROPERTY CXX_STANDARD 11)
set_property(TARGET ReziEdit PROPERTY CXX_STANDARD_REQUIRED ON)


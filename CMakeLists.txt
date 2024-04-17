cmake_minimum_required(VERSION 3.29.1)
project(rezistenta VERSION 0.2)

find_package(raylib 5.0.0 REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

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
target_link_libraries(rezistenta PRIVATE Eigen3::Eigen)

set_property(TARGET rezistenta PROPERTY CXX_STANDARD 11)
set_property(TARGET rezistenta PROPERTY CXX_STANDARD_REQUIRED ON)


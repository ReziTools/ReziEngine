
cmake_minimum_required(VERSION 3.5.0)
project(ReziEngine VERSION 0.5.0)

set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

set(EIGEN3_VERSION 3.4.0)
find_package(Eigen3 ${EIGEN3_VERSION} QUIET)
if(NOT Eigen3_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    eigen
    GIT_REPOSITORY      https://gitlab.com/libeigen/eigen.git
    GIT_TAG             ${EIGEN3_VERSION}
  )
  FetchContent_MakeAvailable(eigen)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libReziEngine)
add_subdirectory(ReziEdit)

set_property(TARGET ReziEdit PROPERTY CXX_STANDARD 11)
set_property(TARGET ReziEdit PROPERTY CXX_STANDARD_REQUIRED ON)

if (APPLE)
    target_link_libraries(ReziEngine "-framework IOKit")
    target_link_libraries(ReziEngine "-framework Cocoa")
    target_link_libraries(ReziEngine "-framework OpenGL")
endif()


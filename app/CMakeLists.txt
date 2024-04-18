add_executable(rezistenta Desktop.cpp)

include_directories(rezistenta PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(rezistenta PRIVATE raylib)
target_link_libraries(rezistenta PRIVATE Eigen3::Eigen)
target_link_libraries(rezistenta PRIVATE librezistenta)

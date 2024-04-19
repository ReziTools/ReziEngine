add_executable(ReziEngine Desktop.cpp)

include_directories(ReziEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(ReziEngine PRIVATE raylib)
target_link_libraries(ReziEngine PRIVATE Eigen3::Eigen)
target_link_libraries(ReziEngine PRIVATE libReziEngine)

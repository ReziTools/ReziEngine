add_executable(ReziCLI Entry.cpp)

include_directories(ReziCLI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../libReziEngine)
target_link_libraries(ReziCLI PRIVATE libReziEngine)
target_link_libraries(ReziCLI PRIVATE raylib)
target_link_libraries(ReziCLI PRIVATE Eigen3::Eigen)

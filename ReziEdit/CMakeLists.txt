add_executable(ReziEdit Desktop.cpp Editor.cpp gui/Button.cpp gui/TextBox.cpp)

include_directories(ReziEdit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../libReziEngine)
target_link_libraries(ReziEdit PRIVATE libReziEngine)
target_link_libraries(ReziEdit PRIVATE raylib)
target_link_libraries(ReziEdit PRIVATE Eigen3::Eigen)

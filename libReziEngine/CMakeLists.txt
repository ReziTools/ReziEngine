add_library(libReziEngine STATIC ReziContext.cpp ReziSolver.cpp)

target_link_libraries(libReziEngine PRIVATE Eigen3::Eigen)
target_link_libraries(libReziEngine PRIVATE raylib)
target_link_libraries(libReziEngine PRIVATE toml11)

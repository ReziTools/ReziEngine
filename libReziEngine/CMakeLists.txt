add_library(libReziEngine STATIC ReziContext.cpp)

target_link_libraries(libReziEngine PRIVATE Eigen3::Eigen)

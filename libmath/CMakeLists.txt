add_library(math
    math/vector.hpp
    math/vector.cpp
    math/matrix.hpp
    math/matrix.cpp
)

target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
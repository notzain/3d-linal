add_library(math
    math/math.cpp
    math/vector.cpp
    math/matrix.cpp
)

target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"/math)
add_executable(tests
    src/catch.hpp
    src/test_main.cpp
    src/test_vector.cpp
    src/test_matrix.cpp
)

target_link_libraries(tests PRIVATE math)

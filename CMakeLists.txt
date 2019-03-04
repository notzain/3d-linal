cmake_minimum_required(VERSION 3.10)
project(LinAlg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)
add_subdirectory(libmath)
add_subdirectory(app)
add_subdirectory(tests)
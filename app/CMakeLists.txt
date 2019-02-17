set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_executable(linalg-app
    src/main.cpp
)

target_link_libraries(linalg-app PRIVATE math ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

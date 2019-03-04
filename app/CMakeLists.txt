set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(SFML_IMGUI ${EXTERNAL_DIR}/imgui-sfml)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui)

add_subdirectory(${SFML_IMGUI})
include_directories(${IMGUI_ROOT} ${SFML_IMGUI})

add_executable(linalg-app
    src/main.cpp
)

target_link_libraries(linalg-app 
    math 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES} 
    ${imgui-sfml}
)

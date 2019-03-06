set(ImGuiSFMLGit            "https://github.com/eliasdaler/imgui-sfml.git")
set(ImGuiSFMLTag            "e5bc24e")
set(ImGuiSFMLDownloadDir    ${CMAKE_CURRENT_BINARY_DIR}/ImGuiSFMLDownload)
set(ImGuiSFMLSrcDir         ${CMAKE_CURRENT_BINARY_DIR}/ImGuiSFMLSrc)
set(ImGuiSFMLBuildDir       ${CMAKE_CURRENT_BINARY_DIR}/ImGuiSFMLBuild)

file(MAKE_DIRECTORY ${ImGuiSFMLDownloadDir})
file(WRITE ${ImGuiSFMLDownloadDir}/CMakeLists.txt "
cmake_minimum_required(VERSION 2.8.2)

project(ImGuiSFMLDownload NONE)

include(ExternalProject)
ExternalProject_Add(ImGuiSFML
    GIT_REPOSITORY    \"${ImGuiSFMLGit}\"
    GIT_TAG           \"${ImGuiSFMLTag}\"
    GIT_SHALLOW       TRUE
    SOURCE_DIR        \"${ImGuiSFMLSrcDir}\"
    BINARY_DIR        \"${ImGuiSFMLBuildDir}\"
    CONFIGURE_COMMAND \"\"
    BUILD_COMMAND     \"\"
    INSTALL_COMMAND   \"\"
    TEST_COMMAND      \"\"
    )
")

# Always Release build for deps
execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${ImGuiSFMLDownloadDir}"
)
if (result)
    message(FATAL_ERROR "CMake step for ImGuiSFML failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${ImGuiSFMLDownloadDir}"
)
if(result)
    message(FATAL_ERROR "Build step for ImGuiSFML failed: ${result}")
endif()

find_package(OpenGL COMPONENTS OpenGL REQUIRED)

add_library(imgui-sfml ${ImGui_SOURCES} ${ImGui_HEADERS} ${ImGuiSFMLSrcDir}/imgui-SFML.cpp)
target_include_directories(imgui-sfml SYSTEM PUBLIC ${ImGui_ROOT} ${ImGuiSFMLSrcDir})
target_link_libraries(imgui-sfml PUBLIC OpenGL::GL sfml-graphics sfml-window)
target_compile_definitions(imgui-sfml PUBLIC -DIMGUI_USER_CONFIG="imconfig-SFML.h")
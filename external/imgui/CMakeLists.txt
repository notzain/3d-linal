set(ImGuiGit            "https://github.com/ocornut/imgui.git")
set(ImGuiTag            "v1.68")
set(ImGuiDownloadDir    ${CMAKE_CURRENT_BINARY_DIR}/ImGuiDownload)
set(ImGuiSrcDir         ${CMAKE_CURRENT_BINARY_DIR}/ImGuiSrc)
set(ImGuiBuildDir       ${CMAKE_CURRENT_BINARY_DIR}/ImGuiBuild)

file(MAKE_DIRECTORY ${ImGuiDownloadDir})
file(WRITE ${ImGuiDownloadDir}/CMakeLists.txt "
cmake_minimum_required(VERSION 2.8.2)

project(ImGuiDownload NONE)

include(ExternalProject)
ExternalProject_Add(ImGui
    GIT_REPOSITORY    \"${ImGuiGit}\"
    GIT_TAG           \"${ImGuiTag}\"
    GIT_SHALLOW       TRUE
    SOURCE_DIR        \"${ImGuiSrcDir}\"
    BINARY_DIR        \"${ImGuiBuildDir}\"
    CONFIGURE_COMMAND \"\"
    BUILD_COMMAND     \"\"
    INSTALL_COMMAND   \"\"
    TEST_COMMAND      \"\"
    )
")

# Always Release build for deps
execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${ImGuiDownloadDir}"
)
if (result)
    message(FATAL_ERROR "CMake step for ImGui failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${ImGuiDownloadDir}"
)
if(result)
    message(FATAL_ERROR "Build step for ImGui failed: ${result}")
endif()

set(ImGui_ROOT ${ImGuiSrcDir} PARENT_SCOPE)

file(GLOB ImGui_SOURCES "${ImGuiSrcDir}/*.cpp")
set(ImGui_SOURCES ${ImGui_SOURCES} PARENT_SCOPE)

file(GLOB ImGui_HEADERS "${ImGuiSrcDir}/*.h")
set(ImGui_HEADERS ${ImGui_HEADERS} PARENT_SCOPE)
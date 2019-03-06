set(SFMLUrl            "https://github.com/SFML/SFML/archive/2.5.1.tar.gz")
set(SFMLDownloadDir    ${CMAKE_CURRENT_BINARY_DIR}/SFMLDownload)
set(SFMLSrcDir         ${CMAKE_CURRENT_BINARY_DIR}/SFMLSrc)
set(SFMLBuildDir       ${CMAKE_CURRENT_BINARY_DIR}/SFMLBuild)

file(MAKE_DIRECTORY ${SFMLDownloadDir})
file(WRITE ${SFMLDownloadDir}/CMakeLists.txt "
cmake_minimum_required(VERSION 2.8.2)

project(SFMLDownload NONE)

include(ExternalProject)
ExternalProject_Add(SFML
    URL               \"${SFMLUrl}\"
    SOURCE_DIR        \"${SFMLSrcDir}\"
    BINARY_DIR        \"${SFMLBuildDir}\"
    CONFIGURE_COMMAND \"\"
    BUILD_COMMAND     \"\"
    INSTALL_COMMAND   \"\"
    TEST_COMMAND      \"\"
    )
")

# Always Release build for deps
execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${SFMLDownloadDir}"
)
if (result)
    message(FATAL_ERROR "CMake step for SFML failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${SFMLDownloadDir}"
)
if(result)
    message(FATAL_ERROR "Build step for SFML failed: ${result}")
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(${SFMLSrcDir} ${SFMLBuildDir} EXCLUDE_FROM_ALL)

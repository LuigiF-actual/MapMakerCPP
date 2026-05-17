# CMakeList.txt : CMake project for MapMakerC++, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.30.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("MapMakerC++" LANGUAGES C CXX)

# Add source to this project's executable.
add_executable(MapMakerC++ 
"source/main.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MapMakerC++ PROPERTY CXX_STANDARD 20)
endif()


if(LINUX)
    # Had problems building the first on Linux (Mint 22) time so decided to include this
    find_path(X11_INCLUDE_DIR X11/Xlib.h)
    if(NOT X11_INCLUDE_DIR)
        message(FATAL_ERROR 
            "\n[ERROR] Missing X11 development libraries!\n"
            "Please run the following command in your terminal to fix this\n"
            "==================================================================================================\n"

            "sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev\n"
            
            "==================================================================================================\n"
            
            "see more at https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux#wayland"
        )
    endif()
endif()

# TODO: Add tests and install targets if needed.
include(FetchContent)

set(CMAKE_TLS_VERIFY OFF)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    URL "https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    tinyfiledialogs
    GIT_REPOSITORY "https://github.com/native-toolkit/libtinyfiledialogs.git"
    SOURCE_DIR tinyfiledialogs
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(raylib tinyfiledialogs)

file(REMOVE_RECURSE ${raylib_SOURCE_DIR}/examples)
file(REMOVE_RECURSE ${raylib_SOURCE_DIR}/projects)


add_library(tinyfiledialogs_lib)

target_sources(tinyfiledialogs_lib
    PRIVATE
        ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c
    
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.h
)

target_link_libraries(MapMakerC++
    raylib
    tinyfiledialogs_lib
)

target_include_directories(MapMakerC++ PUBLIC "include")
target_include_directories(MapMakerC++ PUBLIC ${tinyfiledialogs_SOURCE_DIR})
target_include_directories(MapMakerC++ PUBLIC "images")

target_compile_definitions(MapMakerC++ PRIVATE
    RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/images/"
)
# CMakeList.txt : CMake project for MapMakerC++, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.30.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  #set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")
endif()

project("MapMakerC++" LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB HEADER_FILES CONFIGURE_DEPENDS "include/*.hpp")

find_program(CLANG_FORMAT clang-format)

# Add source to this project's executable.
add_executable(MapMakerC++ 
    ${HEADER_FILES}
    "Core/Source/Core/main.cpp"
    "Core/Source/Core/raygui_impl.cpp"
)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    set_target_properties(MapMakerC++ PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;-p;${CMAKE_BINARY_DIR};--header-filter=.*(include|source).*"
    )
endif()


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


if(MSVC)
    target_compile_options(MapMakerC++ PRIVATE /W4)
else()
    target_compile_options(MapMakerC++ PRIVATE -Wall -Wextra)
endif() 

# TODO: Add tests and install targets if needed.

target_include_directories(MapMakerC++ PUBLIC "include")
target_include_directories(MapMakerC++ PUBLIC "images")

target_include_directories(MapMakerC++ SYSTEM PUBLIC ${raygui_SOURCE_DIR}/src)
target_include_directories(MapMakerC++ SYSTEM PUBLIC ${tinyfiledialogs_SOURCE_DIR})
target_include_directories(MapMakerC++ SYSTEM PUBLIC ${sqlite3_SOURCE_DIR})


target_compile_definitions(MapMakerC++ PRIVATE
    RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/images/"
)

target_compile_definitions(MapMakerC++ PRIVATE
    SAVES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/saves/"
)

set_source_files_properties(
    source/raygui_impl.cpp
    PROPERTIES
    COMPILE_OPTIONS "/analyze-"
)

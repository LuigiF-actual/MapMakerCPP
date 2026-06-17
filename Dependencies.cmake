#
# Dependencies
#
include(FetchContent)



set(CMAKE_TLS_VERIFY OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # Don't build the supplied example games
    
FetchContent_Declare(
    raylib
    URL "https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
    
FetchContent_MakeAvailable(raylib) 

FetchContent_Declare(
    raygui
    GIT_REPOSITORY "https://github.com/raysan5/raygui.git"
    GIT_SHALLOW TRUE 
    SYSTEM
)

FetchContent_MakeAvailable(raygui)


FetchContent_Declare(
    tinyfiledialogs
    GIT_REPOSITORY "https://git.code.sf.net/p/tinyfiledialogs/code"
)

FetchContent_MakeAvailable(tinyfiledialogs)

FetchContent_Declare(
    sqlite3
    URL "https://sqlite.org/2026/sqlite-amalgamation-3530100.zip"
    URL_HASH SHA3_256=3c07136e4f6b5dd0c395be86455014039597bc65b6851f7111e88f71b6e06114
)
FetchContent_MakeAvailable(sqlite3)

FetchContent_Declare(
    json 
    GIT_REPOSITORY "https://github.com/nlohmann/json.git"
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)


add_library(tinyfiledialogs_lib)

target_sources(tinyfiledialogs_lib
    PRIVATE
        ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c
    
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.h
)

add_library(sqlite3_lib)

target_sources(sqlite3_lib
    PRIVATE 
        ${sqlite3_SOURCE_DIR}/sqlite3.c 
        ${sqlite3_SOURCE_DIR}/shell.c 

    PUBLIC 
    FILE_SET HEADERS 
    FILES 
        ${sqlite3_SOURCE_DIR}/sqlite3.h 
        ${sqlite3_SOURCE_DIR}/sqlite3ext.h 

)

file(REMOVE_RECURSE ${raylib_SOURCE_DIR}/examples)
file(REMOVE_RECURSE ${raylib_SOURCE_DIR}/projects)
file(REMOVE_RECURSE ${json_SOURCE_DIR}/.git)
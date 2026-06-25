#
# App
#
###

###

file(GLOB HEADER_FILES CONFIGURE_DEPENDS "Include/*.hpp")
file(GLOB SOURCES_FILES CONFIGURE_DEPENDS "Source/*.cpp")


add_executable(App)
set_property(TARGET App PROPERTY CXX_STANDARD 23)

target_sources(App 
    PUBLIC
        ${HEADER_FILES}
    PRIVATE 
        ${SOURCES_FILES}
)

target_include_directories(App 
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/Include"
)

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCES_FILES})

target_link_libraries(App 
    PRIVATE 
    Core 
    raylib
    tinyfiledialogs_lib
    sqlite3_lib
)

target_include_directories(App PRIVATE Source)

# Set working directory for VS debugger
set_target_properties(App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)

target_compile_definitions(App PRIVATE
    TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Textures"
)

target_compile_definitions(App PRIVATE
    SAVES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Saves/"
)


target_include_directories(App SYSTEM PUBLIC ${raygui_SOURCE_DIR}/src)
target_include_directories(App SYSTEM PUBLIC ${tinyfiledialogs_SOURCE_DIR})

set_source_files_properties(
    source/raygui_impl.cpp
    PROPERTIES
    COMPILE_OPTIONS "/analyze-"
)

if(MINGW)
    target_link_libraries(App PRIVATE stdc++exp)
endif()
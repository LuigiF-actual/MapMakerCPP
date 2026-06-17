## Core #
set(SOURCES
	Source/Core/Application.h
	Source/Core/Application.cpp
	Source/Core/Window.h
	Source/Core/Window.cpp
	Source/Core/Layer.h 
	Source/Core/Event.h
	Source/Core/WindowEvents.h
	Source/Core/InputEvents.h
	Source/Core/SaveFileSystem.hpp
	Source/Core/SaveData.hpp
	Source/Core/DataBaseManager.hpp
	Source/Core/ConfigCore.hpp
	Source/Core/PopUpWarnings.hpp
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(Core 
	PUBLIC
	raylib
	tinyfiledialogs_lib
    sqlite3_lib
)

target_include_directories(Core SYSTEM PUBLIC ${tinyfiledialogs_SOURCE_DIR})
target_include_directories(Core SYSTEM PUBLIC ${sqlite3_SOURCE_DIR})

target_compile_definitions(Core PRIVATE
    TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Textures/"
)

target_compile_definitions(Core PRIVATE
    SAVES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Saves/"
)


target_include_directories(Core PUBLIC "Source")
target_include_directories(Core PUBLIC )
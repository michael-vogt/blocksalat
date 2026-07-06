cmake_minimum_required(VERSION 3.20)

project(BlockSalat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_CONSOLE "Build console app" ON)
option(BUILD_QT "Build Qt app" ON)

# -------------------------------------
# CORE LIB (Board, Tetromino)
# -------------------------------------
add_library(BlockSalatCore STATIC
		src/core/Board.cpp
		src/core/Game.cpp
		src/core/Tetromino.cpp
)

target_include_directories(BlockSalatCore PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(BlockSalatUIConsole STATIC
		src/ui/inputhandler/ConsoleInputHandler.cpp
		src/ui/renderer/ConsoleRenderer.cpp
)

target_include_directories(BlockSalatUIConsole PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(BlockSalatUIQt STATIC
		src/ui/inputhandler/ConsoleInputHandler.cpp
		src/ui/renderer/ConsoleRenderer.cpp
)

target_include_directories(BlockSalatUIQt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)




if(BUILD_CONSOLE)
	add_executable(BlockSalatConsole src/main.cpp)
	target_link_libraries(BlockSalatConsole PRIVATE
			BlockSalatCore
			BlockSalatUIConsole
	)
endif ()

if (BUILD_QT)
	find_package(Qt6 REQUIRED COMPONENTS Widgets)

	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)

	add_executable(BlockSalatQt src/main.cpp)

	target_link_libraries(BlockSalatQt PRIVATE
			BlockSalatCore
			BlockSalatUIQt
			Qt6::Widgets
	)
endif ()

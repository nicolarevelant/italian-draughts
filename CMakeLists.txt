cmake_minimum_required(VERSION 3.20)
project(italian-draughts
		VERSION 1.3.1
		DESCRIPTION "Italian Draughts: a strategy game for 2 players written in C++."
		LANGUAGES CXX)

set(PROJECT_PRETTY_NAME "Italian Draughts")
set(PROJECT_COPYRIGHT "Copyright (C) 2023 Nicola Revelant")
set(PROJECT_LICENSE "GNU General Public License, version 3 or later")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT DEFINED DATA_PATH)
	set(DATA_PATH "${CMAKE_SOURCE_DIR}/")
endif ()

message("Build type: ${CMAKE_BUILD_TYPE}")

if (DEFINED DATA_PATH)
	message(WARNING "Using custom application data path, ignoring manual pages and install rules")
	message("Data path: ${DATA_PATH}")

	# it creates the executable
	add_subdirectory(src)
else ()
	# system configuration path (used by configure_file)
	set(DATA_PATH /usr/share/${PROJECT_NAME}/)

	# Create the target
	add_subdirectory(src)

	# Compile and compress the manual pages
	add_subdirectory(man)

	# Install target
	install(TARGETS ${PROJECT_NAME} DESTINATION usr/bin)

	# Install the license
	install(FILES LICENSE DESTINATION usr/share/licenses/${PROJECT_NAME})

	# Install the .desktop file
	install(FILES italian-draughts.desktop DESTINATION usr/share/applications)

	# Install images
	install(DIRECTORY images DESTINATION "usr/share/${PROJECT_NAME}")

	# Install themes
	install(DIRECTORY themes DESTINATION "usr/share/${PROJECT_NAME}")
endif ()

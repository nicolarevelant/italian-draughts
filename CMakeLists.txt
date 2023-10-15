cmake_minimum_required(VERSION 3.20)
project(italian-draughts
		VERSION 1.3.4
		DESCRIPTION "Italian Draughts, a strategy board game"
		LANGUAGES CXX)

set(PROJECT_PRETTY_NAME "Italian Draughts")
set(PROJECT_COPYRIGHT "Copyright (C) 2023 Nicola Revelant")
set(PROJECT_LICENSE "GNU General Public License, version 3 or later")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG 1)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT DEFINED DATA_PATH)
	set(DATA_PATH "${CMAKE_SOURCE_DIR}")
endif ()

message("Build type: ${CMAKE_BUILD_TYPE}")

if (DEFINED DATA_PATH)
	message(WARNING "Using custom application data path, ignoring manual pages and install rules\nData path: ${DATA_PATH}")

	# Create the executable
	add_subdirectory(src)
else ()
	# System configuration path (used by configure_file)
	set(DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})

	# Create the target
	add_subdirectory(src)

	include(GNUInstallDirs)

	# Compile and compress the manual pages
	add_subdirectory(man)

	# Install target
	install(TARGETS ${PROJECT_NAME})

	# Install images
	install(DIRECTORY images DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")

	# Install colors
	install(DIRECTORY colors DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
endif ()

cmake_minimum_required(VERSION 3.20)
project(italian-draughts
		VERSION 1.3.4
		DESCRIPTION "Italian Draughts, a strategy board game"
		LANGUAGES CXX)

include(GNUInstallDirs)

option(DOCS "Generate and install documentation" OFF)

set(DATA_PATH "${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}" CACHE STRING "Application data path")

message("Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("Build type: ${CMAKE_BUILD_TYPE}")
message("DATA_PATH: ${DATA_PATH}")

set(PROJECT_PRETTY_NAME "Italian Draughts")
set(PROJECT_COPYRIGHT "Copyright (C) 2024 Nicola Revelant")
set(PROJECT_LICENSE "GNU General Public License, version 3 or later")

add_subdirectory(src)

if (DOCS)
	message("Install documentation: ON")
	add_subdirectory(doc)
else ()
	message("Install documentation: OFF")
endif ()

install(TARGETS ${PROJECT_NAME})
install(DIRECTORY images DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
install(DIRECTORY colors DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")

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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(SYSTEM_CFG_PATH "${CMAKE_SOURCE_DIR}/")

	# Prints all Cmake variables
	get_cmake_property(_variableNames VARIABLES)
	list (SORT _variableNames)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

# user specific configuration path (used by configure_file)
set(USER_CFG_PATH ~/.config/${PROJECT_NAME}/)
if (SYSTEM_CFG_PATH)
	message(WARNING "Using custom system path, ignoring manual pages and install rules")

	# it creates the executable
	add_subdirectory(src)
else()
	# system configuration path (used by configure_file)
	set(SYSTEM_CFG_PATH /etc/${PROJECT_NAME}/)

	# it creates the executable
	add_subdirectory(src)

	# if the target need to be installed compiles the manual pages
	add_subdirectory(man)

	# install files
	install(TARGETS ${PROJECT_NAME} DESTINATION usr/bin)

	# license
	install(FILES LICENSE DESTINATION usr/share/licenses/${PROJECT_NAME})

	# .desktop
	install(FILES italian-draughts.desktop DESTINATION usr/share/applications)

	set(INSTALL_CONFIG_PATH "etc/${PROJECT_NAME}")

	# images
	install(DIRECTORY images DESTINATION ${INSTALL_CONFIG_PATH})

	# themes
	install(DIRECTORY themes DESTINATION ${INSTALL_CONFIG_PATH})
endif()

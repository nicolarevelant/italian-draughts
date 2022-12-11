cmake_minimum_required(VERSION 3.20)
project(italian-draughts
		VERSION 1.2.5
		DESCRIPTION "Italian Draughts: the famous strategy game for 2 players written in C++"
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if (CMAKE_TOOLCHAIN_FILE) # Custom toolchain file support (e.g. vcpkg)
	message("Using the following toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

	# system configuration path (used by configure_file), executable must be executed from the CMAKE_SOURCE_DIR
	set(SYSTEM_CFG_PATH "")
	set(CMAKE_BUILD_TYPE Release)
	set(PORTABLE 1)
else()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og") # it disables optimizations that could interfere with debugging
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	else()
		message(FATAL_ERROR "Unsupported CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
	endif()
endif()

if (PORTABLE)
	message("Portable build enabled")
else()
	message("Portable build disabled")
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

# user specific configuration path (used by configure_file)
set(USER_CFG_PATH ~/.config/${PROJECT_NAME}/)

if (PORTABLE)
	# system configuration path (used by configure_file)
	set(SYSTEM_CFG_PATH "${CMAKE_SOURCE_DIR}/")

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

	# images
	install(FILES images/pcDame.png DESTINATION etc/${PROJECT_NAME}/images)
	install(FILES images/pcPawn.png DESTINATION etc/${PROJECT_NAME}/images)
	install(FILES images/plDame.png DESTINATION etc/${PROJECT_NAME}/images)
	install(FILES images/plPawn.png DESTINATION etc/${PROJECT_NAME}/images)

	# colors
	install(FILES colors/default DESTINATION etc/${PROJECT_NAME}/colors)

	# strings
	install(FILES strings/default DESTINATION etc/${PROJECT_NAME}/strings)

	# license
	install(FILES LICENSE DESTINATION usr/share/licenses/${PROJECT_NAME})
endif()


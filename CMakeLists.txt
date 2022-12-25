cmake_minimum_required(VERSION 3.20)
project(italian-draughts
		VERSION 1.2.5
		DESCRIPTION "Italian Draughts: the famous strategy game for 2 players written in C++"
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(SYSTEM_CFG_PATH "${CMAKE_SOURCE_DIR}/")
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

	set(INSTALL_CONFIG_PATH "etc/${PROJECT_NAME}")

	# images
	install(FILES images/pcDame.png DESTINATION ${INSTALL_CONFIG_PATH}/images)
	install(FILES images/pcPawn.png DESTINATION ${INSTALL_CONFIG_PATH}/images)
	install(FILES images/plDame.png DESTINATION ${INSTALL_CONFIG_PATH}/images)
	install(FILES images/plPawn.png DESTINATION ${INSTALL_CONFIG_PATH}/images)

	# themes
	install(FILES themes/default DESTINATION ${INSTALL_CONFIG_PATH}/themes)

	# locales
	install(FILES locales/default DESTINATION ${INSTALL_CONFIG_PATH}/locales)
endif()

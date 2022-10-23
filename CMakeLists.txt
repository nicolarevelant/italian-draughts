cmake_minimum_required(VERSION 3.20)
project(italian-draughts VERSION 1.2.3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if (CMAKE_TOOLCHAIN_FILE) # for portable AppImage builds
	message(WARNING "Using the following toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
	set(SYSTEM_CFG_PATH ../)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

else()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(WARNING "Build type: Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
		set(SYSTEM_CFG_PATH ../)

	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
		set(SYSTEM_CFG_PATH /etc/${PROJECT_NAME}/)

		# convert man pages
		add_custom_command(COMMENT "Converting manual pages"
				OUTPUT ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1 ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5
				COMMAND mkdir -p ${CMAKE_BINARY_DIR}/man
				COMMAND scdoc < ${CMAKE_SOURCE_DIR}/man/${PROJECT_NAME}.1.scd > ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1
				COMMAND scdoc < ${CMAKE_SOURCE_DIR}/man/${PROJECT_NAME}.5.scd > ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5
				DEPENDS ${CMAKE_SOURCE_DIR}/man/${PROJECT_NAME}.1.scd ${CMAKE_SOURCE_DIR}/man/${PROJECT_NAME}.5.scd
				)

		# compress man pages
		add_custom_command(COMMENT "Compressing manual pages"
				OUTPUT ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1.gz ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5.gz
				COMMAND gzip -f ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1 ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5
				DEPENDS ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1 ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5
				)

		set(SET_INSTALL_RULES 1)
	else()
		message(FATAL_ERROR "Unknown value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
	endif()
endif()

if (SET_INSTALL_RULES)
	add_executable(${PROJECT_NAME} main.cpp Chessboard.cpp ChessboardSquare.cpp
			GameLogic.cpp MinimaxThread.cpp Frame.cpp FrameEvents.cpp Resources.cpp man/${PROJECT_NAME}.1.gz man/${PROJECT_NAME}.5.gz)

	# install files
	install(TARGETS ${PROJECT_NAME} DESTINATION usr/bin)

	install(FILES ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.1.gz DESTINATION usr/share/man/man1)
	install(FILES ${CMAKE_BINARY_DIR}/man/${PROJECT_NAME}.5.gz DESTINATION usr/share/man/man5)
else()
	add_executable(${PROJECT_NAME} main.cpp Chessboard.cpp ChessboardSquare.cpp
			GameLogic.cpp MinimaxThread.cpp Frame.cpp FrameEvents.cpp Resources.cpp)
endif()

set(USER_CFG_PATH ~/.config/${PROJECT_NAME}/)
configure_file(ProjectVariables.h.in ProjectVariables.h)

find_package(wxWidgets CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE wx::base wx::core)

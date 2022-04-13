cmake_minimum_required(VERSION 3.20)
project(italian-draughts VERSION 1.2.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Build type: Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
    set(SYSTEM_CFG_PATH ../)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(SYSTEM_CFG_PATH /etc/${PROJECT_NAME}/)
else()
    message(FATAL_ERROR "Unknown value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

set(USER_CFG_PATH ~/.config/${PROJECT_NAME}/)

include_directories(SYSTEM /usr/lib/wx/include/gtk3-unicode-3.0 /usr/include/wx-3.0)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS __WXGTK__)

add_executable(${PROJECT_NAME} main.cpp Chessboard.cpp ChessboardSquare.cpp
        GameLogic.cpp MinimaxThread.cpp Frame.cpp Resources.cpp FrameEvents.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE "-pthread -lwx_gtk3u_core-3.0 -lwx_baseu-3.0 -lwx_gtk3u_adv-3.0")
configure_file(ProjectVariables.h.in ProjectVariables.h)
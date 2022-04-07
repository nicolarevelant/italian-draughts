cmake_minimum_required(VERSION 3.20)
project(Italian-Draughts VERSION 1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if (DEFINED RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(SYSTEM_CFG_PATH /etc/italian-draughts/)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
    set(SYSTEM_CFG_PATH ../)
endif()

set(USER_CFG_PATH ~/.config/italian-draughts/)

include_directories(SYSTEM /usr/lib/wx/include/gtk3-unicode-3.0 /usr/include/wx-3.0)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS WXUSINGDLL __WXGTK__)

add_executable(${PROJECT_NAME} main.cpp Chessboard.cpp ChessboardSquare.cpp
        GameLogic.cpp MinimaxThread.cpp MyFrame.cpp Resources.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME} "-pthread -lwx_gtk3u_core-3.0 -lwx_baseu-3.0 -lwx_gtk3u_adv-3.0")
configure_file(ProjectVariables.h.in ProjectVariables.h)
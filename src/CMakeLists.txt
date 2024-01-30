# Build the libraries and the executable

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall -Wextra -Wpedantic -Werror")

# wxWidgets
find_package(wxWidgets REQUIRED base core adv)
include(${wxWidgets_USE_FILE})

# for configure_file command and for indexing header files
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})

# build static libraries and link dependencies
add_subdirectory(GameUtils)
add_subdirectory(MatchManager)
target_link_libraries(MatchManager PRIVATE GameUtils)
add_subdirectory(ChessboardSquare)
add_subdirectory(ChessboardGrid)
target_link_libraries(ChessboardGrid PRIVATE MatchManager ChessboardSquare)
add_subdirectory(Frame)
target_link_libraries(Frame PRIVATE ChessboardGrid)
add_subdirectory(Resources)

# create executable, link libraries and configure special header file with Cmake variables
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame Resources ${wxWidgets_LIBRARIES})
configure_file(ProjectVariables.h.in ProjectVariables.h)

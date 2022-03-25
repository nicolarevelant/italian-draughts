# Build the libraries and the executable

# wxWidgets
find_package(wxWidgets REQUIRED base core)
include(${wxWidgets_USE_FILE})

# for configure_file command and for indexing header files
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

# build static libraries and link dependencies
add_subdirectory(GameAlgorithm)
add_subdirectory(GameUtils)
target_link_libraries(GameUtils PRIVATE GameAlgorithm)
add_subdirectory(MatchManager)
target_link_libraries(MatchManager PRIVATE GameUtils)
add_subdirectory(ChessboardSquare)
add_subdirectory(ChessboardGrid)
target_link_libraries(ChessboardGrid PRIVATE ChessboardSquare)
add_subdirectory(Frame)
target_link_libraries(Frame PRIVATE MatchManager ChessboardGrid)
add_subdirectory(Resources)

# create executable, link libraries and configure special header file with Cmake variables
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame Resources ${wxWidgets_LIBRARIES})
configure_file(ProjectVariables.h.in ProjectVariables.h)

# Build the libraries and the executable

# wxWidgets
find_package(wxWidgets REQUIRED base core)
include(${wxWidgets_USE_FILE})

# for configure_file command
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# build static libraries and link dependency
add_subdirectory(ChessboardManager)
add_subdirectory(ChessboardSquare)
add_subdirectory(Frame)
target_link_libraries(Frame PRIVATE ChessboardManager ChessboardGrid)
add_subdirectory(ChessboardGrid)
target_link_libraries(ChessboardGrid PRIVATE ChessboardSquare)
add_subdirectory(GameAlgorithm)
add_subdirectory(Resources)
add_subdirectory(GameManager)
target_link_libraries(GameManager PRIVATE GameAlgorithm)

add_executable(${PROJECT_NAME} main.cpp ../include/utils.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ChessboardManager ChessboardSquare)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame GameAlgorithm Resources GameManager)

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

configure_file(ProjectVariables.h.in ProjectVariables.h)

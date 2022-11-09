# Build the libraries and the executable

# wxWidgets
find_package(wxWidgets REQUIRED base core)
include(${wxWidgets_USE_FILE})

# for configure_file command
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# build static libraries and link dependency
add_subdirectory(ChessboardManager)
add_subdirectory(ChessboardSquare)
add_subdirectory(Frame)
target_link_libraries(Frame PRIVATE ChessboardSquare ChessboardManager)
add_subdirectory(GameLogic)
add_subdirectory(Resources)
add_subdirectory(WorkingThread)
target_link_libraries(WorkingThread PRIVATE GameLogic)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ChessboardManager ChessboardSquare)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame GameLogic Resources WorkingThread)

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

configure_file(ProjectVariables.h.in ProjectVariables.h)

# Build the libraries and the executable

# wxWidgets
find_package(wxWidgets REQUIRED base core adv)
include(${wxWidgets_USE_FILE})

# for configure_file command and for indexing header files
include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

# build static libraries and link dependencies
add_subdirectory(ChessboardSquare)
add_subdirectory(ChessboardGrid)
target_link_libraries(ChessboardGrid PRIVATE Checkers ChessboardSquare)
add_subdirectory(Frame)
target_link_libraries(Frame PRIVATE ChessboardGrid)
add_subdirectory(Resources)

# create executable, link libraries and configure special header file with Cmake variables
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame Resources ${wxWidgets_LIBRARIES})

install(TARGETS ${PROJECT_NAME})

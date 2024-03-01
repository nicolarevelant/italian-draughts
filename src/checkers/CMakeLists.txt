# Build the libraries
add_library(Checkers
	GameUtils.cpp
	MatchManager.cpp)

target_include_directories(Checkers PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

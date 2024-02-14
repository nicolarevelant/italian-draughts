# Source code structure

This directory contains various libraries (usually composed by 1 class)
for creating this program. The structure is described below.

Note:

- X : Y means X is subclass of Y
- X::Y means Y declared inside X

## Frame : wxFrame

It represents the window frame including menu bar and status bar.

## ChessboardGrid : wxPanel

A 8x8 grid panel that represents a chessboard.

### Chessboard::WorkerThread : wxThread

This class is used to call MatchManager's methods asynchronously.

## MatchManager

This class handles 1 or more matches

## GameUtils

This class provides a static method to find all possible moves from a specific
pieces' disposition and a static method to calculate the best move the PC
can make (Minimax algorithm)

### GameUtils::Move

It represents a move that can be done by the player or PC.

## ChessboardSquare : wxWindow

A class that represents a square in the chessboard.

## Resources

A class that provides the ability to fetch colors from a particular theme.

# Source code structure

This directory contains various libraries (usually composed by 1 class)
for creating this program. The structure is described below.

Note:

- X : Y means X is subclass of Y
- X::Y means Y, that is declares inside X

## Frame : wxFrame

It represents the window frame including menu bar and status bar.

## ChessboardGrid : wxPanel

A 8x8 grid panel that represents a chessboard.

## MatchManager

This class handle 1 or more matches using a specific ChessboardGrid

## GameUtils

This class provides a static method to find all possible moves from a specific
pieces' disposition.

### gameUtils::AlgorithmThread : wxThread

This class provides a simple way to calculate the best move for PC using a
separate thread that calls the 'calculateBestMove' static method of GameAlgorithm

### GameUtils::Move

It represents a move that can be done by the player or PC.

## ChessboardSquare : wxWindow

A class that represents a square in the chessboard.

## GameAlgorithm

This class allows the computer to play against a player providing a static method
to calculate the best move for PC.

## Resources

A class the provides the ability to fetch colors from a particular theme.

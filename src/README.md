# Source code structure

This directory contains various classes for creating this program. The structure
is described below.

## Frame : wxFrame

It represents the window frame.

## ChessboardGrid : wxPanel

A 8x8 grid panel that represents a chessboard. The class have to handle mouse events
associated with its squares.

## ChessboardManager

Class for handle matches.

### ChessboardManager::Move

It represents a move that can be done by the player or PC.

## ChessboardSquare : wxWindow

A class that represents a square in the chessboard.

## GameAlgorithm

This class allows PC to play against a player.

## Resources

A class the provides the ability to fetch colors from a particular theme.

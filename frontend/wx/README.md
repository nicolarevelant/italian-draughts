# wxWidgets-based frontend structure

Note:

- X : Y means X is subclass of Y
- X::Y means Y declared inside X

## Frame : wxFrame

Window frame used to display the chessboard, a menu bar and a status bar

## ChessboardGrid : wxPanel

A 8x8 grid that represents a chessboard.

### ChessboardGrid::WorkerThread : wxThread

This class is used to call MatchManager's methods asynchronously.

## ChessboardSquare : wxWindow

A class that represents a square in the chessboard.

## Resources

A class that provides the ability to fetch colors from a particular theme.

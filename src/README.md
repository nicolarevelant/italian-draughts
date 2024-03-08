# libCheckers

Library used to play checkers against the computer.

Currently the only checkers variant available is italian draughts.

Note:

- X : Y means X is subclass of Y
- X::Y means Y declared inside X

## MatchManager

Start new matches with the specified difficulty

## GameUtils

This class provides a static method to find all possible moves from a specific
pieces' disposition and a static method to calculate the best move the PC
can make (Minimax algorithm)

### GameUtils::Move

It represents a move that can be done by the player or PC.


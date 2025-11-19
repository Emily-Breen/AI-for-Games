#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.h"
#include "Board.h"
#include <vector>
// represents a move from (row1, col1) to (row2, col2)
struct Move {
	int row1, col1;
	int row2, col2;
};
// represents the current state of the board
struct Boardstate {

	Animal grid[BOARD_SIZE][BOARD_SIZE];
	Player currentPlayer;

};

static const int ULIMITED_POWER = 999999; // i really hope you get the reference :D

class Gameplay
{
public:
	// default constructor
	Gameplay();

	// function to choose the best move for the current player
	Move chooseBestMove(const Boardstate& state);

	// function to check for win condition
	bool checkWimCondition(const Boardstate& state, Player& winner);

private:
	// minimax algorithm with alpha-beta pruning
	int miniMax(Boardstate state, int depth, bool isMaximizing, int alpha, int beta);

	// heuristic evaluation function
	int evaluateBoard(const Boardstate& state);

	// function to generate all possible moves for the current player
	std::vector<Move> generateMoves(const Boardstate& state);

	// function to apply a move and return the new board state
	Boardstate makeMove(const Boardstate& state, const Move& move);

};


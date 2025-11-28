#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.h"
#include "Board.h"
#include <vector>
#include <limits>

// represents a move from (row1, col1) to (row2, col2)
struct Move {
	int row1, col1;
	int row2, col2;

	// Constructor for easy Move creation; we can use this to represent invalid moves or no-move
	Move() : row1(-1), col1(-1), row2(-1), col2(-1) {}
	Move(int r1, int c1, int r2, int c2) : row1(r1), col1(c1), row2(r2), col2(c2) {}

	// Check if this is a valid move
	bool isValid() const {
		return row1 >= 0 && col1 >= 0 && row2 >= 0 && col2 >= 0;
	}
};

// Struct for AI evaluation of each piece on the board
// No rendering information, otherwise performance will be TERRIBLE
struct PieceState {
	Player owner;
	AnimalType type;
};

// Represents the current state of the board
struct Boardstate {
	PieceState grid[BOARD_SIZE][BOARD_SIZE];
	Player currentPlayer;

	// Constructor
	Boardstate() : currentPlayer(Player::NoPlayer) {}

	// Copy constructor to duplicate board state
	Boardstate(const PieceState sourceGrid[BOARD_SIZE][BOARD_SIZE], Player player)
	{
		for (int row = 0; row < BOARD_SIZE; ++row)
		{
			for (int col = 0; col < BOARD_SIZE; ++col)
			{
				grid[row][col] = sourceGrid[row][col];
			}
		}

		currentPlayer = player;
	}
};

static const int UNLIMITED_POWER = 999999; // I really hope you get the reference :D

class Gameplay
{
public:
	Gameplay();

	// function to choose the best move for the current player
	Move chooseBestMove(const Boardstate& state, int depth);

	// function to check for win condition
	bool checkWimCondition(const Boardstate& state, Player& winner);
	std::vector<Move> getValidMovesForPiece(int row, int col, const Boardstate& state);
	static PieceState toPieceState(const Animal& animal);
	static Animal toAnimal(const PieceState& pieceState);

private:
	// minimax algorithm with alpha-beta pruning
	int miniMax(const Boardstate& state, int depth, bool isMaximizing, int alpha, int beta);

	// heuristic evaluation function
	int evaluateBoard(const Boardstate& state, Player maximizingPlayer);

	int evaluateThreats(const Boardstate& state, Player player);
	int checkForThreats(Player p1, Player p2, Player p3, Player p4, Player player);

	// function to generate all possible moves for the current player
	std::vector<Move> generateMoves(const Boardstate& state);

	// function to apply a move and return the new board state
	Boardstate makeMove(const Boardstate& state, const Move& move);

	// Helper function to check if a position is within board bounds
	bool isValidPosition(int row, int col) const;

	// The player that the AI is trying to maximize
	Player m_maximizingPlayer;

	// Counter for debugging - tracks how many board states the AI evaluated before choosing a move
	int m_nodesEvaluated;
};


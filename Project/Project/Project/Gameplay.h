#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.h"
#include "Board.h"
#include <vector>
#include <limits>
/**
 * @file Gameplay.h
 * @brief Contains AI logic, board evaluation, move generation and minimax.
 */

/**
 * @struct Move
 * @brief Represents a move from one board coordinate to another.
 */

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

/**
 * @struct PieceState
 * @brief Representation of a board piece used for AI calculations.
 * Contains only owner and type.
 */

// No rendering information, otherwise performance will be TERRIBLE
struct PieceState {
	Player owner;
	AnimalType type;
};

/**
 * @struct Boardstate
 * @brief Represents the full internal board state used by AI.
 *
 * Contains a grid of PieceState and the current player's turn.
 */
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

static const int UNLIMITED_POWER = 999999; ///< Infinity value for evaluation
/**
 * @class Gameplay
 * @brief Handles all AI logic: minimax, evaluation, move generation, win checks.
 */
class Gameplay
{
public:
    /**
     * @brief Constructs a Gameplay object with default AI parameters.
     */
	Gameplay();
	/**
	 * @brief Computes the best move for the current player using minimax.
	 * @param state Current board state.
	 * @param depth Search depth for minimax.
	 * @return The best move found.
	 */
	Move chooseBestMove(const Boardstate& state, int depth);

	/**
	 * @brief Checks if the board contains a win condition.
	 * @param state The current board state.
	 * @param winner Output parameter storing the winning player.
	 * @return true if a player has achieved four in a row.
	 */
	bool checkWimCondition(const Boardstate& state, Player& winner);
	/**
	 * @brief Gets all valid moves for a piece located at (row, col).
	 * @param row Piece row.
	 * @param col Piece column.
	 * @param state Current board state.
	 * @return List of valid moves.
	 */

	std::vector<Move> getValidMovesForPiece(int row, int col, const Boardstate& state);
	/**
	 * @brief Converts an Animal instance into a PieceState.
	 */
	static PieceState toPieceState(const Animal& animal);
	/**
	* @brief Converts a PieceState into an Animal instance.
	*/
	static Animal toAnimal(const PieceState& pieceState);

private:
	/**
	 * @brief Minimax algorithm with alpha-beta pruning.
	 * @param state Current board state.
	 * @param depth Remaining recursion depth.
	 * @param isMaximizing True if evaluating AI's turn, false for opponent.
	 * @param alpha Alpha pruning value.
	 * @param beta Beta pruning value.
	 * @return The evaluated score.
	 */
	int miniMax(const Boardstate& state, int depth, bool isMaximizing, int alpha, int beta);

	/**
	 * @brief Heuristic board evaluation used when minimax depth ends.
	 * @param state Current board state.
	 * @param maximizingPlayer AI-controlled player.
	 * @return Numeric score (higher = better for AI).
	 */
	int evaluateBoard(const Boardstate& state, Player maximizingPlayer);
	/**
	 * @brief Calculates threat patterns (3 in a row with one empty).
	 */
	int evaluateThreats(const Boardstate& state, Player player);
	/**
	 * @brief Checks four tiles for a potential threat.
	 */
	int checkForThreats(Player p1, Player p2, Player p3, Player p4, Player player);

	/**
	 * @brief Generates all legal moves for the current player.
	 */
	std::vector<Move> generateMoves(const Boardstate& state);

	/**
	 * @brief Applies a move to a board and returns the resulting state.
	 */
	Boardstate makeMove(const Boardstate& state, const Move& move);

	/**
	 * @brief Checks whether a board coordinate is valid.
	 */
	bool isValidPosition(int row, int col) const;

	// The player that the AI is trying to maximize
	Player m_maximizingPlayer;

	// Counter for debugging - tracks how many board states the AI evaluated before choosing a move
	int m_nodesEvaluated;
};


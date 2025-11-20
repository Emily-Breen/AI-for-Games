#include "Gameplay.h"
#include <algorithm>
#include <iostream>

Gameplay::Gameplay() : m_maximizingPlayer(Player::Player2), m_nodesEvaluated(0)
{
}

// =============================================================================
// MAIN AI FUNCTION - Call this to get the AI's move
// =============================================================================
Move Gameplay::chooseBestMove(const Boardstate& state, int depth)
{
	m_nodesEvaluated = 0;
	m_maximizingPlayer = state.currentPlayer;

	std::vector<Move> possibleMoves = generateMoves(state);

	if (possibleMoves.size() > 10) {
		possibleMoves.resize(10);
	}

	if (possibleMoves.empty()) {
		std::cout << "No valid moves available!\n";
		return Move(); // Return invalid move
	}

	Move bestMove;
	int bestScore = -UNLIMITED_POWER;
	int alpha = -UNLIMITED_POWER;
	int beta = UNLIMITED_POWER;

	std::cout << "AI evaluating " << possibleMoves.size() << " possible moves...\n";

	// Try each possible move and evaluate it
	for (const Move& move : possibleMoves) {
		// Create new board state with this move applied
		Boardstate newState = makeMove(state, move);

		// Evaluate this move using minimax (opponent's turn, so minimizing)
		int score = miniMax(newState, depth, false, alpha, beta);

		std::cout << "Move (" << move.row1 << "," << move.col1 << ") -> ("
			<< move.row2 << "," << move.col2 << ") scored: " << score << "\n";

		// Keep track of best move
		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}

		alpha = std::max(alpha, score);
	}

	std::cout << "AI chose move with score " << bestScore
		<< " (evaluated " << m_nodesEvaluated << " nodes)\n";

	return bestMove;
}

// =============================================================================
// MINIMAX ALGORITHM WITH ALPHA-BETA PRUNING
// =============================================================================
int Gameplay::miniMax(const Boardstate& state, int depth, bool isMaximizing, int alpha, int beta)
{
	m_nodesEvaluated++;

	// BASE CASE 1: Check if game is over (someone won)
	Player winner;
	if (checkWimCondition(state, winner)) {
		if (winner == m_maximizingPlayer) {
			return UNLIMITED_POWER - (10 - depth); // Prefer faster wins
		}
		else {
			return -UNLIMITED_POWER + (10 - depth); // Prefer slower losses
		}
	}

	// BASE CASE 2: Maximum depth reached
	if (depth == 0) {
		return evaluateBoard(state, m_maximizingPlayer);
	}

	// Generate all possible moves for current player
	std::vector<Move> possibleMoves = generateMoves(state);

	// BASE CASE 3: No moves available (stalemate)
	if (possibleMoves.empty()) {
		return 0; // Draw
	}

	// RECURSIVE CASE: Evaluate all possible moves
	if (isMaximizing) {
		// MAX NODE: AI is trying to maximize score
		int maxEval = -UNLIMITED_POWER;

		for (const Move& move : possibleMoves) {
			Boardstate newState = makeMove(state, move);

			// Recursively evaluate this move
			int eval = miniMax(newState, depth - 1, false, alpha, beta);
			maxEval = std::max(maxEval, eval);

			// Alpha-beta pruning
			alpha = std::max(alpha, eval);
			if (beta <= alpha) {
				break; // Beta cutoff - prune remaining branches
			}
		}

		return maxEval;
	}
	else {
		// MIN NODE: Opponent is trying to minimize score
		int minEval = UNLIMITED_POWER;

		for (const Move& move : possibleMoves) {
			Boardstate newState = makeMove(state, move);

			// Recursively evaluate this move
			int eval = miniMax(newState, depth - 1, true, alpha, beta);
			minEval = std::min(minEval, eval);

			// Alpha-beta pruning
			beta = std::min(beta, eval);
			if (beta <= alpha) {
				break; // Alpha cutoff - prune remaining branches
			}
		}

		return minEval;
	}
}

// =============================================================================
// BOARD EVALUATION FUNCTION
// =============================================================================
int Gameplay::evaluateBoard(const Boardstate& state, Player maximizingPlayer)
{
	int score = 0;

	// Simple piece counting + center control
	// Valid tiles closer to the center should be more valuable than edge tiles
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			if (state.grid[row][col].isEmpty()) continue;

			if (state.grid[row][col].getOwner() == maximizingPlayer) {
				score += 10;
				// Bonus for center
				if (row >= 1 && row <= 3 && col >= 1 && col <= 3) score += 5;
			}
			else {
				score -= 10;
				if (row >= 1 && row <= 3 && col >= 1 && col <= 3) score -= 5;
			}
		}
	}

	return score;
}

// =============================================================================
// HELPER FUNCTION: Count consecutive pieces in a direction
// =============================================================================
int Gameplay::countConsecutivePieces(const Boardstate& state, Player player,
	int row, int col, int deltaRow, int deltaCol) const
{
	int count = 0;
	int checkRow = row;
	int checkCol = col;

	// Count consecutive pieces in the specified direction
	while (isValidPosition(checkRow, checkCol) &&
		!state.grid[checkRow][checkCol].isEmpty() &&
		state.grid[checkRow][checkCol].getOwner() == player) {
		count++;
		checkRow += deltaRow;
		checkCol += deltaCol;
	}

	return count;
}

// =============================================================================
// GENERATE ALL POSSIBLE MOVES
// =============================================================================
std::vector<Move> Gameplay::generateMoves(const Boardstate& state)
{
	std::vector<Move> moves;

	// Find all pieces belonging to the current player
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			// Skip empty cells and opponent's pieces
			if (state.grid[row][col].isEmpty() ||
				state.grid[row][col].getOwner() != state.currentPlayer) {
				continue;
			}

			// Get all valid moves for this piece
			std::vector<sf::Vector2i> validMoves =
				state.grid[row][col].getValidMoves(row, col, &state.grid[0][0], BOARD_SIZE);

			// Convert each valid destination into a Move
			for (const sf::Vector2i& dest : validMoves) {
				moves.push_back(Move(row, col, dest.x, dest.y));
			}
		}
	}

	return moves;
}

// =============================================================================
// APPLY A MOVE TO CREATE NEW BOARD STATE
// =============================================================================
Boardstate Gameplay::makeMove(const Boardstate& state, const Move& move)
{
	// Create a copy of the current state
	Boardstate newState;
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			newState.grid[row][col] = state.grid[row][col];
		}
	}

	// Move the piece
	newState.grid[move.row2][move.col2] = state.grid[move.row1][move.col1];

	// Clear the original position
	newState.grid[move.row1][move.col1] = Animal(); // Empty animal

	// Switch to the other player
	newState.currentPlayer = (state.currentPlayer == Player::Player1) ?
		Player::Player2 : Player::Player1;

	return newState;
}

// =============================================================================
// CHECK WIN CONDITION
// =============================================================================
bool Gameplay::checkWimCondition(const Boardstate& state, Player& winner)
{
	// Check horizontal wins
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col <= BOARD_SIZE - 4; ++col) {
			Player firstPlayer = state.grid[row][col].getOwner();
			if (firstPlayer == Player::NoPlayer) continue;

			bool isWin = true;
			for (int i = 1; i < 4; ++i) {
				if (state.grid[row][col + i].getOwner() != firstPlayer) {
					isWin = false;
					break;
				}
			}
			if (isWin) {
				winner = firstPlayer;
				return true;
			}
		}
	}

	// Check vertical wins
	for (int col = 0; col < BOARD_SIZE; ++col) {
		for (int row = 0; row <= BOARD_SIZE - 4; ++row) {
			Player firstPlayer = state.grid[row][col].getOwner();
			if (firstPlayer == Player::NoPlayer) continue;

			bool isWin = true;
			for (int i = 1; i < 4; ++i) {
				if (state.grid[row + i][col].getOwner() != firstPlayer) {
					isWin = false;
					break;
				}
			}
			if (isWin) {
				winner = firstPlayer;
				return true;
			}
		}
	}

	// Check diagonal wins (top-left to bottom-right)
	for (int row = 0; row <= BOARD_SIZE - 4; ++row) {
		for (int col = 0; col <= BOARD_SIZE - 4; ++col) {
			Player firstPlayer = state.grid[row][col].getOwner();
			if (firstPlayer == Player::NoPlayer) continue;

			bool isWin = true;
			for (int i = 1; i < 4; ++i) {
				if (state.grid[row + i][col + i].getOwner() != firstPlayer) {
					isWin = false;
					break;
				}
			}
			if (isWin) {
				winner = firstPlayer;
				return true;
			}
		}
	}

	// Check diagonal wins (bottom-left to top-right)
	for (int row = 3; row < BOARD_SIZE; ++row) {
		for (int col = 0; col <= BOARD_SIZE - 4; ++col) {
			Player firstPlayer = state.grid[row][col].getOwner();
			if (firstPlayer == Player::NoPlayer) continue;

			bool isWin = true;
			for (int i = 1; i < 4; ++i) {
				if (state.grid[row - i][col + i].getOwner() != firstPlayer) {
					isWin = false;
					break;
				}
			}
			if (isWin) {
				winner = firstPlayer;
				return true;
			}
		}
	}

	return false;
}

// =============================================================================
// HELPER FUNCTION: Check if position is valid
// =============================================================================
bool Gameplay::isValidPosition(int row, int col) const
{
	return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}
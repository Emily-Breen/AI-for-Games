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
			if (state.grid[row][col].owner ==Player::NoPlayer) continue;

			if (state.grid[row][col].owner == maximizingPlayer) {
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
		state.grid[checkRow][checkCol].owner == player)  // ✔ owner check replaces both old checks
	{
		count++;
		checkRow += deltaRow;
		checkCol += deltaCol;
	}

	return count;
}

PieceState Gameplay::toPieceState(const Animal& a)
{
	PieceState ps;
	ps.owner = a.getOwner();
	ps.type = a.getType();
	return ps;
}

Animal Gameplay::toAnimal(const PieceState& ps)
{
	return Animal(ps.owner, ps.type);
}

// =============================================================================
// GENERATE ALL POSSIBLE MOVES
// =============================================================================
std::vector<Move> Gameplay::generateMoves(const Boardstate& state)
{
	std::vector<Move> moves;

	// Look at all board positions
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			const PieceState& piece = state.grid[row][col];

			// Skip empty cells
			if (piece.owner == Player::NoPlayer)
				continue;

			// Skip opponent's pieces
			if (piece.owner != state.currentPlayer)
				continue;

			// Get moves for this piece (Frog, Snake, Donkey)
			std::vector<Move> pieceMoves = getValidMovesForPiece(row, col, state);

			// Append them
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
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
	newState.grid[move.row1][move.col1] = { Player::NoPlayer, AnimalType::NoType };  // Empty animal

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
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; ++col)
		{
			Player first = state.grid[row][col].owner;
			if (first == Player::NoPlayer) continue;

			bool ok = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row][col + i].owner != first)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				winner = first;
				return true;
			}
		}
	}

	// Check vertical wins
	for (int col = 0; col < BOARD_SIZE; ++col)
	{
		for (int row = 0; row <= BOARD_SIZE - 4; ++row)
		{
			Player first = state.grid[row][col].owner;
			if (first == Player::NoPlayer) continue;

			bool ok = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row + i][col].owner != first)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				winner = first;
				return true;
			}
		}
	}

	// Check diagonal wins (top-left to bottom-right)
	for (int row = 0; row <= BOARD_SIZE - 4; ++row)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; ++col)
		{
			Player first = state.grid[row][col].owner;
			if (first == Player::NoPlayer) continue;

			bool ok = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row + i][col + i].owner != first)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				winner = first;
				return true;
			}
		}
	}
	// Check diagonal wins (bottom-left to top-right)
	for (int row = 3; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; ++col)
		{
			Player first = state.grid[row][col].owner;
			if (first == Player::NoPlayer) continue;

			bool ok = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row - i][col + i].owner != first)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				winner = first;
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

std::vector<Move> Gameplay::getValidMovesForPiece(int row, int col, const Boardstate& state)
{
	std::vector<Move> moves;

	Player owner = state.grid[row][col].owner;
	AnimalType type = state.grid[row][col].type;

	if (owner == Player::NoPlayer)
		return moves;

	static const int DIR4[4][2] = {
		{1,0}, {-1,0}, {0,1}, {0,-1}
	};

	static const int DIR8[8][2] = {
		{1,0}, {-1,0}, {0,1}, {0,-1},
		{1,1}, {1,-1}, {-1,1}, {-1,-1}
	};

	// Donkey moves (4 directions, 1 step)
	if (type == AnimalType::Donkey)
	{
		for (auto& d : DIR4)
		{
			int r = row + d[0];
			int c = col + d[1];
			if (isValidPosition(r, c) && state.grid[r][c].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, r, c });
			}
		}
	}

	// Snake moves (8 directions, 1 step)
	else if (type == AnimalType::Snake)
	{
		for (auto& d : DIR8)
		{
			int r = row + d[0];
			int c = col + d[1];
			if (isValidPosition(r, c) && state.grid[r][c].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, r, c });
			}
		}
	}

	// Frog moves
	else if (type == AnimalType::Frog)
	{
		// Normal 1-step moves
		for (auto& d : DIR8)
		{
			int r = row + d[0];
			int c = col + d[1];
			if (isValidPosition(r, c) && state.grid[r][c].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, r, c });
			}
		}

		// Long jumps
		for (auto& d : DIR8)
		{
			int r = row + d[0];
			int c = col + d[1];

			// Jump over occupied pieces
			while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
				state.grid[r][c].owner != Player::NoPlayer)
			{
				r += d[0];
				c += d[1];
			}

			if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
				state.grid[r][c].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, r, c });
			}
		}
	}

	return moves;
}

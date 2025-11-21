#include "Gameplay.h"
#include <algorithm>
#include <iostream>

Gameplay::Gameplay() : m_maximizingPlayer(Player::Player2), m_nodesEvaluated(0)
{
}

// Primary function to choose the best move using minimax with alpha-beta pruning
Move Gameplay::chooseBestMove(const Boardstate& state, int depth)
{
	m_nodesEvaluated = 0;
	m_maximizingPlayer = state.currentPlayer;

	std::vector<Move> possibleMoves = generateMoves(state);

	// Limit number of moves to evaluate for performance
	if (possibleMoves.size() > 10) {
		possibleMoves.resize(10);
	}

	if (possibleMoves.empty()) {
		std::cout << "No valid moves available!\n";
		return Move();
	}

	// Reset variables here to reassess best move each time this function is called
	Move bestMove;
	int bestScore = -UNLIMITED_POWER;
	int alpha = -UNLIMITED_POWER;
	int beta = UNLIMITED_POWER;

	std::cout << "AI evaluating " << possibleMoves.size() << " possible moves...\n";

	// Try each possible move and evaluate it
	for (const Move& move : possibleMoves) {

		// Apply the move to get a new board state
		Boardstate newState = makeMove(state, move);

		// Evaluate this move using minimax (opponent's turn, so minimizing)
		int score = miniMax(newState, depth, false, alpha, beta);

		std::cout << "Move (" << move.col1 << "," << move.row1 << ") -> (" << move.col2 << "," << move.row2 << ") scored: " << score << "\n";

		// Keep track of best move
		if (score >= bestScore) {
			bestScore = score;
			bestMove = move;
		}

		alpha = std::max(alpha, score);
	}

	std::cout << "AI chose move with score " << bestScore << " (evaluated " << m_nodesEvaluated << " nodes)\n";

	return bestMove;
}

int Gameplay::miniMax(const Boardstate& state, int depth, bool isMaximizing, int alpha, int beta)
{
	m_nodesEvaluated++;

	// Check if game is over before recursing
	Player winner;
	if (checkWimCondition(state, winner)) {
		if (winner == m_maximizingPlayer) {
			return UNLIMITED_POWER;		 // AI wins
		}
		else {
			return -UNLIMITED_POWER;	 // AI loses
		}
	}

	// Maximum depth reached, stop recursion
	if (depth == 0) {
		return evaluateBoard(state, m_maximizingPlayer);
	}

	// Generate all possible moves for current player
	std::vector<Move> possibleMoves = generateMoves(state);

	// Evaluate all possible moves
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
				break; // Beta cutoff - prune the rest of the branches
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
				break; // Alpha cutoff - prune the rest of the branches
			}
		}

		return minEval;
	}
}

int Gameplay::evaluateBoard(const Boardstate& state, Player maximizingPlayer)
{
	int score = 0;

	// Valid tiles closer to the center should be more valuable than edge tiles
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			if (state.grid[row][col].owner == Player::NoPlayer) continue;

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

// Convert Animal to PieceState for board representation
PieceState Gameplay::toPieceState(const Animal& animal)
{
	PieceState pieceState;
	pieceState.owner = animal.getOwner();
	pieceState.type = animal.getType();
	return pieceState;
}

Animal Gameplay::toAnimal(const PieceState& pieceState)
{
	return Animal(pieceState.owner, pieceState.type);
}


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

bool Gameplay::checkWimCondition(const Boardstate& state, Player& winner)
{
	// Check horizontal wins
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; ++col)
		{
			Player first = state.grid[row][col].owner;
			if (first == Player::NoPlayer) continue;

			bool aboutToWin = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row][col + i].owner != first)
				{
					aboutToWin = false;
					break;
				}
			}
			if (aboutToWin)
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

			bool aboutToWin = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row + i][col].owner != first)
				{
					aboutToWin = false;
					break;
				}
			}
			if (aboutToWin)
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

			bool aboutToWin = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row + i][col + i].owner != first)
				{
					aboutToWin = false;
					break;
				}
			}
			if (aboutToWin)
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

			bool aboutToWin = true;
			for (int i = 1; i < 4; ++i)
			{
				if (state.grid[row - i][col + i].owner != first)
				{
					aboutToWin = false;
					break;
				}
			}
			if (aboutToWin)
			{
				winner = first;
				return true;
			}
		}
	}

	return false;
}

// function to check if a position is within board bounds
bool Gameplay::isValidPosition(int row, int col) const
{
	return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// check all valid moves for a piece at (row, col)
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
		for (auto& direction : DIR4)
		{
			int neighbourRow = row + direction[0];
			int neighbourCol = col + direction[1];
			if (isValidPosition(neighbourRow, neighbourCol) && state.grid[neighbourRow][neighbourCol].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, neighbourRow, neighbourCol });
			}
		}
	}

	// Snake moves (8 directions, 1 step)
	else if (type == AnimalType::Snake)
	{
		for (auto& direction : DIR8)
		{
			int neighbourRow = row + direction[0];
			int neighbourCol = col + direction[1];
			if (isValidPosition(neighbourRow, neighbourCol) && state.grid[neighbourRow][neighbourCol].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, neighbourRow, neighbourCol });
			}
		}
	}

	// Frog moves
	else if (type == AnimalType::Frog)
	{
		// Normal 1-step moves
		for (auto& direction : DIR8)
		{
			int neighbourRow = row + direction[0];
			int neighbourCol = col + direction[1];
			if (isValidPosition(neighbourRow, neighbourCol) && state.grid[neighbourRow][neighbourCol].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, neighbourRow, neighbourCol });
			}
		}

		// Long jumps
		for (auto& direction : DIR8)
		{
			int neighbourRow = row + direction[0];
			int neighbourCol = col + direction[1];

			// Jump over occupied pieces; while within bounds and next cell is occupied
			while (neighbourRow >= 0 && neighbourRow < BOARD_SIZE && neighbourCol >= 0 && neighbourCol < BOARD_SIZE &&
				state.grid[neighbourRow][neighbourCol].owner != Player::NoPlayer)
			{
				neighbourRow += direction[0];
				neighbourCol += direction[1];
			}

			// if we land on an empty cell, it's a valid move
			if (neighbourRow >= 0 && neighbourRow < BOARD_SIZE && neighbourCol >= 0 && neighbourCol < BOARD_SIZE &&
				state.grid[neighbourRow][neighbourCol].owner == Player::NoPlayer)
			{
				moves.push_back({ row, col, neighbourRow, neighbourCol });
			}
		}
	}

	return moves;
}

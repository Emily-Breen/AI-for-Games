#include "Gameplay.h"

Gameplay::Gameplay()
{
}

Move Gameplay::chooseBestMove(const Boardstate& state)
{
	return Move();
}
//I realise I have spelled Win as Wim and I am not sorry, it sounds fun I'm a wimmer :D -EB
bool Gameplay::checkWimCondition(const Boardstate& state, Player& winner)
{
	return false;
}

int Gameplay::miniMax(Boardstate state, int depth, bool isMaximizing, int alpha, int beta)
{
	return 0;
}

int Gameplay::evaluateBoard(const Boardstate& state)
{
	return 0;
}

std::vector<Move> Gameplay::generateMoves(const Boardstate& state)
{
	return std::vector<Move>();
}

Boardstate Gameplay::makeMove(const Boardstate& state, const Move& move)
{
	return Boardstate();
}

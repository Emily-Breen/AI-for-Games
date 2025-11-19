#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.h"
#include <vector>

const int BOARD_SIZE = 5;
class Board
{
public:
	Board(int size, float cellSize);

	void draw(sf::RenderWindow& window);
	sf::Vector2f getCellCenter(int row, int col) const;
	void recalculateGrid(const sf::RenderWindow& m_window);
	void updateCellSize(sf::Vector2u windowSize);
	int getSize() const;
	float getCellSize() const;

private:

	int m_size;
	float m_cellSize;
	float m_offsetX{ 0.f };
	float m_offsetY{ 0.f };
	void drawGrid(sf::RenderWindow& window);


};


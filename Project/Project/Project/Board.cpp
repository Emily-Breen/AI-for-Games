#include "Board.h"


//Hi Stephen I've marked a few functions with summaries to help explain what they do its mostly because when the window resizes it goes all over the place :( so its sorted now!

Board::Board(int size, float cellSize) : m_size(size), m_cellSize(cellSize)
{

	
}

void Board::draw(sf::RenderWindow& window)
{
	recalculateGrid(window);
	drawGrid(window);
}
/// <summary>
/// function to get the center position of a cell given its row and column
/// </summary>
/// <param name="row"></param>
/// <param name="col"></param>
/// <returns></returns>
sf::Vector2f Board::getCellCenter(int row, int col) const
{
	return { m_offsetX + col * m_cellSize + m_cellSize / 2.f,
			m_offsetY + row * m_cellSize + m_cellSize / 2.f };
}
/// <summary>
/// function to recalculate grid offsets based on window size
/// </summary>
/// <param name="m_window"></param>
void Board::recalculateGrid(const sf::RenderWindow& m_window)
{
	float gridSize = m_size * m_cellSize;
	m_offsetX = (m_window.getSize().x - gridSize) / 2.f;
	m_offsetY = (m_window.getSize().y - gridSize) / 2.f;
}
/// <summary>
/// function to update cell size based on window size
/// </summary>
/// <param name="windowSize"></param>
void Board::updateCellSize(sf::Vector2u windowSize)
{
	float minDimension = std::min(windowSize.x, windowSize.y);
	m_cellSize = (minDimension * 0.7f) / m_size;
}

int Board::getSize() const
{
	return m_size;
}

float Board::getCellSize() const
{
	return m_cellSize;
}

void Board::drawGrid(sf::RenderWindow& m_window)
{
	sf::RectangleShape cell(sf::Vector2f(m_cellSize - 2, m_cellSize - 2));
	cell.setFillColor(sf::Color::Transparent);
	cell.setOutlineColor(sf::Color::White);
	cell.setOutlineThickness(1.0f);

	for (int row = 0; row < m_size; ++row) {
		for (int col = 0; col < m_size; ++col) {
			cell.setPosition(sf::Vector2f(
				m_offsetX + col * m_cellSize + 1,
				m_offsetY + row * m_cellSize + 1
			));
			m_window.draw(cell);
		}
	}
}



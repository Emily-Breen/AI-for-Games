#include "FlowField.h"

FlowField::FlowField()
{
	m_grid.setPrimitiveType(sf::PrimitiveType::Lines);
	makeGrid();
}

void FlowField::makeGrid()
{
	int cols = 50;
	int rows = 50;
	float screenWidth = 1920;
	float screenHeight = 1920;

	float dx = screenWidth / cols;
	float dy = screenHeight / rows;

	sf::Color lineColor(sf::Color::Cyan);

	for (int i = 0; i <= cols; ++i) {
		float xLines = i * dx;
		m_grid.append(sf::Vertex{ sf::Vector2f(xLines, 0.f), lineColor });
		m_grid.append(sf::Vertex{ sf::Vector2f(xLines, screenHeight),lineColor });
	}

	for(int j = 0; j <= rows; ++j) {
		float yLines = j * dy;
		m_grid.append(sf::Vertex{ sf::Vector2f(0.f, yLines), lineColor });
		m_grid.append(sf::Vertex{ sf::Vector2f(screenWidth, yLines), lineColor });
	}
}

void FlowField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_grid, states);
}

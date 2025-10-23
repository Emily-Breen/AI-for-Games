#include "FlowField.h"

FlowField::FlowField()
{
	m_grid.setPrimitiveType(sf::PrimitiveType::Lines);
	makeGrid();

	m_circle.setRadius(8.f);
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setOrigin(sf::Vector2f(m_circle.getRadius(), m_circle.getRadius()));

	m_goalCircle.setRadius(8.f);
	m_goalCircle.setFillColor(sf::Color::Green);
	m_goalCircle.setOrigin(sf::Vector2f(m_goalCircle.getRadius(), m_goalCircle.getRadius()));
	

	float dx = screenWidth / cols;
	float dy = screenHeight / rows;

	float centerX = (cellX + 0.5f) * dx;
	float centerY = (cellY + 0.5f) * dy;
	float goalCenterX = (cellGoalX + 0.5f) * dx;
	float goalCenterY = (cellGoalY + 0.5f) * dy;

	m_circle.setPosition(sf::Vector2f(centerX,centerY));
	m_goalCircle.setPosition(sf::Vector2f(goalCenterX, goalCenterY));

}

void FlowField::update(float dt)
{
}

void FlowField::makeGrid()
{
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

void FlowField::draw(sf::RenderWindow& m_window)
{
	m_window.draw(m_grid);
	m_window.draw(m_circle);
	m_window.draw(m_goalCircle);
}

sf::Vector2f FlowField::getDirectionsAtPosition(sf::Vector2f position)
{
	return sf::Vector2f();
}

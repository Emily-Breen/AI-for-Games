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

	m_goal = m_goalCircle.getPosition();

	makeDirections();

	if(m_font.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "Font loaded successfully\n";
	}
	// Create   cost text for each cell
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			
			costText.setFont(m_font);
			costText.setCharacterSize(12);
			costText.setFillColor(sf::Color::White);
			float centerX = (x + 0.5f) * cellSize;
			float centerY = (y + 0.5f) * cellSize;
			costText.setPosition(sf::Vector2f(centerX - 10.f, centerY - 10.f));
			float cost = MathUtils::vectorLength(m_goal - sf::Vector2f(centerX, centerY));
			costText.setString(std::to_string(static_cast<int>(cost)));
			m_costTexts.push_back(costText);
		}
	}

	

	

}

void FlowField::update(float dt)
{
	sf::Vector2f position = m_circle.getPosition();
	sf::Vector2f direction = m_goal - position;

	direction = MathUtils::normalize(direction);

	float speed = 100.f;
	m_circle.move(direction * speed * dt);

	float distanceToGoal = MathUtils::vectorLength(m_goal - position);
	if (distanceToGoal < 5.f) {
		m_circle.setFillColor(sf::Color::Blue);
	}
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

	for (auto& text : m_costTexts)
	{
		m_window.draw(text);
	}
}

sf::Vector2f FlowField::getDirectionsAtPosition(sf::Vector2f position)
{
	return sf::Vector2f();
}

void FlowField::makeDirections()
{

	m_directions.resize(cols * rows);
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			sf::Vector2f center((x + 0.5f) * cellSize, (y + 0.5f) * cellSize);
			sf::Vector2f direction = m_goal - center;

			direction = MathUtils::normalize(direction);
			m_directions[y * cols + x] = direction;
		}
	}
}

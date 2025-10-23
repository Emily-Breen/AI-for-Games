#pragma once
#include <SFML/Graphics.hpp>

class FlowField
{
public:
	FlowField();
	void update(float dt);
	void draw(sf::RenderWindow& m_window);

private:
	sf::VertexArray m_grid;
	void makeGrid();
	
	sf::Vector2f getDirectionsAtPosition(sf::Vector2f position);
	float cellSize = 21.6f;
	float screenWidth = 1080;
	float screenHeight = 1080;
	int cellX = 5;
	int cellY = 5;
	int cellGoalX = 45;
	int cellGoalY = 45;
	int cols = 50;
	int rows = 50;
	float dx;
	float dy;
	sf::CircleShape m_circle;
	sf::CircleShape m_goalCircle;
	sf::Vector2f m_goal;




};


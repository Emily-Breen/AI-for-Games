#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include "MathUtils.h"

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
	void makeDirections();
	
	sf::Vector2f m_goal;
	std::vector<float> m_costs;
	std::vector<sf::Vector2f> m_directions;
	//floats
	float cellSize = 21.6f;
	float screenWidth = 1080;
	float screenHeight = 1080;
	float dx;
	float dy;
	//ints 
	int cellX = 5;
	int cellY = 5;
	int cellGoalX = 45;
	int cellGoalY = 45;
	int cols = 50;
	int rows = 50;
	
	//shapes and fonts for demostration
	sf::CircleShape m_circle;
	sf::CircleShape m_goalCircle;
	sf::Font m_font;
	std::vector<sf::Text> m_costTexts;
	sf::Text costText{ m_font };

	




};


#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include "MathUtils.h"
#include "UI.h"


class FlowField
{
public:
	FlowField();

	void update(float dt);
	void draw(sf::RenderWindow& m_window);
	void drawVectorField(sf::RenderWindow& m_window);
	void drawHeatMap(sf::RenderWindow& m_window);
	void drawObstacles(sf::RenderWindow& m_window);
	void toggleVectorField();
	void toggleMovement();
	void toggleObstacleAtPosition(sf::Vector2f position);
	void togglePathfinding();
	void toggleHeatMap();
	void setStartPosition(sf::Vector2f position);
	void setGoalPosition(sf::Vector2f position);
	void updatePath();
	void drawPath(sf::RenderWindow& m_window);
	void resetPath();
	void updateCostTexts();
	void setFont(sf::Font& font);
	bool fieldNeedsUpdate = true;
	bool spawnObstacle = false;
	bool isUsingAStar();
	

private:
	
	void makeGrid();
	sf::Vector2f getDirectionsAtPosition(sf::Vector2f position);
	std::vector<sf::Vector2f> getNeighbours(sf::Vector2f cell);
	float heuristic(sf::Vector2f from, sf::Vector2f to);
	void makeCostsField();
	void makeIntegrationFieldBFS();
	void makeIntegrationFieldAStar();
	void makeFlowField();
	void resetField();
	
	

	

	sf::VertexArray m_grid;
	sf::Vector2f m_goal;
	std::vector<float> m_costs;
	std::vector<float> m_integration;
	std::vector<sf::Vector2f> m_directions;
	std::vector<sf::Vector2f> m_path;

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
	
	//shapes and fonts 
	sf::CircleShape m_circle;
	sf::CircleShape m_goalCircle;
	sf::RectangleShape tile;

	sf::Font* m_font = nullptr;
	std::vector<sf::Text> m_costTexts;


	bool showArrows = false;
	bool movementEnabled = false;
	bool showHeatMap = false;
	bool useAStar = false;
	
	struct Node {
		sf::Vector2f cell;
		float gCost;
		float hCost;
		float fCost;

		Node(sf::Vector2f c, float g, float h)
			: cell(c), gCost(g), hCost(h), fCost(g + h) {}

		bool operator>(const Node& other) const {
			return fCost > other.fCost;
		}

	};




};


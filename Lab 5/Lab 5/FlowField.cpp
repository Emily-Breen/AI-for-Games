#include "FlowField.h"
#include <queue>

FlowField::FlowField()
{
	m_grid.setPrimitiveType(sf::PrimitiveType::Lines);
	makeGrid();

	m_circle.setRadius(8.f);
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setOutlineColor(sf::Color::Black);
	m_circle.setOutlineThickness(2.f);
	m_circle.setOrigin(sf::Vector2f(m_circle.getRadius(), m_circle.getRadius()));

	m_goalCircle.setRadius(8.f);
	m_goalCircle.setFillColor(sf::Color::Green);
	m_goalCircle.setOutlineColor(sf::Color::Black);
	m_goalCircle.setOutlineThickness(2.f);
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

	makeCostsField();
	makeIntegrationFieldBFS();
	makeFlowField();

}

void FlowField::update(float dt)
{
	if (fieldNeedsUpdate)
	{
		makeIntegrationFieldBFS();
		makeFlowField();
		updateCostTexts();
		fieldNeedsUpdate = false;
	}

	if (!movementEnabled)
		return;

	sf::Vector2f position = m_circle.getPosition();

	
	int cellX = static_cast<int>(position.x / cellSize);
	int cellY = static_cast<int>(position.y / cellSize);
	sf::Vector2f cellCenter((cellX + 0.5f) * cellSize, (cellY + 0.5f) * cellSize);

	
	sf::Vector2f direction = getDirectionsAtPosition(position);
	if (direction.x == 0.f && direction.y == 0.f)
		return;

	direction = MathUtils::normalize(direction);
	float speed = 100.f;
	sf::Vector2f nextPos = position + direction * speed * dt;

	
	int nextCellX = static_cast<int>(nextPos.x / cellSize);
	int nextCellY = static_cast<int>(nextPos.y / cellSize);
	sf::Vector2f nextCellCenter((nextCellX + 0.5f) * cellSize, (nextCellY + 0.5f) * cellSize);

	
	if (cellX != nextCellX || cellY != nextCellY)
		m_circle.setPosition(nextCellCenter);
	else
		m_circle.setPosition(nextPos);

	updatePath();

	
	float distanceToGoal = MathUtils::vectorLength(m_goal - m_circle.getPosition());
	if (distanceToGoal <= 5.f)
	{
		m_circle.setFillColor(sf::Color::Blue);
		movementEnabled = false;
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
	drawObstacles(m_window);
	
	if (showHeatMap)
	{
		drawHeatMap(m_window);
	}
	drawPath(m_window);
	m_window.draw(m_grid);
	if (!showArrows) {


		for (auto& text : m_costTexts)
		{
			m_window.draw(text);
		}
	}
	else {
		drawVectorField(m_window);

	}

	m_window.draw(m_goalCircle);
	m_window.draw(m_circle);
	

	
}

void FlowField::drawVectorField(sf::RenderWindow& m_window)
{
	sf::VertexArray arrows(sf::PrimitiveType::Lines);

	const float arrowLen = cellSize * 0.4f;
	const float arrowSize = cellSize * 0.3f;
	const float angle = 3.14159f * 3.f / 4.f;

	const float cosA = cos(angle);
	const float sinA = sin(angle);

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			int index = y * cols + x;
			sf::Vector2f direction = m_directions[index];

			if (direction.x == 0.f && direction.y == 0.f)
				continue;

			sf::Vector2f center((x + 0.5f) * cellSize, (y + 0.5f) * cellSize);
			sf::Vector2f endPoint = center + direction * arrowLen;

			
			arrows.append({ center, sf::Color::Yellow });
			arrows.append({ endPoint, sf::Color::Yellow });

			
			sf::Vector2f leftArrowHead(
				direction.x * cosA - direction.y * sinA,
				direction.x * sinA + direction.y * cosA
			);
			sf::Vector2f rightArrowHead(
				direction.x * cosA + direction.y * sinA,
				-direction.x * sinA + direction.y * cosA
			);

			sf::Vector2f leftPoint = endPoint + MathUtils::normalize(leftArrowHead) * (arrowSize * 0.5f);
			sf::Vector2f rightPoint = endPoint + MathUtils::normalize(rightArrowHead) * (arrowSize * 0.5f);

			
			arrows.append({ endPoint, sf::Color::Yellow });
			arrows.append({ leftPoint, sf::Color::Yellow });
			arrows.append({ endPoint, sf::Color::Yellow });
			arrows.append({ rightPoint, sf::Color::Yellow });
		}
	}

	m_window.draw(arrows);
}
void FlowField::drawPath(sf::RenderWindow& m_window)
{
	sf::RectangleShape pathTile(sf::Vector2f(cellSize, cellSize));
	pathTile.setFillColor(sf::Color(sf::Color::Magenta));
	if (showHeatMap ==true) {
		pathTile.setFillColor(sf::Color(sf::Color::Cyan));
	}
	for (const auto& cell : m_path)
	{
		pathTile.setPosition(sf::Vector2f(cell.x * cellSize, cell.y * cellSize));
		m_window.draw(pathTile);
	}
}
void FlowField::drawHeatMap(sf::RenderWindow& m_window)
{
	sf::RectangleShape tile(sf::Vector2f(cellSize, cellSize));

	float maxIntegration = 1.f;

	for (int i = 0; i < (int)m_integration.size(); ++i)
		if (m_costs[i] < 9999.f && m_integration[i] > maxIntegration)
			maxIntegration = m_integration[i];
	if (maxIntegration < 1.f) maxIntegration = 1.f;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			int index = y * cols + x;

			
				float value = m_integration[index];
				float heatFactor = std::min(value / maxIntegration, 1.f); 

				
				float r = 255.f * heatFactor;
				float g = 0.f;
				float b = 255.f * (1.f - heatFactor);

				tile.setFillColor(sf::Color(r, g, b));
			

			tile.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
			m_window.draw(tile);
		}
	}
}

void FlowField::drawObstacles(sf::RenderWindow& m_window)
{
	sf::RectangleShape obstacleTile(sf::Vector2f(cellSize, cellSize));
	obstacleTile.setFillColor(sf::Color::Black);

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			int index = y * cols + x;
			if (m_costs[index] >= 9999.f)
			{
				obstacleTile.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
				m_window.draw(obstacleTile);
			}
		}
	}
}

void FlowField::toggleVectorField()
{
	showArrows = !showArrows;
	if (!showArrows)
		updateCostTexts();
	
}

void FlowField::toggleMovement()
{
	movementEnabled = !movementEnabled;

}
void FlowField::toggleObstacleAtPosition(sf::Vector2f position)
{
	int x = position.x / cellSize;
	int y = position.y / cellSize;
	if (x < 0 || x >= cols || y < 0 || y >= rows)
		return;

	int index = y * cols + x;
	m_costs[index] = (m_costs[index] == 1.f) ? 9999.f : 1.f;

	
	fieldNeedsUpdate = true;

}

void FlowField::togglePathfinding()
{
	useAStar = !useAStar;
	makeIntegrationFieldBFS();
	makeFlowField();
	updateCostTexts();
}
void FlowField::toggleHeatMap()
{
	showHeatMap = !showHeatMap;

}


std::vector<sf::Vector2f> FlowField::getNeighbours(sf::Vector2f cell)
{

	std::vector<sf::Vector2f> neighbours;

	for (int j = -1; j <= 1; ++j)
	{
		for (int i = -1; i <= 1; ++i)
		{
			if (i == 0 && j == 0) {
				continue;
			}

			int neighbourX = static_cast<int>(cell.x) + i;
			int neighbourY = static_cast<int>(cell.y) + j;

			if (neighbourX < 0 || neighbourX >= cols || neighbourY < 0 || neighbourY >= rows) {
				continue;
			}

			if (std::abs(i) == 1 && std::abs(j) == 1)
			{
				int adjacent1 = neighbourY * cols + static_cast<int>(cell.x);
				int adjacent2 = static_cast<int>(cell.y) * cols + neighbourX;
				if (m_costs[adjacent1] >= 9999.f || m_costs[adjacent2] >= 9999.f)
					continue;
			}

			neighbours.push_back(sf::Vector2f(static_cast<float>(neighbourX), static_cast<float>(neighbourY)));
		}
	}

	return neighbours;
}

float FlowField::heuristic(sf::Vector2f from, sf::Vector2f to)
{
	sf::Vector2f diff = to - from;
	return MathUtils::vectorLength(diff) * cellSize;
}

sf::Vector2f FlowField::getDirectionsAtPosition(sf::Vector2f position)
{
	int x = (int)(position.x / cellSize);
	int y = (int)(position.y / cellSize);
	if (x < 0 || x >= cols || y < 0 || y >= rows)
	{
		return sf::Vector2f(0.f, 0.f);
	}

	int index = y * cols + x;
	return m_directions[index];
}


//movement cost field for each cell in the grid
void FlowField::makeCostsField()
{
	m_costs.resize(cols * rows, 1.f);

	for (int y = 10; y < 15; ++y) {
		for (int x = 10; x < 15; ++x) {
			m_costs[y * cols + x] = 8.f;
		}
	}
}

void FlowField::makeIntegrationFieldBFS()
{
	if (useAStar)
	{
		makeIntegrationFieldAStar();
		std::cout << " Using A*";
	}
	else{
		std::cout << "Using BFS";
		m_integration.assign(cols * rows, 999999.f);

		int goalIndex = cellGoalY * cols + cellGoalX;
		if (goalIndex < 0 || goalIndex >= cols * rows)
			return;

		m_integration[goalIndex] = 0.f;

		std::queue<sf::Vector2f> open;
		open.push(sf::Vector2f(static_cast<float>(cellGoalX), static_cast<float>(cellGoalY)));

		while (!open.empty())
		{
			auto current = open.front();
			open.pop();
			int currentIndex = current.y * cols + current.x;

			for (auto& neighbor : getNeighbours(sf::Vector2f((float)current.x, (float)current.y)))
			{
				int neighbourX = (int)neighbor.x;
				int neighbourY = (int)neighbor.y;
				int neighbourIndex = neighbourY * cols + neighbourX;

				if (m_costs[neighbourIndex] >= 9999.f)
					continue;

				// Euclidean distance calculation
				sf::Vector2f offset = neighbor - current;
				float euclideanDistance = MathUtils::vectorLength(offset) * cellSize;

				float newCost = m_integration[currentIndex] + m_costs[neighbourIndex] + euclideanDistance;

				if (newCost < m_integration[neighbourIndex])
				{
					m_integration[neighbourIndex] = newCost;
					open.push(sf::Vector2f(static_cast<float>(neighbourX), static_cast<float>(neighbourY)));

				}
			}
		}
	}
	
}

void FlowField::makeIntegrationFieldAStar()
{
     m_integration.assign(cols * rows, 999999.f);
     int goalIndex = cellGoalY * cols + cellGoalX;
   if (goalIndex < 0 || goalIndex >= cols * rows)
	return;
   m_integration[goalIndex] = 0.f;

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
  std::vector<bool> closed(cols * rows, false);
  sf::Vector2f goalCell(static_cast<float>(cellGoalX), static_cast<float>(cellGoalY));
  sf::Vector2f startCell(static_cast<float>(cellX), static_cast<float>(cellY));

  float hCostStart = heuristic(goalCell,startCell);

  open.push(Node(goalCell, 0.f, hCostStart));

  while (!open.empty()) {
	  Node current = open.top();

	  open.pop();

	  int currentIndex = static_cast<int>(current.cell.y) * cols + static_cast<int>(current.cell.x);

	  if (closed[currentIndex])
		  continue;
	  closed[currentIndex] = true;

	  for (auto& neighbour : getNeighbours(current.cell)) {

		  int neighbourX = static_cast<int>(neighbour.x);
		  int neighbourY = static_cast<int>(neighbour.y);
		  int neighbourIndex = neighbourY * cols + neighbourX;

		  if (m_costs[neighbourIndex] >= 9999.f || closed[neighbourIndex]) {
			  continue;
		  }
		  sf::Vector2f offset = neighbour - current.cell;
		  float euclideanDistance = MathUtils::vectorLength(offset) * cellSize;
		  float newCost = m_integration[currentIndex] + m_costs[neighbourIndex] + euclideanDistance;

		  if (newCost < m_integration[neighbourIndex]) {
			  m_integration[neighbourIndex] = newCost;
			  float hCost = heuristic(neighbour, startCell);
			  open.push(Node(neighbour, newCost, hCost));
		  }
	  }
  }


}

void FlowField::makeFlowField()
{
	m_directions.resize(cols * rows, sf::Vector2f(0.f, 0.f));
	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < cols; ++x) {
			int index = y * cols + x;
			float lowestCost = m_integration[index];
			sf::Vector2f bestDirection(0.f, 0.f);
			std::vector<sf::Vector2f> neighbours = getNeighbours(sf::Vector2f((float)x, (float)y));
			for (int i = 0; i < neighbours.size(); ++i) {
				int neighbourX = (int)neighbours[i].x;
				int neighbourY = (int)neighbours[i].y;
				int neighbourIndex = neighbourY * cols + neighbourX;
				float value = m_integration[neighbourIndex];

				if (value < lowestCost) {

					lowestCost = value;
					sf::Vector2f current((x + 0.5f) * cellSize, (y + 0.5f) * cellSize);
					sf::Vector2f next((neighbourX + 0.5f) * cellSize, (neighbourY + 0.5f) * cellSize);
					bestDirection = MathUtils::normalize(next - current);
				}
			}
			m_directions[index] = bestDirection;
		}
}

void FlowField::setStartPosition(sf::Vector2f position)
{
	resetPath();
	int x = position.x / cellSize;
	int y = position.y / cellSize;
	cellX = x;
	cellY = y;
	float centerX = (cellX + 0.5f) * cellSize;
	float centerY = (cellY + 0.5f) * cellSize;
	m_circle.setPosition(sf::Vector2f(centerX, centerY));
	resetField();
}

void FlowField::setGoalPosition(sf::Vector2f position)
{
	resetPath();
	int x = position.x / cellSize;
	int y = position.y / cellSize;
	cellGoalX = x;
	cellGoalY = y;

	float goalCenterX = (cellGoalX + 0.5f) * cellSize;
	float goalCenterY = (cellGoalY + 0.5f) * cellSize;
	m_goalCircle.setPosition(sf::Vector2f(goalCenterX, goalCenterY));
	m_goal = m_goalCircle.getPosition();

	makeIntegrationFieldBFS();   
	makeFlowField();        
	updateCostTexts();        
}

void FlowField::updatePath()
{
	float cellXf = m_circle.getPosition().x / cellSize;
	float cellYf = m_circle.getPosition().y / cellSize;
	sf::Vector2f currentCell{ std::floor(cellXf), std::floor(cellYf) };
	if (m_path.empty() || m_path.back() != currentCell)
	{
		m_path.push_back(currentCell);
	}
}

void FlowField::updateCostTexts()
{
	if (m_font == nullptr)
		return;  

	m_costTexts.clear();

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			
			sf::Text costText(*m_font);
			costText.setCharacterSize(12);
			costText.setFillColor(sf::Color::White);

			float centerX = (x + 0.5f) * cellSize;
			float centerY = (y + 0.5f) * cellSize;
			costText.setPosition(sf::Vector2f(centerX - 10.f, centerY - 10.f));

			int index = y * cols + x;
			float value = m_integration[index];
			if (m_costs[index] >= 9999.f)
			{
				costText.setString("X");
			}
			else
			{
				costText.setString(std::to_string((int)value));
			}
			m_costTexts.push_back(costText);
		}
	}
}

void FlowField::setFont(sf::Font& font)
{
	m_font = &font;
	m_costTexts.clear();
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			sf::Text costText{ *m_font };
			
			costText.setCharacterSize(12);
			costText.setFillColor(sf::Color::White);

			float centerX = (x + 0.5f) * cellSize;
			float centerY = (y + 0.5f) * cellSize;
			costText.setPosition(sf::Vector2f(centerX - 10.f, centerY - 10.f));

			int index = y * cols + x;
			costText.setString(std::to_string((int)m_integration[index]));
			m_costTexts.push_back(costText);
		}
	}
}

bool FlowField::isUsingAStar()
{
	return useAStar;
}

void FlowField::resetField()
{
	makeIntegrationFieldBFS();
	makeFlowField();
	updateCostTexts();
	m_circle.setFillColor(sf::Color::Red);

}
void FlowField::resetPath()
{
	m_path.clear();
}
#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

enum Player {
	NoPlayer,
	Player1,
	Player2
};

enum AnimalType {
	NoType,
	Frog,
	Snake,
	Donkey
};
class Animal
{
  public:
	  Animal(); // default
	  Animal(Player owner, AnimalType type); //param constructor
	  
	  std::string getName() const;
	  bool isEmpty() const;
	  Player getOwner() const;
	  AnimalType getType() const;
	  void setPosition(const sf::Vector2f& position);
	  void initAnimalTexture(float cellSize);
	  void draw(sf::RenderWindow& m_window);
	  void rescale(float cellSize);
	  sf::FloatRect getBounds() const;
	  Animal(const Animal& other);
	  Animal& operator=(const Animal& other);

	  // New method to get valid moves for the current animal from a given position
	  std::vector<sf::Vector2i> getValidMoves(int currentRow, int currentCol,
		  const Animal* grid, int boardSize) const;

private:
	Player m_owner;
	AnimalType m_type;
	sf::Texture m_texture;
	sf::Sprite m_sprite{ m_texture };

	// Helper methods for movement validation
	bool isValidPosition(int row, int col, int boardSize) const;

	// Adds cardinal movements (up, down, left, right) to the moves vector
	void addAdjacentMoves(int row, int col, const Animal* grid, int boardSize,
		std::vector<sf::Vector2i>& moves, bool includeDiagonal) const;
	void addFrogJumpMoves(int row, int col, const Animal* grid, int boardSize,
		std::vector<sf::Vector2i>& moves) const;

};


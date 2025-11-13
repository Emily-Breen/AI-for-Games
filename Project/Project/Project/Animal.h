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
	  void setPosition(const sf::Vector2f& position);
	  void initAnimalTexture(float cellSize);
	  void draw(sf::RenderWindow& m_window);
	  void rescale(float cellSize);

private:
	Player m_owner;
	AnimalType m_type;
	sf::Texture m_texture;
	sf::Sprite m_sprite{ m_texture };


};


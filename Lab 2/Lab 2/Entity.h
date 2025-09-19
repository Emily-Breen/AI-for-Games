#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:

	virtual ~Entity() = default;
	virtual void update(float dt) = 0;
	 
	virtual void draw(sf::RenderWindow t_window);
	virtual void wrapAroundScreen(float windowWidth, float windowHeight);

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	float m_speed;
	float m_maxSpeed;
	float m_minSpeed;

	int m_currentFrame = 0;
	int m_firstFrame = 0;
	int m_lastFrame = 3;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f;


};

#pragma once

#include <SFML/Graphics.hpp>


class Entity
{
public:
	Entity();
	virtual ~Entity() = default;
	virtual void update(float dt) = 0;
	 
	virtual void draw(sf::RenderWindow& t_window);
	virtual void wrapAroundScreen(float windowWidth, float windowHeight);

	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;
	virtual void updateAnimation(float dt);


	void setRotation(float angle);
	float getRotation() const;

protected:

	sf::Texture m_texture;
	sf::Sprite m_sprite{m_texture};

	int frameWidth;
	int frameHeight;
	int m_currentFrame = 0;
	int m_firstFrame = 0;
	int m_lastFrame = 3;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f;


};

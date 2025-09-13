#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class Player {
public:
	Player();

	void playerInit();

	void movePlayer(float dx, float dy);
	void moveForward(float dt);
	void drawPlayer(sf::RenderWindow& window);
	void updateAnimation(float dt);
	void accelerate(); 
	void decelerate(); 
	void applyFriction();
	void update(float dt);
	void wrapAroundScreen(float windowWidth, float windowHeight);
	void rotatePlayerShip(float angle);
private:
	//player variables
	sf::Texture m_texture;
	sf::Sprite m_playerSprite{ m_texture };
    float m_speed;
	float m_maxSpeed;
	float m_minSpeed;

	//animation variables
	int m_currentFrame = 6;
	int m_firstFrame = 6;
	int m_lastFrame = 11;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f; 

};
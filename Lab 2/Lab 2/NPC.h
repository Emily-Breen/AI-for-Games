#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class NPC {
public:
	NPC();
	void npcInit();
	void drawNPC(sf::RenderWindow& window);	
	sf::Vector2f getRandomDirection();
	void update(float dt);
	void updateAnimation(float dt);
	void wrapAroundScreen(float windowWidth, float windowHeight);
	void smoothRotate(float targetAngle, float rotationSpeed, float dt);
	void rotateNPCShip(float angle);

private:
    //npc variables 
	sf::Texture m_texture;
	sf::Sprite m_npcSprite{ m_texture };
	float m_speed;
	float m_maxSpeed;
	float m_minSpeed;
	float directionChangeTimer;
	float directionChangeInterval;
	sf::Vector2f currentDirection;
	float m_currentRotation;
	float m_rotationSpeed;
	float pivotOffset;

	//animation variables
	int m_currentFrame = 0;
	int m_firstFrame = 0;
	int m_lastFrame = 3;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f;

};
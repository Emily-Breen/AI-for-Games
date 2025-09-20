#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "MathUtils.h"
class Player :public Entity {
public:
	Player();

	void playerInit();

	void movePlayer(float dx, float dy);
	void moveForward(float dt);
	void updateAnimation(float dt) override;
	void accelerate(); 
	void decelerate(); 
	void applyFriction();
	void update(float dt) override;
private:
	SteeringOutput getSteering();
	//player variables
    float m_speed;
	float m_maxSpeed;
	float m_minSpeed;
	sf::Vector2f m_inputDirection{ 0.f, 0.f };
	//animation variables
	int frameWidth = 2400 / 24;
	int frameHeight = 100;
	int m_currentFrame = 6;
	int m_firstFrame = 6;
	int m_lastFrame = 11;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f; 

};
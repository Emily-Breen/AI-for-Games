#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "MathUtils.h"
class NPC : public Entity {
public:
	NPC();
	void npcInit();
	sf::Vector2f Wander();
	void update(float dt) override;
	void updateAnimation(float dt) override;
	void smoothRotate(float targetAngle, float rotationSpeed, float dt);
	void setTarget(const sf::Vector2f& targetPosition);

private:
	SteeringOutput getSteering();
    //npc variables 
	float m_speed;
	sf::Vector2f m_targetPosition;
	float m_maxSpeed;
	float m_minSpeed;
	float directionChangeTimer;
	float directionChangeInterval;
	sf::Vector2f currentDirection;
	float m_currentRotation;
	float m_rotationSpeed;
	float pivotOffset;

	//animation variables
	int frameWidth = 240 / 4;
	int frameHeight = 60;
	int m_currentFrame = 0;
	int m_firstFrame = 0;
	int m_lastFrame = 3;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f;

};
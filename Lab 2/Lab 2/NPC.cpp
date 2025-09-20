#include "NPC.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

NPC::NPC()
	: m_speed(50.f), m_maxSpeed(100.f), m_minSpeed(-100.f), directionChangeTimer(0.0f), directionChangeInterval(2.0f), 
	currentDirection(0.0f, 0.0f), m_currentRotation(0.0f), m_rotationSpeed(90.f)
{
	npcInit();
}

void NPC::npcInit()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/npc.png"))
	{
		std::cout << "problem loading npc texture" << std::endl;
	}
	else
	{
		std::cout << "Loaded npc texture successfully!" << std::endl;
		m_sprite = sf::Sprite(m_texture);
	}
	int frameCount = 4;
	int frameIndex = 0;
	m_sprite.setTextureRect(sf::IntRect({ frameIndex * frameWidth, 0 }, { frameWidth, frameHeight }));
	m_sprite.setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight / 2.0f));
	m_sprite.setScale(sf::Vector2f(1.5f, 1.5f));
	m_sprite.setPosition(sf::Vector2f(200.f, 200.f));

}



sf::Vector2f NPC::Wander()
{
	float randomX = (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f; 
	float randomY = (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f; 

	currentDirection.x += randomX;
	currentDirection.y += randomY;

	currentDirection = MathUtils::normalize(currentDirection);
	

	return currentDirection;
}

void NPC::update(float dt)
{
	directionChangeTimer += dt;
	if (directionChangeTimer >= directionChangeInterval) {
		sf::Vector2f newDirection = Wander();
		
		float interp = 0.1f; 
		currentDirection = currentDirection + (newDirection - currentDirection) * interp;
		directionChangeTimer = 0.f;
	}

	SteeringOutput steering = getSteering();

	m_sprite.move(steering.linear * dt);
	
	
	float angleDegrees = MathUtils::toDegrees(atan2(steering.linear.y, steering.linear.x));

	smoothRotate(angleDegrees +90, m_rotationSpeed, dt);

	wrapAroundScreen(800.0f, 600.0f);
	updateAnimation(dt);
}

void NPC::updateAnimation(float dt)
{
		m_animationTimer += dt;
		if (m_animationTimer >= m_animationDelay) {
			m_animationTimer = 0.0f;
			m_currentFrame++;
			if (m_currentFrame > m_lastFrame) {
				m_currentFrame = m_firstFrame;
			}

			m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
		}
}




void NPC::smoothRotate(float targetAngle, float rotationSpeed, float dt)
{
	while (targetAngle < 0) targetAngle += 360;
	while (targetAngle >= 360) targetAngle -= 360;

	while (m_currentRotation < 0.0f) m_currentRotation += 360.f;
	while (m_currentRotation >= 360.f) m_currentRotation -= 360.f;

	float angleDiff = targetAngle - m_currentRotation;
	while (angleDiff > 180) angleDiff -= 360.f;
	while (angleDiff < -180) angleDiff += 360.f;
	float rotationStep = angleDiff * 5.0f * dt;
	float maxStep = rotationSpeed * dt;

	if (angleDiff > maxStep)
		m_currentRotation += maxStep;
	else if (angleDiff < -maxStep)
		m_currentRotation -= maxStep;
	else
		m_currentRotation = targetAngle;
	m_sprite.setRotation(sf::degrees(m_currentRotation));
}

void NPC::setTarget(const sf::Vector2f& targetPosition)
{
	m_targetPosition = targetPosition;
}

SteeringOutput NPC::getSteering()
{
	SteeringOutput steering;
	directionChangeTimer += 0.0f;
	steering.linear = MathUtils::normalize(currentDirection) * m_speed;
	steering.angular = 0.0f;
	return steering;
}


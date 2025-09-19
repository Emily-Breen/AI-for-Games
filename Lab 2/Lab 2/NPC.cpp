#include "NPC.h"
#include <random>
#include <iostream>

NPC::NPC()
	: m_speed(50.f), m_maxSpeed(300.f), m_minSpeed(-100.f), directionChangeTimer(0.0f), directionChangeInterval(2.0f), 
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
		m_npcSprite = sf::Sprite(m_texture);
	}
	int frameCount = 4;
	int frameWidth = 240 / frameCount;
	int frameHeight = 60;
	int frameIndex = 0;
	m_npcSprite.setTextureRect(sf::IntRect({ frameIndex * frameWidth, 0 }, { frameWidth, frameHeight }));
	m_npcSprite.setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight / 2.0f));
	m_npcSprite.setScale(sf::Vector2f(1.5f, 1.5f));
	m_npcSprite.setPosition(sf::Vector2f(200.f, 200.f));

}

void NPC::drawNPC(sf::RenderWindow& window)
{
	window.draw(m_npcSprite);
}

sf::Vector2f NPC::getRandomDirection()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.f, 2.f * 3.14159265f);

	float angle = dis(gen);
	return sf::Vector2f(std::cos(angle), std::sin(angle));
}

void NPC::update(float dt)
{
	directionChangeTimer += dt;
	if (directionChangeTimer >= directionChangeInterval) {
		currentDirection = getRandomDirection();
		directionChangeTimer = 0.0f;
	}
	m_npcSprite.move(currentDirection * m_speed * dt);
	
	float angleRad = std::atan2(currentDirection.y, currentDirection.x);
	float angleDegrees = angleRad * 180.0f / 3.14159265f;

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
		int frameWidth = 240 / 4;
		int frameHeight = 60;
		m_npcSprite.setTextureRect(sf::IntRect({ m_currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
	}
}

void NPC::wrapAroundScreen(float windowWidth, float windowHeight)
{
	sf::Vector2f pos = m_npcSprite.getPosition();


	if (pos.x > windowWidth) pos.x = 0;
	else if (pos.x < 0)      pos.x = windowWidth;


	if (pos.y > windowHeight) pos.y = 0;
	else if (pos.y < 0)       pos.y = windowHeight;

	m_npcSprite.setPosition(pos);
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
	float rotationStep = rotationSpeed * dt;

	if (angleDiff > rotationStep)
	{
		m_currentRotation += rotationStep;
	}
	else if (angleDiff < -rotationStep)
	{
		m_currentRotation -= rotationStep;
	}
	else
	{
		m_currentRotation = targetAngle;
	}
	rotateNPCShip(m_currentRotation);
}

void NPC::rotateNPCShip(float angle)
{
	m_npcSprite.setRotation(sf::degrees(angle));
}

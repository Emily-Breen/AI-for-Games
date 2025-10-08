#include "NPC.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

NPC::NPC(SteeringBehaviour* behaviour, sf::Font& font, const Entity* player)
	: m_behaviour(behaviour), m_speed(0.f), m_maxSpeed(600.f), m_minSpeed(-50.f),
	currentDirection(0.0f, 0.0f), m_currentRotation(0.0f), m_rotationSpeed(500.0f),
	m_coneAngle(60.0f), m_visionRange(200.f), m_text(font), m_player(player)
{
	npcInit();
	m_text.setFont(font);
	m_text.setCharacterSize(20);
	m_text.setFillColor(sf::Color::White);
	m_text.setString(m_behaviour->getName());
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
	float x = static_cast<float>(rand() % 1920);
	float y = static_cast<float>(rand() % 1080);
	m_sprite.setPosition(sf::Vector2f(x,y));

	m_visionCone.setPointCount(3);
	m_visionCone.setFillColor(sf::Color(255, 255, 0, 100));
	m_visionCone.setOrigin(sf::Vector2f(0.0f, 0.0f));

}



void NPC::update(float dt)
{
	if (!m_isActive) return;


	SteeringOutput steering = m_behaviour->getSteering(*this, *m_player, dt);

	
	float npcAccelRate = 5.0f; 
	m_velocity += (steering.linear - m_velocity) * npcAccelRate * dt;


	float speed = MathUtils::vectorLength(m_velocity);
	if (speed <= 0.f)
		m_velocity = { 0.f, 0.f };
	const float minSpeed = 0.f;

	if (speed < minSpeed && speed > 0.01f) {
		m_velocity = MathUtils::normalize(m_velocity) * minSpeed;
	}
	if (speed > m_maxSpeed) {
		m_velocity = MathUtils::normalize(m_velocity) * m_maxSpeed;
	}

	
	m_sprite.move(m_velocity * dt);
	currentDirection = m_velocity;
	
	
	m_currentRotation += steering.angular * dt;
	while (m_currentRotation < 0.0f) m_currentRotation += 360.0f;
	while (m_currentRotation >= 360.0f) m_currentRotation -= 360.0f;

	m_sprite.setRotation(sf::degrees(m_currentRotation));

	
	wrapAroundScreen(1920.0f, 1080.0f);
	updateAnimation(dt);
	m_text.setPosition(m_sprite.getPosition() + sf::Vector2f(0, -30));
	updateVisionCone();
}



void NPC::draw(sf::RenderWindow& t_window)
{
	if (!m_isActive) return;
	//t_window.draw(m_visionCone);
	Entity::draw(t_window);
	t_window.draw(m_text);
}

bool NPC::isPlayerInCOV(const Entity& player)
{
	sf::Vector2f playerPos = player.getPosition() - getPosition();
	float distanceToPlayer = MathUtils::vectorLength(playerPos);
	if (distanceToPlayer > m_visionRange) {
		return false;
	}

	float rotationRad = MathUtils::toRadians(getRotation() - 90.f);
	sf::Vector2f forward(std::cos(rotationRad), std::sin(rotationRad));

	float dot = MathUtils::dotProduct(MathUtils::normalize(forward), MathUtils::normalize(playerPos));
	dot = std::max(-1.0f, std::min(1.0f, dot));
	float angle = MathUtils::toDegrees(std::acos(dot));

	return angle < m_coneAngle * 0.5f;
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

void NPC::setActive(bool active)
{
	m_isActive = active;
}

bool NPC::getActive() const
{
	return m_isActive;
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

	float step = rotationSpeed * dt;
	if (angleDiff > step)
		m_currentRotation += step;
	else if (angleDiff < -step)
		m_currentRotation -= step;
	else
		m_currentRotation = targetAngle;

	while (m_currentRotation < 0.0f) m_currentRotation += 360.f;
	while (m_currentRotation >= 360.f) m_currentRotation -= 360.f;

	m_sprite.setRotation(sf::degrees(m_currentRotation));
}


void NPC::updateVisionCone()
{
	m_visionCone.setPosition(m_sprite.getPosition());

	float rotationRad = MathUtils::toRadians(getRotation()  -90.0f);
	sf::Vector2f forward(std::cos(rotationRad), std::sin(rotationRad));
	float halfangleRad = MathUtils::toRadians(m_coneAngle * 0.5f);
	float range = m_visionRange;

	sf::Vector2f leftDirection(std::cos(rotationRad - halfangleRad), std::sin(rotationRad - halfangleRad));

	sf::Vector2f rightDirection(std::cos(rotationRad + halfangleRad), std::sin(rotationRad + halfangleRad));

	m_visionCone.setPointCount(3);
	m_visionCone.setPoint(0, sf::Vector2f(0.0f, 0.0f));
	m_visionCone.setPoint(1, leftDirection * range);
	m_visionCone.setPoint(2, rightDirection * range);

	if (isPlayerInCOV(*m_player))
		m_visionCone.setFillColor(sf::Color(255, 0, 0, 100));
	else
		m_visionCone.setFillColor(sf::Color(255, 255, 0, 100));
}

void NPC::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

sf::Vector2f NPC::getVelocity() const
{
	return m_velocity;
}

void NPC::setBehaviour(SteeringBehaviour* behaviour)
{
	m_behaviour = behaviour;
}




#include "Player.h"
#include <iostream>

Player::Player()
	: m_speed(0.f), m_maxSpeed(600.f), m_minSpeed(-200.f) 
{
	playerInit();
}

void Player::playerInit()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/player.png"))
	{
		std::cout << "problem loading player texture" << std::endl;
	}
	else
	{
		std::cout << "Loaded player texture successfully!" << std::endl;
		m_sprite = sf::Sprite(m_texture);
	}

	int frameCount = 24;
	int frameWidth = 2400 / frameCount; 
	int frameHeight = 100;
	int frameIndex = 6; 

	m_sprite.setTextureRect(sf::IntRect({ frameIndex * frameWidth, 0 }, { frameWidth, frameHeight }));
	m_sprite.setOrigin(sf::Vector2f(frameWidth / 2.f, frameHeight / 2.f));
	m_sprite.setScale(sf::Vector2f(0.7f, 0.7f));
	m_sprite.setPosition(sf::Vector2f(400.f, 300.f));
}


void Player::movePlayer(float dx, float dy)
{
	m_sprite.move(sf::Vector2f(dx, dy));
}

void Player::moveForward(float dt)
{
	SteeringOutput steering = getSteering();
	m_sprite.move(steering.linear * dt);

	if (m_speed > 0 && MathUtils::vectorLength(steering.linear) > 0.01f)
	{
		float angle = std::atan2(steering.linear.y, steering.linear.x);
		m_sprite.setRotation(sf::degrees(MathUtils::toDegrees(angle) + 90.0f));
	}
}


void Player::updateAnimation(float dt)
{
	m_animationTimer += dt;
	if (m_animationTimer >= m_animationDelay) {
		m_animationTimer = 0.0f;
		m_currentFrame++;
		if (m_currentFrame > m_lastFrame)
			m_currentFrame = m_firstFrame;

		m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
	}
}

void Player::accelerate()
{
	if (m_speed < m_maxSpeed)
		m_speed += 20.0f; 
}

void Player::decelerate()
{
	if (m_speed > m_minSpeed)
		m_speed -= 15.0f;
}

void Player::applyFriction()
{
	if (m_speed > 0) {
		m_speed -= 5.0f;
		if (m_speed < 0) m_speed = 0;
	}
	else if (m_speed < 0) {
		m_speed += 5.0f;
		if (m_speed > 0) m_speed = 0;
	}
}

void Player::update(float dt)
{
	moveForward(dt);
	updateAnimation(dt);
	wrapAroundScreen(800.0f, 600.0f);
}

SteeringOutput Player::getSteering()
{
	SteeringOutput steering;
	float rotation_deg = m_sprite.getRotation().asDegrees();
	float rotation_rad = MathUtils::toRadians(rotation_deg);

	sf::Vector2f forward(std::sin(rotation_rad), -std::cos(rotation_rad));
	
	steering.linear = forward * m_speed;

	steering.angular = 0.0f;

	return steering;

}



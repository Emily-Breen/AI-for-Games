#pragma once
#include "SteeringBehaviour.h"
#include "MathUtils.h"
#include <cstdlib>
#include <cmath>


class WanderBehaviour : public SteeringBehaviour
{
	sf::Vector2f m_direction{ 1.0f,0.0f };
	float m_timer{ 0.0f };
	float m_interval{ 2.0f };
public:
	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override {
	
		m_timer += dt;
		if (m_timer >= m_interval) {
			float randX = (static_cast<float>(rand()) / RAND_MAX);
			float randY = (static_cast<float>(rand()) / RAND_MAX);
			m_direction += sf::Vector2f(randX, randY);
			m_direction = MathUtils::normalize(m_direction);

			m_timer = 0.0f;
		}
		SteeringOutput steering;
		steering.linear = m_direction * 50.0f;
		steering.angular = 0.0f;
		return steering;
	
	}
	const char* getName() const override {
		return "Wander";
	}
};

class SeekBehaviour : public SteeringBehaviour
{

public:
	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override
	{
		sf::Vector2f dir = player.getPosition() - npc.getPosition();
		dir = MathUtils::normalize(dir);

		SteeringOutput steering;
		steering.linear = dir * 100.f;
		steering.angular = 0.f;
		return steering;
	}

	const char* getName() const override { 
		return "Seek"; }
};


class ArriveBehaviour : public SteeringBehaviour
{
	float maxSpeed;
	float slowingRadius;

public:
	ArriveBehaviour(float maxSpeed, float slowingRadius)
		: maxSpeed(maxSpeed), slowingRadius(slowingRadius) {};

	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override {
		sf::Vector2f direction = player.getPosition() - npc.getPosition();
		float distance = MathUtils::vectorLength(direction);
		direction = MathUtils::normalize(direction);

		float m_speed = (distance < slowingRadius) ? maxSpeed * (distance / slowingRadius) : maxSpeed;

		SteeringOutput steering;
		steering.linear = direction * m_speed;
		steering.angular = 0.0f;
		return steering;
	}
	const char* getName() const override {
		return "Arrive";
	}
};
class pursueBehaviour : public SteeringBehaviour
{
	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override {
		sf::Vector2f direction = player.getPosition() - npc.getPosition();
		direction = MathUtils::normalize(direction);
		SteeringOutput steering;
		steering.linear = direction * 200.0f;	
		steering.angular = 0.0f;
		return steering;

	}
	const char* getName() const override {
		return "Pursue";
	}
};


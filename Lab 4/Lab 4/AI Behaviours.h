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


class ArriveFastBehaviour : public SteeringBehaviour
{
	float maxSpeed;
	float slowingRadius;

public:
	ArriveFastBehaviour(float maxSpeed, float slowingRadius)
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
		return "Arrive/Fast";
	}
};
class ArriveSlowBehaviour : public SteeringBehaviour
{
	float maxSpeed;
	float slowingRadius;

public:
	ArriveSlowBehaviour(float maxSpeed, float slowingRadius)
		: maxSpeed(maxSpeed), slowingRadius(slowingRadius) {
	};

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
		return "Arrive/Slow";
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
//lab3 swarm behaviour
class SwarmBehaviour : public SteeringBehaviour
{
public:
	SwarmBehaviour(float a, float b, int n, int m, const std::vector<NPC*>& swarm)
		: A(a), B(b), N(n), M(m), m_swarm(swarm) {
	}

	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override {
		sf::Vector2f totalForce(0.f, 0.f);
		for (const auto* other : m_swarm) {
			if (other == &npc) continue;

			sf::Vector2f R = npc.getPosition() - other->getPosition();
			float D = std::sqrt(R.x * R.x + R.y * R.y);
			if (D < 0.0001f) continue;

			float U = -A / std::pow(D, N) + B / std::pow(D, M);
			sf::Vector2f dir = MathUtils::normalize(R);
			totalForce += dir * U;
		}
		SteeringOutput steering;
		steering.linear = totalForce * dt;
		steering.angular = 0.0f;
		return steering;
	}

	const char* getName() const override {
		return "Swarm";
	}
private:
	float A, B;
	int N, M;
	const std::vector<NPC*>& m_swarm;
};
// just a placeholder as entity is abstract and cannot be instantiated - to use existing arrive behaviour
class StaticEntity : public Entity
{
public:
	explicit StaticEntity(const sf::Vector2f& pos)
	{
		m_sprite.setPosition(pos);
	}

	void update(float) override {}
	void draw(sf::RenderWindow&) override {}  


	void wrapAroundScreen(float, float) override {}

	
};

class FormationFollowBehaviour : public SteeringBehaviour {
	sf::Vector2f m_offset;
	ArriveFastBehaviour m_arrive;

public:
	FormationFollowBehaviour(const sf::Vector2f& offset)
		: m_offset(offset), m_arrive(600.f, 120.f) 
	{}

	SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) override {
		SteeringOutput steering;

	
		float rotationRad = MathUtils::toRadians(player.getRotation());
		sf::Vector2f rotatedOffset(
			m_offset.x * std::cos(rotationRad) - m_offset.y * std::sin(rotationRad),
			m_offset.x * std::sin(rotationRad) + m_offset.y * std::cos(rotationRad)
		);

		sf::Vector2f targetPos = player.getPosition() + rotatedOffset;

		StaticEntity targetEntity(targetPos);
		steering = m_arrive.getSteering(npc, targetEntity, dt);

		
		sf::Vector2f playerVelocity = player.getVelocity();
		float playerSpeed = MathUtils::vectorLength(playerVelocity);
		if (playerSpeed < 10.f)
			steering.linear = { 0.f, 0.f };

		float playerRot = player.getRotation();
		float npcRotation = npc.getRotation();
		float diff = playerRot - npcRotation;
		while (diff > 180.f) diff -= 360.f;
		while (diff < -180.f) diff += 360.f;

		float turnSpeed = 720.f;
		float angularChange = std::clamp(diff, -turnSpeed * dt, turnSpeed * dt);
		steering.angular = angularChange / dt;

		return steering;
	}

	const char* getName() const override { return "FormationFollow"; }
};


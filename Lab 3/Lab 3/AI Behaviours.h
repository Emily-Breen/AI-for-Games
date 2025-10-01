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
    SwarmBehaviour(const std::vector<NPC*>& swarm)
        : m_swarm(swarm), m_screenCenter(960.f, 540.f),
        m_targetPoint(960.f, 540.f), m_timeSinceTargetChange(0.f),
        m_targetChangeInterval(3.f) // Change target every 3 seconds
    {
     
        updateRandomTarget();
    }

    SteeringOutput getSteering(const Entity& npc, const Entity& /*player*/, float dt) override {
        SteeringOutput steering;
        steering.linear = sf::Vector2f(0.f, 0.f);
        steering.angular = 0.f;

		//update target point periodically
        m_timeSinceTargetChange += dt;
        if (m_timeSinceTargetChange >= m_targetChangeInterval) {
            updateRandomTarget();
            m_timeSinceTargetChange = 0.f;
        }

        const float perception = 500.f;
        const float separationRadius = 100.f;
        const float maxForce = 300.f;

        sf::Vector2f separation(0.f, 0.f);
        sf::Vector2f alignment(0.f, 0.f);
        sf::Vector2f cohesion(0.f, 0.f);

        int separationCount = 0;
        int alignmentCount = 0;
        int cohesionCount = 0;

        const NPC* currentNPC = dynamic_cast<const NPC*>(&npc);
        if (!currentNPC) return steering;

        for (auto* other : m_swarm) {
            if (other == currentNPC) continue;

            sf::Vector2f offset = other->getPosition() - npc.getPosition();
            float distance = MathUtils::vectorLength(offset);

            if (distance > 0.f && distance < perception) {
                //avoids crowding
                if (distance < separationRadius) {
                    sf::Vector2f diff = npc.getPosition() - other->getPosition();
                    diff = MathUtils::normalize(diff) / distance;
                    separation += diff;
                    separationCount++;
                }

				//steer towwards average heading
                alignment += other->getVelocity();
                alignmentCount++;

				// steer towards average position
                cohesion += other->getPosition();
                cohesionCount++;
            }
        }

        sf::Vector2f finalForce(0.f, 0.f);

        // Calculate separation
        if (separationCount > 0) {
            separation /= (float)separationCount;
            if (MathUtils::vectorLength(separation) > 0.f) {
                separation = MathUtils::normalize(separation) * maxForce;
                separation -= currentNPC->getVelocity();
            }
        }

        // Calculate alignment
        if (alignmentCount > 0) {
            alignment /= (float)alignmentCount;
            if (MathUtils::vectorLength(alignment) > 0.f) {
                alignment = MathUtils::normalize(alignment) * maxForce;
                alignment -= currentNPC->getVelocity();
            }
        }

        // Calculate cohesion
        if (cohesionCount > 0) {
            cohesion /= (float)cohesionCount;
            cohesion -= npc.getPosition();
            if (MathUtils::vectorLength(cohesion) > 0.f) {
                cohesion = MathUtils::normalize(cohesion) * maxForce;
                cohesion -= currentNPC->getVelocity();
            }
        }

        // Calculate attraction to random target point
        sf::Vector2f targetAttraction(0.f, 0.f);
        sf::Vector2f toTarget = m_targetPoint - npc.getPosition();
        float distanceToTarget = MathUtils::vectorLength(toTarget);

        if (distanceToTarget > 50.f) {  // Move towards target if not too close
            targetAttraction = MathUtils::normalize(toTarget) * maxForce;
            targetAttraction -= currentNPC->getVelocity();

            // Scale by distance (weaker when close, stronger when far)
            float attractionStrength = std::min(distanceToTarget / 400.f, 1.2f);
            targetAttraction *= attractionStrength;
        }

        
        sf::Vector2f boundaryForce = calculateBoundaryAvoidance(npc.getPosition(), maxForce);

        // Weight the forces
        separation *= 1.5f;         // Avoid collisions
        alignment *= 1.0f;          // Match velocity
        cohesion *= 1.0f;           // Stay together
        targetAttraction *= 0.6f;   // Move towards random points
        boundaryForce *= 2.0f;      // Strong boundary avoidance

        // Combine all forces
        finalForce = separation + alignment + cohesion + targetAttraction + boundaryForce;

        //add random movements
        float jitter = 50.f;
        finalForce += sf::Vector2f(
            ((rand() % 2000) / 1000.f - 1.f) * jitter,
            ((rand() % 2000) / 1000.f - 1.f) * jitter
        );

        // Limit to max force
        float len = MathUtils::vectorLength(finalForce);
        if (len > maxForce) {
            finalForce = MathUtils::normalize(finalForce) * maxForce;
        }

        steering.linear = finalForce;
        return steering;
    }

    const char* getName() const override { return "Swarm"; }

private:
    const std::vector<NPC*>& m_swarm;
    sf::Vector2f m_screenCenter;
    sf::Vector2f m_targetPoint;
    float m_timeSinceTargetChange;
    float m_targetChangeInterval;

    void updateRandomTarget() {
        // Define safe margins from screen edges
        const float marginX = 200.f;
        const float marginY = 150.f;

        // Generate random position within screen bounds
        float x = marginX + (rand() % static_cast<int>(1920.f - 2 * marginX));
        float y = marginY + (rand() % static_cast<int>(1080.f - 2 * marginY));

        m_targetPoint = sf::Vector2f(x, y);
    }

    sf::Vector2f calculateBoundaryAvoidance(const sf::Vector2f& position, float maxForce) {
        sf::Vector2f avoidance(0.f, 0.f);
        const float boundary = 150.f;  // Distance from edge to start avoiding
        const float screenWidth = 1920.f;
        const float screenHeight = 1080.f;

        // Check left boundary
        if (position.x < boundary) {
            avoidance.x += (boundary - position.x) / boundary * maxForce;
        }
        // Check right boundary
        if (position.x > screenWidth - boundary) {
            avoidance.x -= (position.x - (screenWidth - boundary)) / boundary * maxForce;
        }
        // Check top boundary
        if (position.y < boundary) {
            avoidance.y += (boundary - position.y) / boundary * maxForce;
        }
        // Check bottom boundary
        if (position.y > screenHeight - boundary) {
            avoidance.y -= (position.y - (screenHeight - boundary)) / boundary * maxForce;
        }

        return avoidance;
    }
};
#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

struct SteeringOutput
{
	sf::Vector2f linear;
	float angular;
};

class SteeringBehaviour
{
public:
	virtual ~SteeringBehaviour() = default;
	virtual SteeringOutput getSteering(const Entity& npc, const Entity& player, float dt) = 0;
	virtual const char* getName() const = 0;
};

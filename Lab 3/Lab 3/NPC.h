#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "SteeringBehaviour.h"
#include "MathUtils.h"
class NPC : public Entity {
public:
	NPC(SteeringBehaviour* behaviour, sf::Font& font, const Entity* player);
	void npcInit();
	void update(float dt) override;
	void draw(sf::RenderWindow& t_window) override;
	bool isPlayerInCOV(const Entity& player);
	void updateAnimation(float dt) override;
	void setActive(bool active);
	bool getActive() const;
	void smoothRotate(float targetAngle, float rotationSpeed, float dt);
	void updateVisionCone();

private:

	const Entity* m_player;
	SteeringBehaviour* m_behaviour;
	sf::Text m_text; 
	float m_speed;
	float m_coneAngle;
	float m_visionRange;
	bool m_isActive = true;
	float m_maxSpeed;
	float m_minSpeed;
	sf::Vector2f currentDirection;
	float m_currentRotation;
	float m_rotationSpeed;
	sf::ConvexShape m_visionCone;
	//animation variables
	int frameWidth = 240 / 4;
	int frameHeight = 60;
	int m_currentFrame = 0;
	int m_firstFrame = 0;
	int m_lastFrame = 3;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f;

};
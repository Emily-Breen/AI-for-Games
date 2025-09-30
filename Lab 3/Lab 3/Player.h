#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "MathUtils.h"
#include "SteeringBehaviour.h"
#include "NPC.h"
class Player :public Entity {
public:
	Player();

	void playerInit();

	void movePlayer(float dx, float dy);
	void moveForward(float dt);
	void updateAnimation(float dt) override;
	void accelerate(); 
	void decelerate(); 
	void draw(sf::RenderWindow& t_window) override;
	void applyFriction();
	bool isNPCInCOV(const Entity& npc) const;
	void updateVisionCone(const std::vector<std::unique_ptr<NPC>>& npcs);
	void setNPCs(const std::vector<std::unique_ptr<NPC>>* npcs);
	
	void update(float dt) override;
private:
	SteeringOutput getSteering();
	const std::vector<std::unique_ptr<NPC>>* m_npcs = nullptr;
	//player variables
    float m_speed;
	float m_maxSpeed;
	float m_minSpeed;
	sf::Vector2f m_inputDirection{ 0.f, 0.f };
	float m_coneAngle{ 60.0f };     
	float m_visionRange{ 200.0f };
	sf::ConvexShape m_visionCone;
	//animation variables
	int frameWidth = 2400 / 24;
	int frameHeight = 100;
	int m_currentFrame = 6;
	int m_firstFrame = 6;
	int m_lastFrame = 11;
	float m_animationTimer = 0.0f;
	float m_animationDelay = 0.1f; 

};
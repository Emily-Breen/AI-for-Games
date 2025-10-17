
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "NPC.h"
#include "Entity.h"
#include "AI Behaviours.h"
const sf::Color BLACK; // const colour
struct FormationSlot {
	sf::Vector2f localOffset;
	float orientationOffset;
};

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	

	
	Player player;
	std::vector<std::unique_ptr<NPC>> npcs;
	std::vector<std::unique_ptr<NPC>> formationNPCs;
	std::vector<std::unique_ptr<FormationFollowBehaviour>> formationBehaviours;
	std::vector<sf::Vector2f> formationOffsets;
	bool m_formationActive = false;
	std::vector<std::unique_ptr<NPC>> swarm;
	std::vector<NPC*> swarmPtrs;
	WanderBehaviour wanderAI;
	SeekBehaviour seekAI;
	ArriveSlowBehaviour arriveAISlow;
	ArriveFastBehaviour arriveAIFast;
	pursueBehaviour pursueAI;
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message
	sf::Text m_toggleText{ m_jerseyFont };
	bool m_DELETEexitGame; // control exiting game
	bool playerMoved = false;

	

};

#pragma warning( pop ) 
#endif // !GAME_HPP


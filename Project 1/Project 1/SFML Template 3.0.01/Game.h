
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "NPC.h"

const sf::Color BLACK; // const colour


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
	NPC npc;
	
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message
	bool m_DELETEexitGame; // control exiting game

};

#pragma warning( pop ) 
#endif // !GAME_HPP


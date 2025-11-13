
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.h"
#include "Animal.h"


const sf::Color BLACK { 0, 0, 0, 0 }; // const colour

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
	void updateAnimals();
	
	Board m_board{ 5,150.f };
	std::vector<Animal> m_player1Pieces;
	std::vector<Animal> m_player2Pieces;
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message
	sf::Text m_DELETEwelcomeMessage{ m_jerseyFont }; // text used for message on screen
	sf::Texture m_DELETElogoTexture;//  texture used for sfml logo
	sf::Sprite m_DELETElogoSprite{ m_DELETElogoTexture }; // sprite used for sfml logo
	
	
	bool m_DELETEexitGame; // control exiting game

};

#pragma warning( pop ) 
#endif // !GAME_HPP


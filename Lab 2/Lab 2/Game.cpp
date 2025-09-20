
#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{800U, 600U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false} //when true game will exit
{ 
	
	
	
}
Game::~Game()
{
}


void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) 
		{
			processKeys(newEvent);
		}
	}
}


void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
	}
	
}

void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}

void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
	bool moving = false;
	const float rotationSpeed = 100.0f; 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		player.accelerate();
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		player.decelerate();
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		player.setRotation(-rotationSpeed * t_deltaTime.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		player.setRotation(rotationSpeed * t_deltaTime.asSeconds());
	}
	if (!moving) {
		player.applyFriction();
	}
	player.update(t_deltaTime.asSeconds());

	npc.update(t_deltaTime.asSeconds());
}

void Game::render()
{
	m_window.clear(BLACK);

	
	player.draw(m_window);
	npc.draw(m_window);
	
	m_window.display();
}


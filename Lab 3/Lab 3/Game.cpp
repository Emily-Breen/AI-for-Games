
#include "Game.h"
#include <iostream>


Game::Game() : 
	m_window{ sf::VideoMode{ sf::Vector2u{1920U, 1080U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false}, wanderAI(), seekAI(), arriveAISlow(100.0f,50.0f), arriveAIFast(200.0f,150.0f), pursueAI()//when true game will exit
{ 
	
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
   //npcs.push_back(std::make_unique<NPC>(&wanderAI, m_jerseyFont, &player));
	//npcs.push_back(std::make_unique<NPC>(&seekAI, m_jerseyFont, &player));
	//npcs.push_back(std::make_unique<NPC>(&arriveAISlow, m_jerseyFont, &player));
	//npcs.push_back(std::make_unique<NPC>(&arriveAIFast, m_jerseyFont, &player));
	//npcs.push_back(std::make_unique<NPC>(&pursueAI, m_jerseyFont, &player));
	
	SwarmBehaviour* swarmBehavior = new SwarmBehaviour(200.f, 6000.0f, 0.9, 1.0f, swarmPtrs);

	for (int i = 0; i < 200; i++) {
		auto npc = std::make_unique<NPC>(swarmBehavior, m_jerseyFont, &player);

	
		sf::Vector2f initVel(
			(rand() % 300 - 150),  
			(rand() % 300 - 150)
		);
		npc->setVelocity(initVel);

		swarmPtrs.push_back(npc.get());
		swarm.push_back(std::move(npc));
	}

	
	m_toggleText.setFont(m_jerseyFont);
	m_toggleText.setFillColor(sf::Color::White);
	m_toggleText.setCharacterSize(30.0f);
	m_toggleText.setPosition(sf::Vector2f(0, 0));
	m_toggleText.setString("Swarming behaviour");

	
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
	//if (sf::Keyboard::Key::Numpad1 == newKeypress->code)
	//{
	//	npcs[0]->setActive(!npcs[0]->getActive());
	//}	
	//if (sf::Keyboard::Key::Numpad2 == newKeypress->code)
	//{
	//	npcs[1]->setActive(!npcs[1]->getActive());
	//}
	//if (sf::Keyboard::Key::Numpad3 == newKeypress->code)
	//{
	//	npcs[2]->setActive(!npcs[2]->getActive());
	//}
	//if (sf::Keyboard::Key::Numpad4 == newKeypress->code)
	//{
	//	npcs[3]->setActive(!npcs[3]->getActive());
	//}
	//if (sf::Keyboard::Key::Numpad5 == newKeypress->code)
	//{
	//	npcs[4]->setActive(!npcs[4]->getActive());
	//}

	
	
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
	player.updateVisionCone(npcs);
	for (auto& npc : swarm) {
		npc->update(t_deltaTime.asSeconds());
	}
}

void Game::render()
{
	m_window.clear(BLACK);

	
	player.draw(m_window);
	for (auto& npc : swarm) {
		npc->draw(m_window);
	}
	m_window.draw(m_toggleText);
	m_window.display();
}


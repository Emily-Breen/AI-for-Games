
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


	formationOffsets = {
	   {-120.f, 0.f},
	   {-60.f, 60.f},
	   {-60.f, -60.f},
	   {0.f, 120.f}
	};

	for (size_t i = 0; i < formationOffsets.size(); ++i) {
		auto behaviour = std::make_unique<FormationFollowBehaviour>(formationOffsets[i]);
		auto npc = std::make_unique<NPC>(behaviour.get(), m_jerseyFont, &player);

		formationBehaviours.push_back(std::move(behaviour));

		npc->setPosition(player.getPosition() + formationOffsets[i]);
		npc->setVelocity({ 0.f, 0.f });

		formationNPCs.push_back(std::move(npc));
	}

	
	m_toggleText.setFont(m_jerseyFont);
	m_toggleText.setFillColor(sf::Color::White);
	m_toggleText.setCharacterSize(30.0f);
	

	
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
	

	if (sf::Keyboard::Key::F == newKeypress->code) {
		m_formationActive = !m_formationActive;
		if (m_formationActive) {
			m_toggleText.setString("Formation Mode ON");
		}
		else {
			m_toggleText.setString("Formation Mode OFF");
		}
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
		playerMoved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		player.decelerate();
		moving = true;
		playerMoved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		player.setRotation(-rotationSpeed * t_deltaTime.asSeconds());
		playerMoved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		player.setRotation(rotationSpeed * t_deltaTime.asSeconds());
		playerMoved = true;
	}
	if (!moving) {
		player.applyFriction();
	}
	player.update(t_deltaTime.asSeconds());
	player.updateVisionCone(npcs);
	if (m_formationActive) {
		for (auto& npc : formationNPCs) {
			npc->update(t_deltaTime.asSeconds());
		}
	}
}

	


void Game::render()
{
	m_window.clear(BLACK);

	
	player.draw(m_window);
	for (auto& npc : formationNPCs) {
		npc->draw(m_window);
	}
	m_window.draw(m_toggleText);
	m_window.display();
}



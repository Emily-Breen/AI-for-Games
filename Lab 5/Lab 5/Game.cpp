
#include "Game.h"

#include <iostream>




Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{1080U, 1080U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{ false }
{
	if (!m_jerseyFont.openFromFile("ASSETS/FONTS/Jersey20-Regular.ttf"))
	{
		std::cout << "Error: Could not load font!" << std::endl;
	}
	else
	{
		std::cout << "Font loaded successfully (Game)\n";
	}

	m_UI.setFont(m_jerseyFont);
	m_UI.setUpText();  
	m_flowField.setFont(m_jerseyFont);
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
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonPressed>()) //user clicked mouse
		{
			processMouse(newEvent);
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
	if (sf::Keyboard::Key::Space == newKeypress->code)
	{
		m_flowField.toggleMovement();
	}
	if (sf::Keyboard::Key::Numpad1 == newKeypress->code || sf::Keyboard::Key::Num1 == newKeypress->code)
	{
		m_flowField.toggleVectorField();
	}
	if (sf::Keyboard::Key::Numpad2 == newKeypress->code || sf::Keyboard::Key::Num2 == newKeypress->code)
	{
		m_flowField.toggleHeatMap();
	}
	if (sf::Keyboard::Key::A == newKeypress->code)
	{
		m_flowField.togglePathfinding();
		m_UI.setPathfindingMode(m_flowField.isUsingAStar());
	}
	
}

void Game::processMouse(const std::optional<sf::Event> t_event)
{
	const sf::Event::MouseButtonPressed* click = t_event->getIf<sf::Event::MouseButtonPressed>();
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);

	if (sf::Mouse::Button::Left == click->button) {
		std::cout << "Left mouse button clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
		m_flowField.setStartPosition(mousePos);
	}

	if (sf::Mouse::Button::Right == click->button) {
		std::cout << "Right mouse button clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
		m_flowField.setGoalPosition(mousePos);
	}
	if (sf::Mouse::Button::Middle == click->button) {
		
		m_flowField.toggleObstacleAtPosition(mousePos);
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
	m_flowField.update(t_deltaTime.asSeconds());
}


void Game::render()
{
	m_window.clear(ULTRAMARINE);
	m_flowField.draw(m_window);
	m_UI.draw(m_window);
	m_window.display();
}



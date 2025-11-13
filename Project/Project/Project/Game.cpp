
#include "Game.h"
#include <iostream>



Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{1920U, 1080U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false}//when true game will exit
{
	m_board.updateCellSize(m_window.getSize());

	float cellSize = m_board.getCellSize();
	//player 1 pieces
	m_player1Pieces = {
		{ Player::Player1, AnimalType::Frog },
		{ Player::Player1, AnimalType::Snake },
		{ Player::Player1, AnimalType::Donkey },
		{ Player::Player1, AnimalType::Donkey },
		{ Player::Player1, AnimalType::Donkey }
	};

	//player 2 pieces
	m_player2Pieces = {
		{ Player::Player2, AnimalType::Frog },
		{ Player::Player2, AnimalType::Snake },
		{ Player::Player2, AnimalType::Donkey },
		{ Player::Player2, AnimalType::Donkey },
		{ Player::Player2, AnimalType::Donkey }
	};

	for (int i = 0; i < 5; ++i)
	{
		// initalize the texture & use the cell size
		m_player1Pieces[i].initAnimalTexture(cellSize);
		m_player2Pieces[i].initAnimalTexture(cellSize);

	}

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
		// Handle window resize *****STEPHEN THIS IS COOL! :D
		if (newEvent->is<sf::Event::Resized>())
		{
			const sf::Event::Resized* resizeEvent = newEvent->getIf<sf::Event::Resized>();

			// Update view to prevent stretching
			sf::View view(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(resizeEvent->size)));
			m_window.setView(view);

			// Rejigs the board and animals to new size
			m_board.updateCellSize(resizeEvent->size);
			updateAnimals();
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
	updateAnimals();
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}


void Game::render()
{
	m_window.clear(BLACK);

	m_board.draw(m_window);
	
	for (auto & piece : m_player1Pieces)
		piece.draw(m_window);

	for (auto& piece : m_player2Pieces)
		piece.draw(m_window);
	
	m_window.display();
}
/// <summary>
/// just a function to update the animal pieces positions and scales based on board size (resizing screen)
/// </summary>
void Game::updateAnimals()
{
	float cellSize = m_board.getCellSize();
	float boardLeft = (m_window.getSize().x - m_board.getSize() * cellSize) / 2.f;
	float boardTop = (m_window.getSize().y - m_board.getSize() * cellSize) / 2.f;
	float boardRight = boardLeft + m_board.getSize() * cellSize;

	for (int i = 0; i < 5; ++i)
	{
		float yPos = boardTop + (i + 0.5f) * cellSize;

		// Rescale and reposition Player 1 pieces
		m_player1Pieces[i].rescale(cellSize);
		sf::Vector2f pos1(boardLeft - cellSize * 1.2f, yPos);
		m_player1Pieces[i].setPosition(pos1);

		// Rescale and reposition Player 2 pieces
		m_player2Pieces[i].rescale(cellSize);
		sf::Vector2f pos2(boardRight + cellSize * 1.2f, yPos);
		m_player2Pieces[i].setPosition(pos2);
	}
}







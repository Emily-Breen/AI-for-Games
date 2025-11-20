
#include "Game.h"
#include <iostream>

Game::Game() : m_window{sf::VideoMode{sf::Vector2u{1920U, 1080U}, 32U}, "SFML Game 3.0"},
			   m_DELETEexitGame{false} // when true game will exit
{
	if (!m_jerseyFont.openFromFile("ASSETS/FONTS/Jersey20-Regular.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	m_winMessage.setFont(m_jerseyFont);
	m_winMessage.setCharacterSize(60);
	m_winMessage.setFillColor(sf::Color::Yellow);
	m_winMessage.setOutlineColor(sf::Color::Black);
	m_winMessage.setOutlineThickness(3.f);
	m_winMessage.setString("");

	m_board.updateCellSize(m_window.getSize());

	float cellSize = m_board.getCellSize();
	// player 1 pieces
	m_player1Pieces = {
		{Player::Player1, AnimalType::Frog},
		{Player::Player1, AnimalType::Snake},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey}};

	// player 2 pieces
	m_player2Pieces = {
		{Player::Player2, AnimalType::Frog},
		{Player::Player2, AnimalType::Snake},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey}};

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
	const float fps{60.0f};
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();	  // at least 60 fps
			update(timePerFrame); // 60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{

	while (const std::optional newEvent = m_window.pollEvent())
	{
		if (newEvent->is<sf::Event::Closed>()) // close window message
		{
			m_DELETEexitGame = true;
		}

		if (newEvent->is<sf::Event::KeyPressed>()) // user pressed a key
		{
			processKeys(newEvent);
		}
		// Handle window resize *****STEPHEN THIS IS COOL! :D
		if (newEvent->is<sf::Event::Resized>())
		{
			const sf::Event::Resized *resizeEvent = newEvent->getIf<sf::Event::Resized>();

			// Update view to prevent stretching
			sf::View view(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resizeEvent->size)));
			m_window.setView(view);

			// Rejigs the board and animals to new size
			m_board.updateCellSize(resizeEvent->size);
			updateAnimals();
		}

		// Handle mouse clicks and movement for drag-and-drop
		if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			const sf::Event::MouseButtonPressed *mouseEvent = newEvent->getIf<sf::Event::MouseButtonPressed>();
			if (mouseEvent->button == sf::Mouse::Button::Left && m_currentGameState == GameState::Placement)
			{
				handleMousePress(sf::Vector2i(mouseEvent->position));
			}
		}
		if (newEvent->is<sf::Event::MouseButtonReleased>())
		{
			const sf::Event::MouseButtonReleased *mouseEvent = newEvent->getIf<sf::Event::MouseButtonReleased>();
			if (mouseEvent->button == sf::Mouse::Button::Left && m_currentGameState == GameState::Placement)
			{
				handleMouseRelease(sf::Vector2i(mouseEvent->position));
			}
		}
		if (newEvent->is<sf::Event::MouseMoved>())
		{
			const sf::Event::MouseMoved *mouseEvent = newEvent->getIf<sf::Event::MouseMoved>();
			if (m_currentGameState == GameState::Placement)
			{
				handleMouseMoved(sf::Vector2i(mouseEvent->position));
			}
		}
	}
}

void Game::handleMousePress(sf::Vector2i mousePos)
{
	int piecesPlaced;
	std::vector<Animal>* pieces = nullptr;

	// Determine which player's pieces to check
	if (m_currentPlayer == Player::Player1)
	{
		pieces = &m_player1Pieces;
		piecesPlaced = m_player1PiecesPlaced;
	}
	else
	{
		pieces = &m_player2Pieces;
		piecesPlaced = m_player2PiecesPlaced;
	}

	// Check if clicking on an unplaced piece
	for (int i = 0; i < pieces->size(); ++i)
	{
		sf::FloatRect bounds = (*pieces)[i].getBounds();
		if (bounds.contains({ (float)mousePos.x, (float)mousePos.y }))
		{
			m_isDragging = true;
			m_draggedPieceIndex = i;
			m_draggedPiece = &(*pieces)[i];
			break;
		}
	}
}

void Game::handleMouseMoved(sf::Vector2i mousePos)
{
	if (m_isDragging && m_draggedPiece != nullptr)
	{
		// Update dragged piece position to follow mouse
		m_draggedPiece->setPosition({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
	}
}

void Game::handleMouseRelease(sf::Vector2i mousePos)
{
	// Need to make sure we were dragging a piece
	if (!m_isDragging || m_draggedPiece == nullptr)
		return;

	// Calculate board position and cell size
	float cellSize = m_board.getCellSize();
	float boardLeft = (m_window.getSize().x - m_board.getSize() * cellSize) / 2.f;
	float boardTop = (m_window.getSize().y - m_board.getSize() * cellSize) / 2.f;

	// Convert mouse position to grid coordinates
	int col = static_cast<int>((mousePos.x - boardLeft) / cellSize);
	int row = static_cast<int>((mousePos.y - boardTop) / cellSize);

	// Check if dropped on valid empty cell
	if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && m_grid[row][col].isEmpty())
	{
		// Place the piece
		Player owner = m_draggedPiece->getOwner();
		AnimalType type = m_draggedPiece->getType();

		m_grid[row][col] = Animal(owner, type);
		m_grid[row][col].initAnimalTexture(m_board.getCellSize());
		m_grid[row][col].setPosition(m_board.getCellCenter(row, col));

		// Remove from side list
		if (m_currentPlayer == Player::Player1)
		{
			m_player1Pieces.erase(m_player1Pieces.begin() + m_draggedPieceIndex);
		}
		else
		{
			m_player2Pieces.erase(m_player2Pieces.begin() + m_draggedPieceIndex);
		}

		// Immediately reset drag state BEFORE anything else can use the old index
		m_draggedPiece = nullptr;
		m_draggedPieceIndex = -1;

		// Check for win condition
		if (checkWinCondition())
		{
			m_winner = m_currentPlayer;
			switchGameState(GameState::GameOver);
			m_isDragging = false;
			m_draggedPiece = nullptr;
			m_draggedPieceIndex = -1;
			return;
		}

		// Check if all pieces are placed
		if (m_player1PiecesPlaced == 5 && m_player2PiecesPlaced == 5)
		{
			switchGameState(GameState::Movement);
			m_isDragging = false;
			m_draggedPiece = nullptr;
			m_draggedPieceIndex = -1;
			return;
		}

		// Switch player
		if (m_currentPlayer == Player::Player1)
			m_currentPlayer = Player::Player2;
		else
			m_currentPlayer = Player::Player1;
	}
	else
	{
		// Return piece to original position if dropped in invalid location
		updateAnimals(); // This will reset all unplaced pieces to their original positions
	}

	// Reset draging state variables back to default
	m_isDragging = false;
	m_draggedPiece = nullptr;
	m_draggedPieceIndex = -1;
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

	if (m_currentGameState == GameState::Placement)
	{
		if (m_currentGameState == GameState::Placement)
		{
			updateAnimals();  
		}
	}

	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(BLACK);

	m_board.draw(m_window);

	// Draw pieces on the board
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			m_grid[row][col].draw(m_window);
		}
	}

	// Draw unplaced pieces during placement phase
	if (m_currentGameState == GameState::Placement)
	{
		for (int i = 0; i < m_player1Pieces.size(); ++i)
			m_player1Pieces[i].draw(m_window);

		for (int i = 0; i < m_player2Pieces.size(); ++i)
			m_player2Pieces[i].draw(m_window);
	}

	if (m_currentGameState == GameState::GameOver)
	{
		m_window.draw(m_winMessage);
	}

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

	
	for (int i = 0; i < m_player1Pieces.size(); i++)
	{
		if (m_isDragging && m_draggedPiece == &m_player1Pieces[i])
			continue; 

		m_player1Pieces[i].rescale(cellSize);
		m_player1Pieces[i].setPosition({
			boardLeft - cellSize * 1.2f,
			boardTop + (i + 0.5f) * cellSize
			});
	}

	
	for (int i = 0; i < m_player2Pieces.size(); i++)
	{
		if (m_isDragging && m_draggedPiece == &m_player2Pieces[i])
			continue; 

		m_player2Pieces[i].rescale(cellSize);
		m_player2Pieces[i].setPosition({
			boardRight + cellSize * 1.2f,
			boardTop + (i + 0.5f) * cellSize
			});
	}

	
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			if (!m_grid[row][col].isEmpty())
			{
				m_grid[row][col].rescale(cellSize);
				m_grid[row][col].setPosition(m_board.getCellCenter(row, col));
			}
		}
	}


	
}

bool Game::checkWinCondition()
{
	// Loop through the whole board
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			// Horizontal check
			for (int row = 0; row < BOARD_SIZE; ++row)
			{
				for (int col = 0; col <= BOARD_SIZE - 4; ++col)
				{
					int count = 0;
					for (int i = 0; i < 4; ++i)
					{
						if (!m_grid[row][col + i].isEmpty() &&
							m_grid[row][col + i].getOwner() == m_currentPlayer)
							count++;
					}
					if (count == 4)
						return true;
				}
			}

			// Vertical check
			for (int col = 0; col < BOARD_SIZE; ++col)
			{
				for (int row = 0; row <= BOARD_SIZE - 4; ++row)
				{
					int count = 0;
					for (int i = 0; i < 4; ++i)
					{
						if (!m_grid[row + i][col].isEmpty() &&
							m_grid[row + i][col].getOwner() == m_currentPlayer)
							count++;
					}
					if (count == 4)
						return true;
				}
			}

			// For diagonal checks, only two directions are needed,
			// Since the inverse of these will be covered by those two checks
			// Checks all possible diagonal win conditions starting at the top-left tile
			for (int startRow = 0; startRow <= BOARD_SIZE - 4; ++startRow)
			{
				for (int startCol = 0; startCol <= BOARD_SIZE - 4; ++startCol)
				{
					int count = 0;
					for (int i = 0; i < 4; ++i)
					{
						if (!m_grid[startRow + i][startCol + i].isEmpty() &&
							m_grid[startRow + i][startCol + i].getOwner() == m_currentPlayer)
							count++;
					}
					if (count == 4)
						return true;
				}
			}

			// Checks all possible diagonal win conditions starting at the bottom-left tile
			for (int startRow = 3; startRow < BOARD_SIZE; ++startRow)
			{
				for (int startCol = 0; startCol <= BOARD_SIZE - 4; ++startCol)
				{
					int count = 0;
					for (int i = 0; i < 4; ++i)
					{
						if (!m_grid[startRow - i][startCol + i].isEmpty() &&
							m_grid[startRow - i][startCol + i].getOwner() == m_currentPlayer)
							count++;
					}
					if (count == 4)
						return true;
				}
			}
		}
	}
	return false;
}

void Game::switchGameState(GameState newState)
{
	m_currentGameState = newState;

	if (newState == GameState::GameOver)
	{
		std::string winText = (m_winner == Player::Player1) ? "Player 1 Wins!" : "Player 2 Wins!";
		m_winMessage.setString(winText);

		// Center the text above the board
		float cellSize = m_board.getCellSize();
		float boardTop = (m_window.getSize().y - m_board.getSize() * cellSize) / 2.f;

		sf::FloatRect textBounds = m_winMessage.getLocalBounds();
		m_winMessage.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
		m_winMessage.setPosition({ m_window.getSize().x / 2.f, boardTop - 80.f });
	}
}

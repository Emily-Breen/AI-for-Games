
#include "Game.h"
#include <iostream>
#include <cmath>
/**
 * @brief Constructs the Game object, loads fonts, initializes UI text,
 *        sets up menu buttons, creates starting pieces, and prepares the game window.
 *
 * This constructor:
 * - Loads the game font
 * - Sets up title, subtitle, credits text
 * - Initializes player pieces and textures
 * - Creates all main menu buttons
 * - Sets initial game state to MainMenu
 */
Game::Game() : m_window{ sf::VideoMode{sf::Vector2u{1920U, 1080U}, 32U}, "SFML Game 3.0" },
m_DELETEexitGame{ false } // when true game will exit
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

	m_winMessage2.setFont(m_jerseyFont);
	m_winMessage2.setCharacterSize(30);
	m_winMessage2.setFillColor(sf::Color::Yellow);
	m_winMessage2.setOutlineColor(sf::Color::Black);
	m_winMessage2.setOutlineThickness(3.f);
	m_winMessage2.setString("Press any key to return to the Main Menu");

	// Setup menu title
	m_menuTitle.setFont(m_jerseyFont);
	m_menuTitle.setCharacterSize(80);
	m_menuTitle.setFillColor(sf::Color::Yellow);
	m_menuTitle.setOutlineColor(sf::Color::Black);
	m_menuTitle.setOutlineThickness(4.f);
	m_menuTitle.setString("Fourth Protocol with Minimax");

	// Center the title
	sf::FloatRect titleBounds = m_menuTitle.getLocalBounds();
	m_menuTitle.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
	m_menuTitle.setPosition({ m_window.getSize().x / 2.f, 100.f });

	// Setup menu subtitle
	m_menuSubtitle.setFont(m_jerseyFont);
	m_menuSubtitle.setCharacterSize(40);
	m_menuSubtitle.setFillColor(sf::Color::White);
	m_menuSubtitle.setString("Choose a game mode:");

	// Center the subtitle
	sf::FloatRect subtitleBounds = m_menuSubtitle.getLocalBounds();
	m_menuSubtitle.setOrigin({ subtitleBounds.size.x / 2.f, subtitleBounds.size.y / 2.f });
	m_menuSubtitle.setPosition({ m_window.getSize().x / 2.f, 220.f });

	// Setup animated credits text
	m_menuCredits.setFont(m_jerseyFont);
	m_menuCredits.setCharacterSize(28);
	m_menuCredits.setFillColor(sf::Color::Yellow);
	m_menuCredits.setString("By Emily and Stephen!");

	// Position to the right of the title
	m_menuCredits.setPosition({ m_window.getSize().x / 2.f + 380.f, 180.f });

	m_board.updateCellSize(m_window.getSize());

	float cellSize = m_board.getCellSize();
	// player 1 pieces
	m_player1Pieces = {
		{Player::Player1, AnimalType::Frog},
		{Player::Player1, AnimalType::Snake},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey} };

	// player 2 pieces
	m_player2Pieces = {
		{Player::Player2, AnimalType::Frog},
		{Player::Player2, AnimalType::Snake},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey} };

	for (int i = 0; i < 5; ++i)
	{
		// initalize the texture & use the cell size
		m_player1Pieces[i].initAnimalTexture(cellSize);
		m_player2Pieces[i].initAnimalTexture(cellSize);
	}

	std::cout << "Game initialized. Player 1 starts in Placement phase.\n";
	m_currentGameState = GameState::MainMenu;

	// menu buttons
	m_btnHvH = new MenuButton(m_jerseyFont);
	m_btnHvAI = new MenuButton(m_jerseyFont);
	m_btnAIvAI = new MenuButton(m_jerseyFont);
	m_btnQuit = new MenuButton(m_jerseyFont);

	setupButtons(m_btnHvH, m_jerseyFont, "Human vs Human", 300.f);
	setupButtons(m_btnHvAI, m_jerseyFont, "Human vs AI", 420.f);
	setupButtons(m_btnAIvAI, m_jerseyFont, "AI vs AI", 540.f);
	setupButtons(m_btnQuit, m_jerseyFont, "Quit", 660.f);

}
/**
 * @brief Default destructor for the Game class.
 */
Game::~Game()
{
}
/**
 * @brief Main game loop running at 60 FPS.
 *
 * Handles event processing, updates, and rendering.
 */
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
/**
 * @brief Polls and handles all events (mouse, keyboard, close, resize).
 *
 * Handles:
 * - Closing the window
 * - Key presses
 * - Resizing and view adjustment
 * - Mouse dragging, movement, releasing
 */
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
		// Handle window resize
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
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				handleMousePress(sf::Vector2i(mouseEvent->position));
			}
		}
		if (newEvent->is<sf::Event::MouseButtonReleased>())
		{
			const sf::Event::MouseButtonReleased *mouseEvent = newEvent->getIf<sf::Event::MouseButtonReleased>();
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				handleMouseRelease(sf::Vector2i(mouseEvent->position));
			}
		}
		if (newEvent->is<sf::Event::MouseMoved>())
		{
			const sf::Event::MouseMoved *mouseEvent = newEvent->getIf<sf::Event::MouseMoved>();
			handleMouseMoved(sf::Vector2i(mouseEvent->position));
		}
	}
}

/**
 * @brief Converts a pixel coordinate to a grid coordinate based on board position.
 * @param screenPos Mouse coordinate in screen pixels.
 * @return Grid coordinate (row, col).
 */
sf::Vector2i Game::screenToGrid(sf::Vector2i screenPos) const
{
	float cellSize = m_board.getCellSize();
	float boardLeft = (m_window.getSize().x - m_board.getSize() * cellSize) / 2.f;
	float boardTop = (m_window.getSize().y - m_board.getSize() * cellSize) / 2.f;

	int col = static_cast<int>((screenPos.x - boardLeft) / cellSize);
	int row = static_cast<int>((screenPos.y - boardTop) / cellSize);

	return {row, col};
}
/**
 * @brief Handles mouse input when pressing the left mouse button.
 *
 * Behavior depends on the current game state:
 * - MainMenu: click buttons
 * - Placement: pick up unplaced pieces
 * - Movement: select a piece already on the board
 *
 * @param mousePos Pixel position of the mouse.
 */
void Game::handleMousePress(sf::Vector2i mousePos)
{
	if (m_currentGameState == GameState::MainMenu)
	{
		sf::Vector2f mouse(mousePos);

		// Lambda to check if mouse is over a button
		auto hit = [&](MenuButton* b)
			{
				return b->box.getGlobalBounds().contains(mouse);
			};

		if (hit(m_btnHvH))
		{
			m_player1IsAI = false;
			m_player2IsAI = false;
			m_currentGameState = GameState::Placement;
			return;
		}
		if (hit(m_btnHvAI))
		{
			m_player1IsAI = false;
			m_player2IsAI = true;
			m_currentGameState = GameState::Placement;
			return;
		}
		if (hit(m_btnAIvAI))
		{
			m_player1IsAI = true;
			m_player2IsAI = true;
			m_currentGameState = GameState::Placement;
			return;
		}
		if (hit(m_btnQuit))
		{
			m_window.close();
			return;
		}
	}
	// PLACEMENT PHASE: Check if clicking on unplaced piece
	if (m_currentGameState == GameState::Placement)
	{
		int piecesPlaced;
		std::vector<Animal> *pieces = nullptr;

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
			if (bounds.contains({(float)mousePos.x, (float)mousePos.y}))
			{
				m_isDragging = true;
				m_draggedPieceIndex = i;
				m_draggedPiece = &(*pieces)[i];
				break;
			}
		}
	}
	// MOVEMENT PHASE: Check if clicking on current player's piece on the board
	else if (m_currentGameState == GameState::Movement)
	{
		sf::Vector2i gridPos = screenToGrid(mousePos);
		int row = gridPos.x;
		int col = gridPos.y;

		std::cout << "Click at grid (" << col << "," << row << "), Current player: "
			<< (m_currentPlayer == Player::Player1 ? "Player1" : "Player2") << "\n";

		// Check if click is within board bounds
		if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
			return;

		// Check if clicking on current player's piece
		if (!m_grid[row][col].isEmpty() && m_grid[row][col].getOwner() == m_currentPlayer)
		{
			std::cout << "Cell has: " << m_grid[row][col].getName()
				<< " owned by Player" << (m_grid[row][col].getOwner() == Player::Player1 ? "1" : "2") << "\n";

			// Select this piece
			m_isPieceSelected = true;
			m_isDragging = true;
			m_selectedCell = {row, col};
			m_originalCell = {row, col};
			m_selectedPiece = &m_grid[row][col];

			// Calculate valid moves for this piece
			m_validMoves = m_selectedPiece->getValidMoves(row, col, &m_grid[0][0], BOARD_SIZE);

			std::cout << "Selected " << m_selectedPiece->getName()
					  << " at (" << col << ", " << row << ") with "
					  << m_validMoves.size() << " valid moves\n";
		}
	}
}
/**
 * @brief Updates the position of dragged pieces while holding the mouse.
 *
 * Works for both:
 * - Placement phase (dragging unplaced pieces)
 * - Movement phase (dragging selected board pieces)
 *
 * @param mousePos Pixel position.
 */
void Game::handleMouseMoved(sf::Vector2i mousePos)
{
	if (m_currentGameState == GameState::Placement)
	{
		// PLACEMENT PHASE: Update dragged piece position
		if (m_isDragging && m_draggedPiece != nullptr)
		{
			m_draggedPiece->setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
		}
	}
	else if (m_currentGameState == GameState::Movement)
	{
		// MOVEMENT PHASE: Update selected piece position
		if (m_isDragging && m_selectedPiece != nullptr)
		{
			m_selectedPiece->setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
		}
	}
}
/**
 * @brief Handles releasing a dragged piece.
 *
 * Behavior:
 * - Placement phase: drop piece into grid cell if valid
 * - Movement phase: move piece to a valid move location
 *
 * @param mousePos Pixel position where mouse was released.
 */
void Game::handleMouseRelease(sf::Vector2i mousePos)
{
	if (m_currentGameState == GameState::Placement)
	{
		// PLACEMENT PHASE: Try to place piece on board
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
			if (m_player1Pieces.empty() && m_player2Pieces.empty())
			{
				m_currentPlayer = Player::Player1; // Reset to Player 1 for movement phase
				switchGameState(GameState::Movement);
				std::cout << "\n*** ALL PIECES PLACED! ***\n";
				std::cout << "Entering Movement phase.\n";
				std::cout << "Player 1's turn to move.\n\n";
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

		// Reset dragging state variables back to default
		m_isDragging = false;
		m_draggedPiece = nullptr;
		m_draggedPieceIndex = -1;
	}
	else if (m_currentGameState == GameState::Movement)
	{
		// MOVEMENT PHASE: Try to move piece to valid location
		if (!m_isDragging || m_selectedPiece == nullptr)
			return;

		sf::Vector2i gridPos = screenToGrid(mousePos);
		int targetRow = gridPos.x;
		int targetCol = gridPos.y;

		bool validMove = false;

		// Check if the target position is in the list of valid moves
		for (const auto &move : m_validMoves)
		{
			if (move.x == targetRow && move.y == targetCol)
			{
				validMove = true;
				break;
			}
		}

		if (validMove)
		{
			std::cout << "Moving piece from (" << m_originalCell.x << ", " << m_originalCell.y
					  << ") to (" << targetRow << ", " << targetCol << ")\n";

			// Move the piece
			m_grid[targetRow][targetCol] = m_grid[m_originalCell.x][m_originalCell.y];
			m_grid[targetRow][targetCol].initAnimalTexture(m_board.getCellSize());
			m_grid[targetRow][targetCol].setPosition(m_board.getCellCenter(targetRow, targetCol));

			// Clear the original cell
			m_grid[m_originalCell.x][m_originalCell.y] = Animal();

			// Check for win condition
			if (checkWinCondition())
			{
				m_winner = m_currentPlayer;
				switchGameState(GameState::GameOver);

				// Reset selection state
				m_isPieceSelected = false;
				m_isDragging = false;
				m_selectedPiece = nullptr;
				m_validMoves.clear();
				return;
			}

			// Switch player
			m_currentPlayer = (m_currentPlayer == Player::Player1) ? Player::Player2 : Player::Player1;
			std::cout << "Turn switched to Player " << (m_currentPlayer == Player::Player1 ? "1" : "2") << "\n";
		}
		else
		{
			std::cout << "Invalid move - returning piece to original position\n";
			// Return piece to original position
			m_selectedPiece->setPosition(m_board.getCellCenter(m_originalCell.x, m_originalCell.y));
		}

		// Reset selection state
		m_isPieceSelected = false;
		m_isDragging = false;
		m_selectedPiece = nullptr;
		m_validMoves.clear();
	}
}
/**
 * @brief Handles keyboard key-press events.
 *
 * ESC closes the game.
 * Any key during GameOver returns to main menu via resetGame().
 *
 * @param t_event: KeyPressed event.
 */
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true;
	}
	//press any key to reset after game over
	if (m_currentGameState == GameState::GameOver)
	{
		resetGame();
	}
}
/**
 * @brief Continuous keyboard state checking (ESC).
 */
void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true;
	}
}
/**
 * @brief Updates game logic every frame.
 *
 * Handles:
 * - Main menu animation
 * - Placement logic
 * - Movement logic
 * - AI turns for both placement and movement phases
 * - Closing window
 *
 * @param t_deltaTime Time since last update tick.
 */
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();

	// Animate credits text on main menu
	if (m_currentGameState == GameState::MainMenu)
	{
		float time = m_animationClock.getElapsedTime().asSeconds();

		// Create a pulsing effect using sine wave
		// Period of ~3 seconds for a slow pulse
		float scale = 1.0f + 0.15f * std::sin(time * 2.0f);

		// Apply the scale
		m_menuCredits.setScale({ scale, scale });
	} 

	if (m_currentGameState == GameState::Placement)
	{
		updateAnimals();

		//check is the current player is AI
		if ((m_player1IsAI && m_currentPlayer == Player::Player1) ||
			(m_player2IsAI && m_currentPlayer == Player::Player2))
		{
			// Collect all empty positions
			std::vector<sf::Vector2i> emptyPositions;
			for (int row = 0; row < BOARD_SIZE; row++)
			{
				for (int col = 0; col < BOARD_SIZE; col++)
				{
					if (m_grid[row][col].isEmpty())
					{
						emptyPositions.push_back({ row, col });
					}
				}
			}

			// If there are empty positions and pieces to place
			auto& pieces = (m_currentPlayer == Player::Player1)
				? m_player1Pieces : m_player2Pieces;

			if (!emptyPositions.empty() && !pieces.empty())
			{
				// Randomly select an empty position
				int randomIndex = rand() % emptyPositions.size();
				sf::Vector2i chosenPos = emptyPositions[randomIndex];
				int row = chosenPos.x;
				int col = chosenPos.y;

				// Get the piece info before removing it
				Player owner = pieces.back().getOwner();
				AnimalType type = pieces.back().getType();

				// Create new piece in grid at randomly chosen position
				m_grid[row][col] = Animal(owner, type);

				// Initialize texture and position
				m_grid[row][col].initAnimalTexture(m_board.getCellSize());
				m_grid[row][col].setPosition(m_board.getCellCenter(row, col));

				//removes from the list
				pieces.pop_back();

				// Check for win condition after placing piece
				if (checkWinCondition())
				{
					m_winner = m_currentPlayer;
					switchGameState(GameState::GameOver);
					std::cout << "Player " << (m_currentPlayer == Player::Player1 ? "1" : "2")
						<< " wins during placement phase!\n";
					return;
				}

				// checks if the pieces are in place
				if (m_player1Pieces.empty() && m_player2Pieces.empty())
				{
					m_currentPlayer = Player::Player1; // Reset to Player 1 for movement
					switchGameState(GameState::Movement);
					std::cout << "\n*** ALL PIECES PLACED! ***\n";
					std::cout << "Entering Movement phase.\n";
					std::cout << "Player 1's turn to move.\n\n";
					return;
				}

				// Switch to next player
				m_currentPlayer = (m_currentPlayer == Player::Player1)
					? Player::Player2 : Player::Player1;

				return; // one placement per frame
			}
		}
	}

	// Handle AI turn during movement phase
	if (m_currentGameState == GameState::Movement) {
		// Check if the current player is AI 
		bool currentPlayerIsAI = (m_currentPlayer == Player::Player1 && m_player1IsAI) ||
			(m_currentPlayer == Player::Player2 && m_player2IsAI);
		//if the current player is AI and not dragging then handle AI turn
		if (currentPlayerIsAI && !m_isDragging) {
			static sf::Clock aiThinkTimer;
			static bool aiStarted = false;

			if (!aiStarted && aiThinkTimer.getElapsedTime().asSeconds() > 1.0f) {
				aiStarted = true;
				handleAITurn();
				aiStarted = false;
				aiThinkTimer.restart();
			}

			// Reset timer when player switches to AI
			if (aiThinkTimer.getElapsedTime().asSeconds() < 0.1f) {
				aiStarted = false;
			}
		}
	}

	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}
/**
 * @brief Draws visual highlights for the selected piece and its valid moves.
 *
 * Highlights:
 * - Yellow square for selected piece
 * - Green circles for all valid move locations
 *
 * @param window Render window to draw to.
 */
void Game::drawValidMoveHighlights(sf::RenderWindow &window)
{
	if (!m_isPieceSelected || m_validMoves.empty())
		return;

	float cellSize = m_board.getCellSize();

	// Draw a highlight on the selected cell
	sf::RectangleShape selectedHighlight(sf::Vector2f(cellSize - 4, cellSize - 4));
	selectedHighlight.setFillColor(sf::Color(255, 255, 0, 80)); // Semi-transparent yellow
	selectedHighlight.setOutlineColor(sf::Color::Yellow);
	selectedHighlight.setOutlineThickness(2.0f);

	sf::Vector2f selectedPos = m_board.getCellCenter(m_originalCell.x, m_originalCell.y);
	selectedHighlight.setPosition({selectedPos.x - cellSize / 2 + 2, selectedPos.y - cellSize / 2 + 2});
	window.draw(selectedHighlight);

	// Draw highlights for all valid moves
	for (const auto &move : m_validMoves)
	{
		sf::CircleShape highlight(cellSize * 0.15f);
		highlight.setFillColor(sf::Color(0, 255, 0, 150)); // Semi-transparent green
		highlight.setOutlineColor(sf::Color::Green);
		highlight.setOutlineThickness(2.0f);

		sf::Vector2f cellCenter = m_board.getCellCenter(move.x, move.y);
		highlight.setOrigin({cellSize * 0.15f, cellSize * 0.15f});
		highlight.setPosition(cellCenter);

		window.draw(highlight);
	}
}
/**
 * @brief Renders the entire game depending on the current game state.
 *
 * Draws:
 * - Board
 * - Pieces
 * - Dragged piece
 * - Valid move highlights
 * - Main menu UI
 * - Game over messages
 */
void Game::render()
{
	m_window.clear(BLACK);

	m_board.draw(m_window);

	// Draw valid move highlights during movement phase
	if (m_currentGameState == GameState::Movement)
	{
		drawValidMoveHighlights(m_window);
	}

	// Draw pieces on the board
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			// Don't draw the piece we're currently dragging from its original position
			if (m_isDragging && m_selectedCell.x == row && m_selectedCell.y == col)
				continue;

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

	// Draw the dragged piece on top of everything else
	if (m_isDragging && m_selectedPiece != nullptr)
	{
		m_selectedPiece->draw(m_window);
	}

	if (m_currentGameState == GameState::GameOver)
	{
		m_window.draw(m_winMessage);
		m_window.draw(m_winMessage2);
	}
	//draw the main menu
	if (m_currentGameState == GameState::MainMenu)
	{
		m_window.clear(sf::Color(20, 20, 20));

		m_window.draw(m_menuTitle);
		m_window.draw(m_menuSubtitle);
		m_window.draw(m_menuCredits);

		m_window.draw(m_btnHvH->box);
		m_window.draw(m_btnHvH->label);

		m_window.draw(m_btnHvAI->box);
		m_window.draw(m_btnHvAI->label);

		m_window.draw(m_btnAIvAI->box);
		m_window.draw(m_btnAIvAI->label);

		m_window.draw(m_btnQuit->box);
		m_window.draw(m_btnQuit->label);

		m_window.display();
		return;
	}
	m_window.display();
}
/**
 * @brief Rescales and repositions all pieces when the board or window is resized.
 */
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
		m_player1Pieces[i].setPosition({boardLeft - cellSize * 1.2f,
										boardTop + (i + 0.5f) * cellSize});
	}

	for (int i = 0; i < m_player2Pieces.size(); i++)
	{
		if (m_isDragging && m_draggedPiece == &m_player2Pieces[i])
			continue;

		m_player2Pieces[i].rescale(cellSize);
		m_player2Pieces[i].setPosition({boardRight + cellSize * 1.2f,
										boardTop + (i + 0.5f) * cellSize});
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
/**
 * @brief Checks all win conditions (horizontal, vertical, diagonal).
 * @return true if the current player has 4 in a row.
 */
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
/**
 * @brief Switches the active game state and updates UI text for GameOver.
 * @param newState New state to switch to.
 */
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
		m_winMessage.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
		m_winMessage.setPosition({m_window.getSize().x / 2.f, boardTop - 80.f});

		sf::FloatRect textBounds2 = m_winMessage2.getLocalBounds();
		m_winMessage2.setOrigin({ textBounds2.size.x / 2.f, textBounds2.size.y / 2.f });
		m_winMessage2.setPosition({ m_window.getSize().x / 2.f, m_window.getSize().y - 150.f });
	}
}
/**
 * @brief Configures the appearance and position of a main menu button.
 * @param btn Button to configure.
 * @param font Font used for the label text.
 * @param text Text appearing on the button.
 * @param y Vertical position on screen.
 */
void Game::setupButtons(MenuButton* btn, sf::Font& font, const std::string& text, float y)
{
	float width = 400.f;
	float height = 80.f;
	float x = (m_window.getSize().x - width) / 2.f; // Center horizontally

	
	btn->box.setSize(sf::Vector2f(width, height));
	btn->box.setPosition({ x, y });
	btn->box.setFillColor(sf::Color(sf::Color::Blue));
	btn->box.setOutlineColor(sf::Color::White);
	btn->box.setOutlineThickness(2.f);

	btn->label.setFont(font);
	btn->label.setString(text);
	btn->label.setCharacterSize(24);
	btn->label.setFillColor(sf::Color::White);

	
	sf::FloatRect textBounds = btn->label.getLocalBounds();
	btn->label.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
	btn->label.setPosition({ x + width / 2.f, y + height / 2.f - 5.f });
}
/**
 * @brief Fully resets the game after Game Over and sends the player back to MainMenu.
 */
void Game::resetGame()
{
	// Clear the board
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			m_grid[row][col] = Animal();
		}
	}

	float cellSize = m_board.getCellSize();

	// Reset player 1 pieces
	m_player1Pieces = {
		{Player::Player1, AnimalType::Frog},
		{Player::Player1, AnimalType::Snake},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey},
		{Player::Player1, AnimalType::Donkey}
	};

	// Reset player 2 pieces
	m_player2Pieces = {
		{Player::Player2, AnimalType::Frog},
		{Player::Player2, AnimalType::Snake},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey},
		{Player::Player2, AnimalType::Donkey}
	};

	// Initialize textures
	for (int i = 0; i < 5; ++i)
	{
		m_player1Pieces[i].initAnimalTexture(cellSize);
		m_player2Pieces[i].initAnimalTexture(cellSize);
	}

	// Reset game state variables
	m_currentPlayer = Player::Player1;
	m_winner = Player::NoPlayer;
	m_isDragging = false;
	m_draggedPieceIndex = -1;
	m_draggedPiece = nullptr;
	m_isPieceSelected = false;
	m_selectedCell = { -1, -1 };
	m_originalCell = { -1, -1 };
	m_selectedPiece = nullptr;
	m_validMoves.clear();

	// Return to main menu
	m_currentGameState = GameState::MainMenu;

	std::cout << "Game reset. Returning to main menu.\n";
}
/**
 * @brief Executes the AI's turn during the movement phase.
 *
 * Uses minimax to compute best move, applies it, checks win,
 * and switches the current player.
 */
void Game::handleAITurn()
{
	
	// Check if current player is AI
	if ((m_currentPlayer == Player::Player1 && !m_player1IsAI) ||
		(m_currentPlayer == Player::Player2 && !m_player2IsAI))
	{
		return;
	}

	std::cout << "AI ("
		<< (m_currentPlayer == Player::Player1 ? "P1" : "P2")
		<< ") is thinking...\n";

	Boardstate currentState = getCurrentBoardState();
	Move aiMove = m_aiPlayer.chooseBestMove(currentState, 3);

	if (!aiMove.isValid())
	{
		std::cout << "AI found no valid move.\n";
		return;
	}

	m_grid[aiMove.row2][aiMove.col2] = m_grid[aiMove.row1][aiMove.col1];
	m_grid[aiMove.row1][aiMove.col1] = Animal();

	// Move sprite
	m_grid[aiMove.row2][aiMove.col2].initAnimalTexture(m_board.getCellSize());
	m_grid[aiMove.row2][aiMove.col2].setPosition(m_board.getCellCenter(aiMove.row2, aiMove.col2));

	if (checkWinCondition())
	{
		m_winner = m_currentPlayer;
		switchGameState(GameState::GameOver);
		return;
	}

	// Switches between P1 and P2 
	m_currentPlayer = (m_currentPlayer == Player::Player1)
		? Player::Player2 : Player::Player1;
}


/**
 * @brief Converts the current game grid into a Boardstate used by the AI.
 * @return Boardstate version of m_grid.
 */
Boardstate Game::getCurrentBoardState() const
{
	Boardstate state;

	// Copy the grid
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			state.grid[row][col] = m_gameplay.toPieceState(m_grid[row][col]);
		}
	}

	// Set current player
	state.currentPlayer = m_currentPlayer;

	return state;
}

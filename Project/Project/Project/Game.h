#ifndef GAME_HPP
#define GAME_HPP
#pragma warning(push)
#pragma warning(disable : 4275)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.h"
#include "Animal.h"
#include "Gameplay.h"

/// @brief Background clear colour.
const sf::Color BLACK{ 0, 0, 0, 0 };

/**
 * @enum GameState
 * @brief Represents the current phase of the game.
 */
enum class GameState
{
	MainMenu,   ///< Player chooses Human/AI game mode
	Placement,  ///< Players place their pieces
	Movement,   ///< Players move placed pieces
	GameOver    ///< Win screen
};

/**
 * @enum GameMode
 * @brief Selected configuration of Human/AI involvement.
 */
enum class GameMode {
	HumanVSHuman, ///< Both players are human
	HumanVSAI,    ///< P1 human, P2 AI
	AIVSAI        ///< Both players controlled by AI
};

/**
 * @struct MenuButton
 * @brief UI button used on the main menu screen.
 */
struct MenuButton {
	sf::RectangleShape box; ///< Visual rectangle
	sf::Text label;         ///< Text on the button

	/**
	 * @brief Constructs a menu button with a font reference.
	 * @param font Font used for the label.
	 */
	MenuButton(const sf::Font& font)
		: label(font) {};
};

/**
 * @class Game
 * @brief Main controller for the entire game logic, rendering, input, AI, UI and state transitions.
 */
class Game
{
public:
	/**
	 * @brief Constructs the game, loads fonts, initializes UI and pieces.
	 */
	Game();

	/**
	 * @brief Destructor.
	 */
	~Game();

	/**
	 * @brief Main game loop.
	 */
	void run();

private:
	// ------------ Core Loop ------------
	void processEvents();   ///< Poll and handle SFML events
	void processKeys(const std::optional<sf::Event> t_event); ///< Handle key press events
	void checkKeyboardState(); ///< Check real-time keyboard input
	void update(sf::Time t_deltaTime); ///< Update game logic
	void render(); ///< Render all game graphics

	// ------------ Gameplay Logic ------------
	void updateAnimals(); ///< Update positions/scales when window resizes
	void handleMousePress(sf::Vector2i mousePos); ///< Handle mouse down
	void handleMouseRelease(sf::Vector2i mousePos); ///< Handle mouse release
	void handleMouseMoved(sf::Vector2i mousePos); ///< Handle mouse drag
	bool checkWinCondition(); ///< Checks 4-in-a-row for current player
	void switchGameState(GameState newState); ///< Transition between phases
	void resetGame(); ///< Reset everything back to MainMenu

	// ------------ UI Helpers ------------
	/**
	 * @brief Creates and positions a menu button.
	 * @param btn Button pointer to modify.
	 * @param font Font reference for the label.
	 * @param text Text to display.
	 * @param y Vertical placement.
	 */
	void setupButtons(MenuButton* btn, sf::Font& font, const std::string& text, float y);

	void drawValidMoveHighlights(sf::RenderWindow& window); ///< Highlights valid moves
	sf::Vector2i screenToGrid(sf::Vector2i screenPos) const; ///< Converts pixel coords to grid coords

	// ------------ Game Data ------------

	Board m_board{ 5, 150.f }; ///< 5×5 board
	std::vector<Animal> m_player1Pieces; ///< P1 unplaced pieces
	std::vector<Animal> m_player2Pieces; ///< P2 unplaced pieces
	Animal m_grid[BOARD_SIZE][BOARD_SIZE]{}; ///< Placed animals

	sf::RenderWindow m_window; ///< Main SFML window
	sf::Font m_jerseyFont; ///< Primary UI font

	// --- UI Text ---
	sf::Text m_winMessage{ m_jerseyFont };
	sf::Text m_winMessage2{ m_jerseyFont };
	sf::Text m_instructionText{ m_jerseyFont };
	sf::Text m_menuTitle{ m_jerseyFont };
	sf::Text m_menuSubtitle{ m_jerseyFont };
	sf::Text m_menuCredits{ m_jerseyFont };
	sf::Clock m_animationClock;

	// --- Old/unused items (kept for reference) ---
	sf::Text m_DELETEwelcomeMessage{ m_jerseyFont };
	sf::Texture m_DELETElogoTexture;
	sf::Sprite m_DELETElogoSprite{ m_DELETElogoTexture };

	// --- Game State / Turn Handling ---
	GameState m_currentGameState{ GameState::Placement };
	Player m_currentPlayer{ Player::Player1 };
	Player m_winner{ Player::NoPlayer };

	int m_player1PiecesPlaced{ 0 };
	int m_player2PiecesPlaced{ 0 };

	// --- Drag + Movement ---
	bool m_isDragging{ false };
	int m_draggedPieceIndex{ -1 };
	Animal* m_draggedPiece{ nullptr };

	// --- Selection ---
	bool m_isPieceSelected{ false };
	sf::Vector2i m_selectedCell{ -1, -1 };
	sf::Vector2i m_originalCell{ -1, -1 };
	std::vector<sf::Vector2i> m_validMoves;
	Animal* m_selectedPiece{ nullptr };

	// --- Exit flag ---
	bool m_DELETEexitGame;

	// --- AI Logic ---
	Gameplay m_aiPlayer;
	Gameplay m_gameplay;
	bool m_player2IsAI{ true };
	bool m_player1IsAI{ false };

	/**
	 * @brief Performs AI decision making for current player's turn.
	 */
	void handleAITurn();

	/**
	 * @brief Converts live grid to AI friendly Boardstate.
	 */
	Boardstate getCurrentBoardState() const;

	// --- Menu Buttons ---
	MenuButton* m_btnHvH{};
	MenuButton* m_btnHvAI{};
	MenuButton* m_btnAIvAI{};
	MenuButton* m_btnQuit{};
};

#pragma warning(pop)
#endif // !GAME_HPP

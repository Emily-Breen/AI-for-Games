
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning(push)
#pragma warning(disable : 4275)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.h"
#include "Animal.h"

const sf::Color BLACK{0, 0, 0, 0}; // const colour

enum class GameState
{
	Placement,
	Movement,
	GameOver
};

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
	void handleMousePress(sf::Vector2i mousePos);
	void handleMouseRelease(sf::Vector2i mousePos);
	void handleMouseMoved(sf::Vector2i mousePos);
	bool checkWinCondition();
	void switchGameState(GameState newState);

	void drawValidMoveHighlights(sf::RenderWindow& window);
	sf::Vector2i screenToGrid(sf::Vector2i screenPos) const;
	void updateInstructionsText();

	Board m_board{5, 150.f};
	std::vector<Animal> m_player1Pieces;
	std::vector<Animal> m_player2Pieces;
	Animal m_grid[BOARD_SIZE][BOARD_SIZE]{};
	sf::RenderWindow m_window;			 // main SFML window
	sf::Font m_jerseyFont;				 // font used by message
	sf::Text m_winMessage{m_jerseyFont}; // text used for win message
	sf::Text m_instructionText{ m_jerseyFont }; // text for turn/phase instructions

	// (to Emily) : keeping these here in case you want to reuse them at some point,
	// but I only use the text above these for the win message right now.
	sf::Text m_DELETEwelcomeMessage{m_jerseyFont};		// text used for message on screen
	sf::Texture m_DELETElogoTexture;					//  texture used for sfml logo
	sf::Sprite m_DELETElogoSprite{m_DELETElogoTexture}; // sprite used for sfml logo

	// Variables for placing and moving pieces, and swapping turns back and forth
	GameState m_currentGameState{GameState::Placement};
	Player m_currentPlayer{Player::Player1};

	// Tracks the winner
	Player m_winner{Player::NoPlayer};
	int m_player1PiecesPlaced{0};
	int m_player2PiecesPlaced{0};

	// Variables for drag-and-drop functionality of tiles
	bool m_isDragging{false};
	int m_draggedPieceIndex{-1};
	Animal *m_draggedPiece{nullptr};

	// Variables for movement phase
	bool m_isPieceSelected{ false };
	sf::Vector2i m_selectedCell{ -1, -1 };  // Grid position of selected piece
	sf::Vector2i m_originalCell{ -1, -1 };  // Original position before dragging
	std::vector<sf::Vector2i> m_validMoves;  // Valid move positions for selected piece
	Animal* m_selectedPiece{ nullptr };  // Pointer to the selected piece being moved

	bool m_DELETEexitGame; // control exiting game
};

#pragma warning(pop)
#endif // !GAME_HPP

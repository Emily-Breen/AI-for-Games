#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

/**
 * @enum Player
 * @brief Represents which player owns a piece.
 */
enum Player {
    NoPlayer,
    Player1,
    Player2
};

/**
 * @enum AnimalType
 * @brief Represents the type of animal.
 */
enum AnimalType {
    NoType,
    Frog,
    Snake,
    Donkey
};

/**
 * @class Animal
 * @brief Represents a single game piece placed on the board.
 *
 * Stores ownership, type, sprite, texture, and movement logic.
 */
class Animal
{
public:
    /**
     * @brief Default constructor. Creates an empty/unoccupied piece.
     */
    Animal(); // default

    /**
     * @brief Parameter constructor.
     * @param owner The player that owns this piece.
     * @param type The animal type (Frog, Snake, Donkey, etc.).
     */
    Animal(Player owner, AnimalType type); //param constructor

    /**
     * @brief Gets the name of the animal.
     * @return Name of the piece as a string.
     */
    std::string getName() const;

    /**
     * @brief Checks if this is an empty piece (no owner, no type).
     * @return True if this represents an empty cell.
     */
    bool isEmpty() const;

    /**
     * @brief Gets the owning player.
     * @return Player1, Player2, or NoPlayer.
     */
    Player getOwner() const;

    /**
     * @brief Gets the type of animal.
     * @return One of the AnimalType values.
     */
    AnimalType getType() const;

    /**
     * @brief Sets the on screen sprite position.
     * @param position Pixel coordinates.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Loads the correct sprite texture and scales it to cellSize.
     * @param cellSize The board cell size for scaling.
     */
    void initAnimalTexture(float cellSize);

    /**
     * @brief Draws the animal sprite to the window.
     * @param m_window The SFML render window.
     */
    void draw(sf::RenderWindow& m_window);

    /**
     * @brief Rescales the sprite when the board/cell size changes.
     * @param cellSize New size of board cells.
     */
    void rescale(float cellSize);

    /**
     * @brief Gets the bounding rectangle of the sprite.
     * @return FloatRect in world coordinates.
     */
    sf::FloatRect getBounds() const;

    /**
     * @brief Copy constructor.
     */
    Animal(const Animal& other);

    /**
     * @brief Copy assignment operator.
     * @return Reference to this object.
     */
    Animal& operator=(const Animal& other);

    /**
     * @brief Computes valid movement locations for this animal.
     *
     * @param currentRow Row of the piece.
     * @param currentCol Column of the piece.
     * @param grid Pointer to full board grid array.
     * @param boardSize Size of board.
     * @return A list of valid movement destination positions.
     */
    std::vector<sf::Vector2i> getValidMoves(int currentRow, int currentCol,
        const Animal* grid, int boardSize) const;

private:
    Player m_owner;
    AnimalType m_type;
    sf::Texture m_texture;
    sf::Sprite m_sprite{ m_texture };

    /**
     * @brief Checks if a position is within board bounds.
     */
    bool isValidPosition(int row, int col, int boardSize) const;

    /**
     * @brief Adds adjacent moves (cardinal + optional diagonal).
     */
    void addAdjacentMoves(int row, int col, const Animal* grid, int boardSize,
        std::vector<sf::Vector2i>& moves, bool includeDiagonal) const;

    /**
     * @brief Adds frog style multi tile jumping moves.
     */
    void addFrogJumpMoves(int row, int col, const Animal* grid, int boardSize,
        std::vector<sf::Vector2i>& moves) const;
};

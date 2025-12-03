/**
 * @file Animal.cpp
 * @brief Implements the Animal class which represents player game pieces,
 *        their textures, drawing, movement rules, and board interactions.
 */

#include "Animal.h"

 /**
  * @brief Default constructor. Creates an empty (NoPlayer, NoType) Animal.
  */
Animal::Animal() :m_owner(Player::NoPlayer), m_type(AnimalType::NoType)
{
}

/**
 * @brief Constructs an Animal with the given owner and type.
 * @param owner Player who owns the piece.
 * @param type  Type of animal.
 */
Animal::Animal(Player owner, AnimalType type) : m_owner(owner), m_type(type)
{
}

/**
 * @brief Copy constructor. Ensures sprite position and ownership are copied.
 * @param other The Animal to copy.
 */
Animal::Animal(const Animal& other)
{
    m_owner = other.m_owner;
    m_type = other.m_type;

    m_sprite.setPosition(other.m_sprite.getPosition());
}

/**
 * @brief Assignment operator. Prevents texture issues when copying pieces.
 * @param other The Animal to assign from.
 * @return Reference to *this.
 */
Animal& Animal::operator=(const Animal& other)
{
    if (this != &other)
    {
        m_owner = other.m_owner;
        m_type = other.m_type;

        initAnimalTexture(50);
        m_sprite.setPosition(other.m_sprite.getPosition());
    }
    return *this;
}

/**
 * @brief Returns the type of this Animal as a string.
 * @return "Frog", "Snake", "Donkey", or "Empty".
 */
std::string Animal::getName() const
{
    switch (m_type) {

    case AnimalType::Frog:
        return "Frog";
    case AnimalType::Snake:
        return "Snake";
    case AnimalType::Donkey:
        return "Donkey";
    default:
        return "Empty";

    }
}

/**
 * @brief Checks if this tile contains no Animal.
 * @return true if owner is NoPlayer.
 */
bool Animal::isEmpty() const
{
    return m_owner == Player::NoPlayer;
}

/**
 * @brief Gets the owning player of this piece.
 * @return Player1, Player2, or NoPlayer.
 */
Player Animal::getOwner() const
{
    return m_owner;
}

/**
 * @brief Gets the type of this animal.
 * @return Frog, Snake, Donkey, or NoType.
 */
AnimalType Animal::getType() const
{
    return m_type;
}

/**
 * @brief Sets the on screen position of the Animal's sprite.
 * @param position New world position.
 */
void Animal::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position);
}

/**
 * @brief Loads texture, sets texture rect, origin, and scale based on cell size.
 * @param cellSize Size of a board cell, used for scaling.
 */
void Animal::initAnimalTexture(float cellSize)
{
    std::string filename;

    switch (m_type) {
    case AnimalType::Frog:   filename = "ASSETS/IMAGES/frog.png";
        break;
    case AnimalType::Snake:  filename = "ASSETS/IMAGES/snake.png";
        break;
    case AnimalType::Donkey: filename = "ASSETS/IMAGES/donkey.png";
        break;
    case AnimalType::NoType:
        return;
    default:
        std::cout << "Name is wrong\n";
        return;
    }

    if (!m_texture.loadFromFile(filename)) {
        std::cout << "no texture loaded: " << filename << "\n";
        return;
    }

    sf::Vector2u texSize = m_texture.getSize();
    sf::Vector2i frameSize(texSize.x / 2, texSize.y);

    sf::IntRect textureRect;
    if (m_owner == Player::Player1) {
        textureRect = sf::IntRect({ 0, 0 }, frameSize);
    }
    else if (m_owner == Player::Player2) {
        textureRect = sf::IntRect({ static_cast<int>(frameSize.x), 0 }, frameSize);
    }
    else {
        textureRect = sf::IntRect({ 0, 0 }, frameSize);
    }

    m_sprite = sf::Sprite(m_texture, textureRect);

    m_sprite.setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));

    float scale = (cellSize * 0.8f) / std::max(frameSize.x, frameSize.y);
    m_sprite.setScale(sf::Vector2f(scale, scale));
}

/**
 * @brief Draws the animal sprite to the game window.
 * @param m_window The render window.
 */
void Animal::draw(sf::RenderWindow& m_window)
{
    if (!isEmpty())
        m_window.draw(m_sprite);
}

/**
 * @brief Adjusts sprite scale when the board is resized.
 * @param cellSize New board cell size.
 */
void Animal::rescale(float cellSize)
{
    if (isEmpty()) return;

    sf::Vector2u texSize = m_texture.getSize();
    sf::Vector2i frameSize(texSize.x / 2, texSize.y);

    sf::IntRect textureRect;
    if (m_owner == Player::Player1) {
        textureRect = sf::IntRect({ 0, 0 }, frameSize);
    }
    else if (m_owner == Player::Player2) {
        textureRect = sf::IntRect({ static_cast<int>(frameSize.x), 0 }, frameSize);
    }
    m_sprite.setTextureRect(textureRect);

    m_sprite.setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));

    float scale = (cellSize * 0.8f) / std::max(frameSize.x, frameSize.y);
    m_sprite.setScale(sf::Vector2f(scale, scale));
}

/**
 * @brief Returns global bounds of the sprite, useful for click detection.
 * @return FloatRect of sprite.
 */
sf::FloatRect Animal::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

/**
 * @brief Checks if a board coordinate is within valid range.
 */
bool Animal::isValidPosition(int row, int col, int boardSize) const
{
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
}

/**
 * @brief Returns a list of valid moves for the animal based on its type.
 * @param currentRow Current grid row.
 * @param currentCol Current grid column.
 * @param grid Pointer to full board of Animals.
 * @param boardSize Dimension of square board.
 * @return Vector of valid move coordinates.
 */
std::vector<sf::Vector2i> Animal::getValidMoves(int currentRow, int currentCol,
    const Animal* grid, int boardSize) const
{
    std::vector<sf::Vector2i> validMoves;

    if (isEmpty())
        return validMoves;

    switch (m_type)
    {
    case AnimalType::Donkey:
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, false);
        break;

    case AnimalType::Snake:
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, true);
        break;

    case AnimalType::Frog:
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, true);
        addFrogJumpMoves(currentRow, currentCol, grid, boardSize, validMoves);
        break;

    default:
        break;
    }

    return validMoves;
}

/**
 * @brief Adds all 1-step moves (cardinal & diagonal if allowed).
 */
void Animal::addAdjacentMoves(int row, int col, const Animal* grid, int boardSize,
    std::vector<sf::Vector2i>& moves, bool includeDiagonal) const
{
    const int cardinalDirs[4][2] = { {0, -1}, {0, 1}, {-1, -0}, {1, 0} };
    const int diagonalDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (int i = 0; i < 4; ++i)
    {
        int newRow = row + cardinalDirs[i][0];
        int newCol = col + cardinalDirs[i][1];

        if (isValidPosition(newRow, newCol, boardSize) &&
            grid[newRow * boardSize + newCol].isEmpty())
        {
            moves.push_back({ newRow, newCol });
        }
    }

    if (includeDiagonal)
    {
        for (int i = 0; i < 4; ++i)
        {
            int newRow = row + diagonalDirs[i][0];
            int newCol = col + diagonalDirs[i][1];

            if (isValidPosition(newRow, newCol, boardSize) &&
                grid[newRow * boardSize + newCol].isEmpty())
            {
                moves.push_back({ newRow, newCol });
            }
        }
    }
}

/**
 * @brief Adds frog jump moves (jump over any piece and land on next empty tile).
 */
void Animal::addFrogJumpMoves(int row, int col, const Animal* grid, int boardSize,
    std::vector<sf::Vector2i>& moves) const
{
    const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int i = 0; i < 8; ++i)
    {
        int dirRow = directions[i][0];
        int dirCol = directions[i][1];

        int checkRow = row + dirRow;
        int checkCol = col + dirCol;

        while (isValidPosition(checkRow, checkCol, boardSize))
        {
            if (!grid[checkRow * boardSize + checkCol].isEmpty())
            {
                int landRow = checkRow + dirRow;
                int landCol = checkCol + dirCol;

                if (isValidPosition(landRow, landCol, boardSize) &&
                    grid[landRow * boardSize + landCol].isEmpty())
                {
                    moves.push_back({ landRow, landCol });
                }

                break;
            }

            checkRow += dirRow;
            checkCol += dirCol;
        }
    }
}

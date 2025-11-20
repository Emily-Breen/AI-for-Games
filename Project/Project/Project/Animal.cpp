#include "Animal.h"

Animal::Animal() :m_owner(Player::NoPlayer), m_type(AnimalType::NoType)
{
}

Animal::Animal(Player owner, AnimalType type) : m_owner(owner), m_type(type)
{
}
//copy constructor and assignment operator to help with bug of sprites not displaying correctly
Animal::Animal(const Animal& other)
{
    m_owner = other.m_owner;
    m_type = other.m_type;

    
    m_sprite.setPosition(other.m_sprite.getPosition());
}
//assignment operator so when they move into the board grid the ones on the side dont overwrite to the sprite originally there 
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
//migh be handy when we want to have a combat log on screen or somthing?
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

bool Animal::isEmpty() const
{
	return m_owner == Player::NoPlayer;
}

Player Animal::getOwner() const
{
    return m_owner;
}

AnimalType Animal::getType() const
{
	return m_type;
}

void Animal::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position);
}

void Animal::initAnimalTexture(float cellSize)
{
    std::string filename;

    //sets to load based on animal type
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

    //gets the texture size and divides the sides left player right Ai or player2
    sf::Vector2u texSize = m_texture.getSize();
    sf::Vector2i frameSize(texSize.x / 2, texSize.y);

	//left side is player 1 right side is player 2
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
       

    // Create sprite using the right half of the texture
    m_sprite = sf::Sprite(m_texture, textureRect);

   //centering the origin for positioning
    m_sprite.setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));

   //fits it into the board scale
    float scale = (cellSize * 0.8f) / std::max(frameSize.x, frameSize.y);
    m_sprite.setScale(sf::Vector2f(scale, scale));
}

void Animal::draw(sf::RenderWindow& m_window)
{
	//if the enum isnt noPlayer draw it
    if (!isEmpty())
        m_window.draw(m_sprite);
}

void Animal::rescale(float cellSize)
{
    if (isEmpty()) return;

    // Get the frame size from the texture
    sf::Vector2u texSize = m_texture.getSize();
    sf::Vector2i frameSize(texSize.x / 2, texSize.y);

    // Ensure the texture rect is set correctly (this was causing issues with textures getting replaced)
    sf::IntRect textureRect;
    if (m_owner == Player::Player1) {
        textureRect = sf::IntRect({ 0, 0 }, frameSize);
    }
    else if (m_owner == Player::Player2) {
        textureRect = sf::IntRect({ static_cast<int>(frameSize.x), 0 }, frameSize);
    }
    m_sprite.setTextureRect(textureRect);

    // Reset origin to ensure it's centered
    m_sprite.setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));

    // Recalculate scale based on new cell size
    float scale = (cellSize * 0.8f) / std::max(frameSize.x, frameSize.y);
    m_sprite.setScale(sf::Vector2f(scale, scale));
}

sf::FloatRect Animal::getBounds() const
{
	return m_sprite.getGlobalBounds();
}

// Check if a position is within the board boundaries
bool Animal::isValidPosition(int row, int col, int boardSize) const
{
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
}

// Main method to get all valid moves for this animal
std::vector<sf::Vector2i> Animal::getValidMoves(int currentRow, int currentCol,
    const Animal* grid, int boardSize) const
{
    std::vector<sf::Vector2i> validMoves;

    // Empty cells can't move
    if (isEmpty())
        return validMoves;

    switch (m_type)
    {
    case AnimalType::Donkey:
        // Donkey: one cardinal move only (no diagonals)
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, false);
        break;

    case AnimalType::Snake:
        // Snake: one space in any direction (including diagonals)
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, true);
        break;

    case AnimalType::Frog:
        // Frog: one space in any direction (including diagonals)
        addAdjacentMoves(currentRow, currentCol, grid, boardSize, validMoves, true);
        // PLUS jumping over pieces
        addFrogJumpMoves(currentRow, currentCol, grid, boardSize, validMoves);
        break;

    default:
        break;
    }

    return validMoves;
}

// Add adjacent moves (cardinal and optionally diagonal) to the moves vector for valid moves
void Animal::addAdjacentMoves(int row, int col, const Animal* grid, int boardSize,
    std::vector<sf::Vector2i>& moves, bool includeDiagonal) const
{
	// Cardinal directions: up, down, left, right
    const int cardinalDirs[4][2] = { {0, -1}, {0, 1}, {-1, -0}, {1, 0} };

	// Diagonal directions: top-left, bottom-left, top-right, bottom-right
    const int diagonalDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    // Check orthogonal moves
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

    // Check diagonal moves if allowed
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


// Add frog's special jump moves
void Animal::addFrogJumpMoves(int row, int col, const Animal* grid, int boardSize,
    std::vector<sf::Vector2i>& moves) const
{
    // All 8 directions (cardinal + diagonal)
    const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},      // cardinal
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     // diagonal
    };

    // Check each direction
    for (int i = 0; i < 8; ++i)
    {
        int dirRow = directions[i][0];
        int dirCol = directions[i][1];

        // Keep moving in this direction to find pieces to jump over
        int checkRow = row + dirRow;
        int checkCol = col + dirCol;

        while (isValidPosition(checkRow, checkCol, boardSize))
        {
            // If we find a piece (any piece), check if we can land on the other side
            if (!grid[checkRow * boardSize + checkCol].isEmpty())
            {
                // Check the landing position (one step further in same direction)
                int landRow = checkRow + dirRow;
                int landCol = checkCol + dirCol;

                // If landing spot is valid and empty, it's a valid jump
                if (isValidPosition(landRow, landCol, boardSize) &&
                    grid[landRow * boardSize + landCol].isEmpty())
                {
                    moves.push_back({ landRow, landCol });
                }

                // Stop checking this direction after finding first piece
                break;
            }

            // Move to next position in this direction
            checkRow += dirRow;
            checkCol += dirCol;
        }
    }
}




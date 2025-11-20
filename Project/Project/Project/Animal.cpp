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

    initAnimalTexture(50); 
    m_sprite.setPosition(other.m_sprite.getPosition());
}
//assignment operator so when they move into the board grid the ones on the side dont overwrite to the sprite originally there 
Animal& Animal::operator=(const Animal& other)
{
    if (this != &other)
    {
        m_owner = other.m_owner;
        m_type = other.m_type;

        // Reconstruct sprite from scratch
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

Player Animal::getOwner()
{
    return m_owner;
}

AnimalType Animal::getType()
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

    // Recalculate scale based on new cell size
    float scale = (cellSize * 0.8f) / std::max(frameSize.x, frameSize.y);
    m_sprite.setScale(sf::Vector2f(scale, scale));
}

sf::FloatRect Animal::getBounds() const
{
	return m_sprite.getGlobalBounds();
}




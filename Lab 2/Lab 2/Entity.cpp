#include "Entity.h"

void Entity::draw(sf::RenderWindow t_window)
{
	t_window.draw(m_sprite);
}

void Entity::wrapAroundScreen(float windowWidth, float windowHeight) {
	sf::Vector2f pos = m_sprite.getPosition();


	if (pos.x > windowWidth) pos.x = 0;
	else if (pos.x < 0)      pos.x = windowWidth;


	if (pos.y > windowHeight) pos.y = 0;
	else if (pos.y < 0)       pos.y = windowHeight;

	m_sprite.setPosition(pos);

}
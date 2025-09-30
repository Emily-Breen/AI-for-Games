#include "Entity.h"

Entity::Entity()
{
}

void Entity::draw(sf::RenderWindow& t_window)
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

void Entity::setPosition(const sf::Vector2f& position)
{
	m_sprite.setPosition(position);
}

sf::Vector2f Entity::getPosition() const
{
	return m_sprite.getPosition();
}

void Entity::updateAnimation(float dt)
{
	m_animationTimer += dt;
	if (m_animationTimer >= m_animationDelay) {
		m_animationTimer = 0.0f;
		m_currentFrame++;
		if (m_currentFrame > m_lastFrame) {
			m_currentFrame = m_firstFrame;
		}
		
		m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
	}
}

void Entity::setRotation(float angle)
{
	m_sprite.rotate(sf::degrees(angle));
}

float Entity::getRotation() const
{
	return m_sprite.getRotation().asDegrees();
}

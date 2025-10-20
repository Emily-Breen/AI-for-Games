#pragma once
#include <SFML/Graphics.hpp>

class FlowField : public sf::Drawable
{
public:
	FlowField();

private:
	sf::VertexArray m_grid;
	void makeGrid();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};


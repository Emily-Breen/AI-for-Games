#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class UI
{
	
public:
	UI();
	void draw(sf::RenderWindow& m_window);
	void setFont(const sf::Font& font);
	const sf::Font& getFont() const;

	bool loadFont(const std::string& fontPath);
	void setPathfindingMode(bool useAStar);
	void setUpText();
private:
	
	sf::RectangleShape m_background;;
	std::vector<sf::Text> m_texts;
	sf::Font m_font;
	sf::Text m_modeText{ m_font };
	std::string m_currentMode = "BFS";

	
};


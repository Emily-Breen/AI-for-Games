#include "UI.h"

UI::UI()
{
	m_background.setSize(sf::Vector2f(300.0f, 250.0f));
	m_background.setFillColor(sf::Color(0, 0, 0, 180));
	m_background.setOutlineColor(sf::Color::White);
	m_background.setOutlineThickness(2.0f);
	m_background.setPosition(sf::Vector2f(750.0f, 10.0f));
}

void UI::draw(sf::RenderWindow& m_window)
{
	m_window.draw(m_background);
	for (const auto& text : m_texts)
	{
		m_window.draw(text);
	}

	m_window.draw(m_modeText);

}

void UI::setFont(const sf::Font& font)
{
	m_font = font;
}

const sf::Font& UI::getFont() const
{
	return m_font;
}

bool UI::loadFont(const std::string& fontPath)
{
	if (m_font.openFromFile(fontPath))
	{
		std::cout << "Font loaded successfully (UI)\n";
		return true;
	}
    else {
		std::cout << "Error: Could not load font!" << std::endl;
    }
}

void UI::setPathfindingMode(bool useAStar)
{
    m_currentMode = useAStar ? "A* Pathfinding" : "BFS Pathfinding";
	m_modeText.setFont(m_font);
    m_modeText.setCharacterSize(18);
    m_modeText.setFillColor(sf::Color::Yellow);
    m_modeText.setString("Mode: " + m_currentMode);
	m_modeText.setPosition(sf::Vector2f(760.0f, 220.0f));
}

void UI::setUpText()
{
    m_texts.clear();

    std::vector<std::string> instructions = {
        "Flow Field Pathfinding",
		"A: Toggle Pathfinding Mode (BFS(on start)/A*)",
        "Left Click: Set Red Circle (Agent)",
        "Right Click: Set Green Circle (Goal)",
        "Middle Click: Toggle Obstacle",
        "Num 1: Toggle VectorField ON/OFF",
        "Num 2: Toggle Heatmap ON/OFF",
        "Space: Start/Stop Movement",
        "Esc: Exit Game"
    };


    sf::Vector2f bgPos = m_background.getPosition();
    sf::Vector2f bgSize = m_background.getSize();

    float paddingX = 10.0f;
    float paddingY = 15.0f;
    float lineSpacing = 20.0f;

    for (size_t i = 0; i < instructions.size(); ++i)
    {
        sf::Text text{ m_font };
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setString(instructions[i]);


        float x = bgPos.x + paddingX;
        float y = bgPos.y + paddingY + (i * lineSpacing);
        text.setPosition(sf::Vector2f(x, y));

        m_texts.push_back(text);
    }
}

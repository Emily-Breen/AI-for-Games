#include "Board.h"

//Hi Stephen I've marked a few functions with summaries to help explain what they do its mostly because when the window resizes it goes all over the place :( so its sorted now!

/**
 * @brief Constructs a Board with the given size and cell dimensions.
 * @param size The number of cells per side.
 * @param cellSize Initial pixel size of each cell.
 */
Board::Board(int size, float cellSize) : m_size(size), m_cellSize(cellSize)
{

}

/**
 * @brief Draws the board by recalculating offsets then rendering the grid.
 * @param window The window to draw on.
 */
void Board::draw(sf::RenderWindow& window)
{
    recalculateGrid(window);
    drawGrid(window);
}

/**
 * @brief Computes the center position of a given cell.
 * @param row The row index.
 * @param col The column index.
 * @return On-screen center position of the cell.
 */
sf::Vector2f Board::getCellCenter(int row, int col) const
{
    return { m_offsetX + col * m_cellSize + m_cellSize / 2.f,
            m_offsetY + row * m_cellSize + m_cellSize / 2.f };
}

/**
 * @brief Updates board offsets based on window size so the board stays centered.
 * @param m_window The window used to compute offsets.
 */
void Board::recalculateGrid(const sf::RenderWindow& m_window)
{
    float gridSize = m_size * m_cellSize;
    m_offsetX = (m_window.getSize().x - gridSize) / 2.f;
    m_offsetY = (m_window.getSize().y - gridSize) / 2.f;
}

/**
 * @brief Adjusts the cell size proportionally when the window resizes.
 * @param windowSize The current window dimensions.
 */
void Board::updateCellSize(sf::Vector2u windowSize)
{
    float minDimension = std::min(windowSize.x, windowSize.y);
    m_cellSize = (minDimension * 0.7f) / m_size;
}

/**
 * @brief Gets the number of cells per side.
 */
int Board::getSize() const
{
    return m_size;
}

/**
 * @brief Gets the current pixel size of a cell.
 */
float Board::getCellSize() const
{
    return m_cellSize;
}

/**
 * @brief Draws grid cell outlines for the entire board.
 * @param m_window The window to draw on.
 */
void Board::drawGrid(sf::RenderWindow& m_window)
{
    sf::RectangleShape cell(sf::Vector2f(m_cellSize - 2, m_cellSize - 2));
    cell.setFillColor(sf::Color::Transparent);
    cell.setOutlineColor(sf::Color::White);
    cell.setOutlineThickness(1.0f);

    for (int row = 0; row < m_size; ++row) {
        for (int col = 0; col < m_size; ++col) {
            cell.setPosition(sf::Vector2f(
                m_offsetX + col * m_cellSize + 1,
                m_offsetY + row * m_cellSize + 1
            ));
            m_window.draw(cell);
        }
    }
}

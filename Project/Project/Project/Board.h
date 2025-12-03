/**
 * @file Board.h
 * @brief Declares the Board class, responsible for drawing and managing a square game grid.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.h"
#include <vector>

const int BOARD_SIZE = 5;

/**
 * @class Board
 * @brief Handles drawing, sizing, and positioning of the game board grid.
 *
 * The Board manages cell size, window-based scaling, and utility
 * functions for determining cell positions on screen.
 */
class Board
{
public:

    /**
     * @brief Constructs a Board with a given number of cells and base cell size.
     * @param size The width/height of the board in cells.
     * @param cellSize Initial size of each cell in pixels.
     */
    Board(int size, float cellSize);

    /**
     * @brief Draws the full board (recalculates layout then draws the grid).
     * @param window The render window to draw on.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Computes the pixel center of a specific cell.
     * @param row The cell's row index.
     * @param col The cell's column index.
     * @return The on screen center position.
     */
    sf::Vector2f getCellCenter(int row, int col) const;

    /**
     * @brief Recalculates the board's screen offset so it stays centered.
     * @param m_window The window used to determine available space.
     */
    void recalculateGrid(const sf::RenderWindow& m_window);

    /**
     * @brief Updates internal cell size based on the window's dimensions.
     * @param windowSize The new size of the window.
     */
    void updateCellSize(sf::Vector2u windowSize);

    /**
     * @brief Gets the number of cells per side (board size).
     * @return Board side length.
     */
    int getSize() const;

    /**
     * @brief Gets the current pixel size of each cell.
     * @return Cell size in pixels.
     */
    float getCellSize() const;

private:
    int m_size;          ///< Width/height of the board in cells.
    float m_cellSize;    ///< Current pixel size of each cell.
    float m_offsetX{ 0.f };///< Left offset from window edge to board start.
    float m_offsetY{ 0.f };///< Top offset from window edge to board start.

    /**
     * @brief Draws the individual grid lines and cells.
     * @param window Window to draw the grid onto.
     */
    void drawGrid(sf::RenderWindow& window);
};

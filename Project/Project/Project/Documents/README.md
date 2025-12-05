# Foruth Protocol

## % Of Work Done
Effort done on this project was 50/50.

## Extras: Multiple Game Modes

### Implementation Details

The game supports three different match configurations through boolean flags:

```cpp
bool m_player2IsAI{ true };   // Controls if Player 2 is AI-controlled
bool m_player1IsAI{ false };  // Controls if Player 1 is AI-controlled
```

### Game Mode Configuration

#### 1. **Human vs. Human (HvH)**
- **Configuration**: `m_player1IsAI = false`, `m_player2IsAI = false`
- **Behavior**: Both players control their pieces manually through mouse input
- **Turn Flow**: Players alternate turns, each controlling their pieces directly

#### 2. **Human vs. AI (HvAI)**
- **Configuration**: `m_player1IsAI = false`, `m_player2IsAI = true`
- **Behavior**: 
  - Player 1 uses mouse input to place and move pieces
  - Player 2's turns are handled automatically by the `handleAITurn()` function
- **Turn Flow**: Human moves → AI calculates and executes move → repeat

#### 3. **AI vs. AI (AIvAI)**
- **Configuration**: `m_player1IsAI = true`, `m_player2IsAI = true`
- **Behavior**: Both players are controlled by AI, allowing observation of AI strategy
- **Turn Flow**: Both turns call `handleAITurn()` in sequence

#### Turn Management (`Game.cpp`)
The game checks these flags during the update loop to determine if AI should make a move:

```cpp
void Game::update(sf::Time t_deltaTime)
{
    if (m_currentGameState == GameState::Placement || 
        m_currentGameState == GameState::Movement)
    {
        // Check if current player is AI-controlled
        if ((m_currentPlayer == Player::Player1 && m_player1IsAI) ||
            (m_currentPlayer == Player::Player2 && m_player2IsAI))
        {
            handleAITurn();
        }
    }
}
```

#### AI Decision Making (`handleAITurn()`)
When called, this function:
1. Converts the current board state to a `Boardstate` object
2. Calls `m_aiPlayer.chooseBestMove()` with the current state
3. Uses **minimax algorithm with alpha-beta pruning** to evaluate possible moves
4. Executes the best move found
5. Automatically switches to the next player's turn

#### AI Algorithm (`Gameplay.cpp`)
The AI uses:
- **Minimax Algorithm**: Searches the game tree to find optimal moves
- **Alpha-Beta Pruning**: Optimizes search by eliminating branches that won't affect the outcome
- **Heuristic Evaluation**: Scores board positions based on:
  - Win conditions (immediate or potential)
  - Three-in-a-row threats
  - Two-in-a-row patterns
  - Piece positioning

### Menu Button Setup
Players select their preferred game mode from the main menu:
- **HvH Button**: Sets both AI flags to `false`
- **HvAI Button**: Sets `m_player1IsAI = false`, `m_player2IsAI = true`
- **AIvAI Button**: Sets both AI flags to `true`

### Board State Conversion
The `getCurrentBoardState()` function converts the visual game grid into a lightweight `Boardstate` structure that the AI can efficiently process:

```cpp
Boardstate Game::getCurrentBoardState() const
{
    PieceState grid[BOARD_SIZE][BOARD_SIZE];
    
    // Convert Animal objects to PieceState (owner + type only)
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            grid[row][col] = Gameplay::toPieceState(m_grid[row][col]);
        }
    }
    
    return Boardstate(grid, m_currentPlayer);
}
```
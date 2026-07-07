#include "ConsoleRenderer.h"

#include <iostream>
#include <vector>

#include "../../core/Game.h"

void ConsoleRenderer::render(const Game& game) {
    const Board& board = game.getBoard();
    int width = board.getWidth();
    int height = board.getHeight();

    // Buffer aus dem statischen Board-Zustand aufbauen
    std::vector<std::string> lines(height, std::string(width, '.'));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (board.getCell(x, y).has_value()) {
                lines[y][x] = '#';
            }
        }
    }

    // Aktives Tetromino ueberlagern (falls vorhanden)
    if (const Tetromino* piece = game.getCurrentPiece()) {
        for (const Position& cell : piece->getOccupiedCells()) {
            if (cell.x >= 0 && cell.x < width && cell.y >= 0 && cell.y < height) {
                lines[cell.y][cell.x] = '@';
            }
        }
    }

    std::cout << "\n";
    std::cout << "Score: " << game.getScore()
               << "  Level: " << game.getLevel()
               << "  Lines: " << game.getLinesCleared() << "\n";
    std::cout << "+" << std::string(width, '-') << "+\n";
    for (const std::string& line : lines) {
        std::cout << "|" << line << "|\n";
    }
    std::cout << "+" << std::string(width, '-') << "+\n";
}

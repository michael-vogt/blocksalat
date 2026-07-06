#include "Board.h"

Board::Board(int width, int height)
    : width_(width)
    , height_(height)
    , grid_(height, std::vector<std::optional<TetrominoType>>(width, std::nullopt))
{
}

int Board::getWidth() const {
    return width_;
}

int Board::getHeight() const {
    return height_;
}

std::optional<TetrominoType> Board::getCell(int x, int y) const {
    if (!isInsideBounds(Position{ x, y })) {
        return std::nullopt;
    }
    return grid_[y][x];
}

bool Board::isInsideBounds(const Position& pos) const {
    return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
}

bool Board::isValidPosition(const std::vector<Position>& cells) const {
    for (const Position& pos : cells) {
        // Grenzen prüfen
        if (!isInsideBounds(pos)) {
            return false;
        }
        // Überlappung prüfen
        if (grid_[pos.y][pos.x].has_value()) {
            return false;
        }
    }
    return true;
}

void Board::lockPiece(const std::vector<Position>& cells, TetrominoType type) {
    for (const Position& pos : cells) {
        if (isInsideBounds(pos)) {
            grid_[pos.y][pos.x] = type;
        }
    }
}

bool Board::isRowFull(int row) const {
    for (int x = 0; x < width_; ++x) {
        if (!grid_[row][x].has_value()) {
            return false;
        }
    }
    return true;
}

int Board::clearFullLines() {
    std::vector<std::vector<std::optional<TetrominoType>>> newGrid;
    newGrid.reserve(height_);

    int clearedCount = 0;

    // Alle nicht-vollen Reihen von oben nach unten in newGrid übernehmen
    for (int row = 0; row < height_; ++row) {
        if (isRowFull(row)) {
            ++clearedCount;
        } else {
            newGrid.push_back(grid_[row]);
        }
    }

    // Für jede gelöschte Reihe eine neue, leere Reihe oben einfügen,
    // damit die Gesamthöhe erhalten bleibt
    for (int i = 0; i < clearedCount; ++i) {
        newGrid.insert(newGrid.begin(), std::vector<std::optional<TetrominoType>>(width_, std::nullopt));
    }

    grid_ = std::move(newGrid);
    return clearedCount;
}

bool Board::isGameOver() const {
    // Einfachste Variante: wenn in der obersten Reihe (Spawn-Zone) bereits
    // irgendeine Zelle belegt ist, ist kein Platz mehr für neue Tetrominos.
    for (int x = 0; x < width_; ++x) {
        if (grid_[0][x].has_value()) {
            return true;
        }
    }
    return false;
}

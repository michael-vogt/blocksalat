#include "Tetromino.h"

// ---------------------------------------------------------------------------
// Formen-Tabelle: konkrete Daten
// ---------------------------------------------------------------------------
//
// Reihenfolge der Rotationszustände pro Typ: 0 (Spawn), R (90° im UZS), 2 (180°), L (270°)
// Hinweis: Diese Grids orientieren sich am offiziellen SRS-Standard.
// Es lohnt sich, sie später gegen eine Referenztabelle zu verifizieren,
// falls beim Testen Abweichungen zum erwarteten Verhalten auffallen.

const std::array<ShapeRotations, static_cast<size_t>(TetrominoType::Count)> kShapeTable = { {
    // I
    { {
        { { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} } },
        { { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0} } },
        { { {0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0} } },
    } },
    // O
    { {
        { { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} } },
    } },
    // T
    { {
        { { {0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,1,0,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,1,0,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {0,1,0,0} } },
    } },
    // S
    { {
        { { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,1,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {0,1,1,0}, {1,1,0,0} } },
        { { {0,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,1,0,0} } },
    } },
    // Z
    { {
        { { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,0,1,0}, {0,1,1,0}, {0,1,0,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {1,1,0,0}, {0,1,1,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {1,0,0,0} } },
    } },
    // J
    { {
        { { {0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,1,0}, {0,1,0,0}, {0,1,0,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,0,1,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {1,1,0,0} } },
    } },
    // L
    { {
        { { {0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0} } },
        { { {0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,1,0} } },
        { { {0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {1,0,0,0} } },
        { { {0,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,0,0} } },
    } },
} };

// ---------------------------------------------------------------------------
// Tetromino-Implementierung
// ---------------------------------------------------------------------------

Tetromino::Tetromino(TetrominoType type, Position spawnPosition)
    : type_(type)
    , state_{ spawnPosition, 0 }
{
}

TetrominoType Tetromino::getType() const {
    return type_;
}

const TetrominoState& Tetromino::getState() const {
    return state_;
}

std::vector<Position> Tetromino::getOccupiedCells(const TetrominoState& state) const {
    std::vector<Position> cells;
    cells.reserve(4);

    const ShapeGrid& grid = kShapeTable[static_cast<size_t>(type_)][state.rotationIndex];

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (grid[row][col]) {
                // col entspricht x, row entspricht y innerhalb der Box
                cells.push_back(state.position + Position{ col, row });
            }
        }
    }

    return cells;
}

std::vector<Position> Tetromino::getOccupiedCells() const {
    return getOccupiedCells(state_);
}

TetrominoState Tetromino::previewMove(int dx, int dy) const {
    TetrominoState newState = state_;
    newState.position = newState.position + Position{ dx, dy };
    return newState;
}

TetrominoState Tetromino::previewRotateClockwise() const {
    TetrominoState newState = state_;
    newState.rotationIndex = (newState.rotationIndex + 1) % 4;
    return newState;
}

TetrominoState Tetromino::previewRotateCounterClockwise() const {
    TetrominoState newState = state_;
    newState.rotationIndex = (newState.rotationIndex + 3) % 4; // +3 statt -1, um negative Werte zu vermeiden
    return newState;
}

void Tetromino::applyState(const TetrominoState& newState) {
    state_ = newState;
}

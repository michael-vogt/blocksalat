#include "Game.h"

namespace {
    // Spawn-Position: 4x4-Box horizontal mittig, oben am Spielfeld
    Position calculateSpawnPosition(int boardWidth) {
        return Position{ (boardWidth - 4) / 2, 0 };
    }
}

Game::Game(int boardWidth, int boardHeight)
    : board_(boardWidth, boardHeight)
    , nextType_(TetrominoType::I) // Platzhalter, wird unten sofort ueberschrieben
    , randomEngine_(std::random_device{}())
    , typeDistribution_(0, static_cast<int>(TetrominoType::Count) - 1)
{
    nextType_ = generateRandomType();
    spawnNextPiece();
}

TetrominoType Game::generateRandomType() {
    return static_cast<TetrominoType>(typeDistribution_(randomEngine_));
}

void Game::spawnNextPiece() {
    TetrominoType typeToSpawn = nextType_;
    nextType_ = generateRandomType();

    Position spawnPosition = calculateSpawnPosition(board_.getWidth());
    currentPiece_.emplace(typeToSpawn, spawnPosition);

    // Wenn direkt beim Spawnen schon eine Kollision vorliegt, ist das Spiel vorbei.
    if (!board_.isValidPosition(currentPiece_->getOccupiedCells())) {
        gameOver_ = true;
    }
}

bool Game::tryMove(int dx, int dy) {
    if (!currentPiece_.has_value()) {
        return false;
    }

    TetrominoState newState = currentPiece_->previewMove(dx, dy);
    std::vector<Position> cells = currentPiece_->getOccupiedCells(newState);

    if (board_.isValidPosition(cells)) {
        currentPiece_->applyState(newState);
        return true;
    }
    return false;
}

bool Game::tryRotate(bool clockwise) {
    if (!currentPiece_.has_value()) {
        return false;
    }

    TetrominoState newState = clockwise
        ? currentPiece_->previewRotateClockwise()
        : currentPiece_->previewRotateCounterClockwise();

    std::vector<Position> cells = currentPiece_->getOccupiedCells(newState);

    if (board_.isValidPosition(cells)) {
        currentPiece_->applyState(newState);
        return true;
    }
    return false;
    // Hinweis: hier waere der natuerliche Ansatzpunkt fuer spaetere Wall-Kicks -
    // bei ungueltiger Rotation zusaetzliche Positions-Offsets probieren, bevor man aufgibt.
}

void Game::lockCurrentPiece() {
    if (!currentPiece_.has_value()) {
        return;
    }

    board_.lockPiece(currentPiece_->getOccupiedCells(), currentPiece_->getType());
    int clearedNow = board_.clearFullLines();
    updateScoreAndLevel(clearedNow);

    spawnNextPiece();
}

void Game::updateScoreAndLevel(int linesClearedNow) {
    if (linesClearedNow <= 0) {
        return;
    }

    // Einfaches Scoring-Schema (angelehnt an klassische Punktevergabe pro Level):
    // mehr gleichzeitig geloeschte Reihen = ueberproportional mehr Punkte.
    static const int kPointsPerLineCount[5] = { 0, 100, 300, 500, 800 }; // Index = Anzahl Reihen (max 4)
    int index = linesClearedNow;
    if (index > 4) index = 4;

    score_ += kPointsPerLineCount[index] * level_;
    linesCleared_ += linesClearedNow;

    // Einfache Level-Progression: alle 10 geloeschten Reihen ein Level aufsteigen.
    level_ = 1 + (linesCleared_ / 10);
}

double Game::calculateFallIntervalSeconds() const {
    // Einfache Formel: startet bei 1.0s, wird pro Level etwas schneller, mit unterer Grenze.
    double interval = 1.0 - (level_ - 1) * 0.08;
    return interval < 0.1 ? 0.1 : interval;
}

void Game::handleInput(InputAction action) {
    if (gameOver_ || !currentPiece_.has_value()) {
        return;
    }

    switch (action) {
        case InputAction::MoveLeft:
            tryMove(-1, 0);
            break;
        case InputAction::MoveRight:
            tryMove(1, 0);
            break;
        case InputAction::SoftDrop:
            if (!tryMove(0, 1)) {
                lockCurrentPiece();
            }
            break;
        case InputAction::HardDrop:
            while (tryMove(0, 1)) {
                // so lange nach unten bewegen, bis es nicht mehr geht
            }
            lockCurrentPiece();
            break;
        case InputAction::RotateClockwise:
            tryRotate(true);
            break;
        case InputAction::RotateCounterClockwise:
            tryRotate(false);
            break;
    }
}

void Game::update(double deltaTimeSeconds) {
    if (gameOver_) {
        return;
    }

    fallTimerSeconds_ += deltaTimeSeconds;

    double fallInterval = calculateFallIntervalSeconds();
    if (fallTimerSeconds_ >= fallInterval) {
        fallTimerSeconds_ -= fallInterval;

        if (!tryMove(0, 1)) {
            lockCurrentPiece();
        }
    }
}

const Board& Game::getBoard() const {
    return board_;
}

bool Game::hasCurrentPiece() const {
    return currentPiece_.has_value();
}

const Tetromino* Game::getCurrentPiece() const {
    return currentPiece_.has_value() ? &(*currentPiece_) : nullptr;
}

TetrominoType Game::peekNextType() const {
    return nextType_;
}

int Game::getScore() const {
    return score_;
}

int Game::getLevel() const {
    return level_;
}

int Game::getLinesCleared() const {
    return linesCleared_;
}

bool Game::isGameOver() const {
    return gameOver_;
}

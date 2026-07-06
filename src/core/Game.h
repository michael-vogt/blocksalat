#pragma once

#include <optional>
#include <random>

#include "Board.h"
#include "Tetromino.h"

// ---------------------------------------------------------------------------
// Eingabe-Aktionen
// ---------------------------------------------------------------------------
//
// Abstrakte Aktionen, die z.B. von einem InputHandler erzeugt und an Game
// weitergereicht werden. Game kennt keine konkreten Tasten, nur diese Aktionen.

enum class InputAction {
    MoveLeft,
    MoveRight,
    SoftDrop,       // ein Feld schneller nach unten (Spieler haelt Taste gedrueckt)
    HardDrop,       // sofort ganz nach unten fallen lassen und einbrennen
    RotateClockwise,
    RotateCounterClockwise
};

// ---------------------------------------------------------------------------
// Game-Klasse
// ---------------------------------------------------------------------------

class Game {
public:
    explicit Game(int boardWidth = Board::kDefaultWidth, int boardHeight = Board::kDefaultHeight);

    // Verarbeitet eine einzelne Eingabe-Aktion (Bewegung/Rotation des aktiven Tetrominos).
    // Nutzt intern konsequent das "try before commit"-Prinzip ueber Tetromino/Board.
    void handleInput(InputAction action);

    // Muss regelmaessig aufgerufen werden (z.B. einmal pro Frame).
    // deltaTimeSeconds: vergangene Zeit seit dem letzten Aufruf, in Sekunden.
    // Kuemmert sich um automatisches Faellen (Gravity) basierend auf dem aktuellen Level.
    void update(double deltaTimeSeconds);

    // --- Zustandsabfragen (z.B. fuer einen Renderer) ---

    const Board& getBoard() const;

    // Gibt true zurueck, wenn gerade ein aktives Tetromino existiert
    // (i.d.R. immer der Fall, ausser ganz am Anfang oder nach Game Over).
    bool hasCurrentPiece() const;

    // Gibt einen Zeiger auf das aktive Tetromino zurueck, oder nullptr,
    // falls keines existiert. Der Aufrufer MUSS auf nullptr pruefen
    // (oder vorher hasCurrentPiece() abfragen), bevor er den Zeiger nutzt.
    const Tetromino* getCurrentPiece() const;

    TetrominoType peekNextType() const;

    int getScore() const;
    int getLevel() const;
    int getLinesCleared() const;
    bool isGameOver() const;

private:
    // Erzeugt einen zufaelligen Tetromino-Typ (einfache Gleichverteilung ueber alle 7 Typen).
    TetrominoType generateRandomType();

    // Spawnt ein neues aktives Tetromino (Typ kommt aus nextType_), zieht den naechsten Typ nach.
    // Setzt gameOver_ = true, falls direkt beim Spawnen keine gueltige Position existiert.
    void spawnNextPiece();

    // Versucht, das aktive Tetromino um (dx, dy) zu bewegen. Gibt zurueck, ob die Bewegung gueltig war.
    bool tryMove(int dx, int dy);

    // Versucht, das aktive Tetromino zu rotieren. Gibt zurueck, ob die Rotation gueltig war.
    bool tryRotate(bool clockwise);

    // Brennt das aktive Tetromino ins Board ein, loescht volle Reihen, aktualisiert Score/Level,
    // und spawnt danach das naechste Tetromino.
    void lockCurrentPiece();

    // Berechnet das aktuelle Fallintervall (in Sekunden) basierend auf dem Level -
    // hoeheres Level = kuerzeres Intervall = schnelleres Fallen.
    double calculateFallIntervalSeconds() const;

    // Aktualisiert Score und Level anhand der Anzahl gleichzeitig geloeschter Reihen.
    void updateScoreAndLevel(int linesClearedNow);

    Board board_;
    std::optional<Tetromino> currentPiece_;
    TetrominoType nextType_;

    int score_ = 0;
    int level_ = 1;
    int linesCleared_ = 0;
    bool gameOver_ = false;

    double fallTimerSeconds_ = 0.0;

    std::mt19937 randomEngine_;
    std::uniform_int_distribution<int> typeDistribution_;
};

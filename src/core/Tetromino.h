#pragma once

#include <array>
#include <vector>

// ---------------------------------------------------------------------------
// Grundlegende Datentypen
// ---------------------------------------------------------------------------

// Die 7 klassischen Tetromino-Typen
enum class TetrominoType {
    I,
    O,
    T,
    S,
    Z,
    J,
    L,
    Count // Hilfswert, um die Anzahl der Typen zu bekommen (= 7)
};

// Eine Position auf dem Spielfeld (bzw. relative Position innerhalb einer 4x4-Box)
struct Position {
    int x = 0;
    int y = 0;

    Position operator+(const Position& other) const {
        return Position{ x + other.x, y + other.y };
    }
};

// Der vollständige veränderliche Zustand eines Tetrominos:
// - position: Position der 4x4-Bounding-Box auf dem Spielfeld (z.B. obere linke Ecke)
// - rotationIndex: 0-3, Index in die Formen-Tabelle (0 = Spawn-Zustand, dann im Uhrzeigersinn)
struct TetrominoState {
    Position position;
    int rotationIndex = 0;
};

// ---------------------------------------------------------------------------
// Formen-Tabelle (SRS - Super Rotation System)
// ---------------------------------------------------------------------------
//
// Jede Form wird als 4x4-Grid aus bool-Werten dargestellt (true = belegte Zelle).
// Es gibt 7 Typen x 4 Rotationszustände = 28 Grids.
//
// Zeilen-Reihenfolge: erste Zeile = oberste Zeile der Box (y=0), erste Spalte = linkeste Spalte (x=0)

using ShapeGrid = std::array<std::array<bool, 4>, 4>; // [row][col]
using ShapeRotations = std::array<ShapeGrid, 4>;      // 4 Rotationszustände pro Typ

// Deklaration der globalen, programmweiten Formen-Tabelle.
// Die eigentlichen Daten stehen in Tetromino.cpp
extern const std::array<ShapeRotations, static_cast<size_t>(TetrominoType::Count)> kShapeTable;

// ---------------------------------------------------------------------------
// Tetromino-Klasse
// ---------------------------------------------------------------------------

class Tetromino {
public:
    // Erzeugt ein Tetromino des gegebenen Typs an einer Startposition (Spawn-Zustand, rotationIndex = 0)
    Tetromino(TetrominoType type, Position spawnPosition);

    // --- Zugriff auf aktuellen Zustand ---

    TetrominoType getType() const;
    const TetrominoState& getState() const;

    // Gibt die absoluten Board-Koordinaten für einen beliebigen State zurück
    // (funktioniert sowohl für den aktuellen als auch für einen hypothetischen/Preview-State).
    std::vector<Position> getOccupiedCells(const TetrominoState& state) const;

    // Bequemlichkeits-Überladung: nutzt den aktuellen internen State
    std::vector<Position> getOccupiedCells() const;

    // --- Preview-Methoden: berechnen nur, verändern NICHT den internen Zustand ---

    TetrominoState previewMove(int dx, int dy) const;
    TetrominoState previewRotateClockwise() const;
    TetrominoState previewRotateCounterClockwise() const;

    // --- Apply-Methode: übernimmt einen (bereits geprüften) State als neuen internen Zustand ---

    void applyState(const TetrominoState& newState);

private:
    TetrominoType type_;
    TetrominoState state_;
};

#pragma once

#include <optional>
#include <vector>

#include "Tetromino.h"

// ---------------------------------------------------------------------------
// Board-Klasse
// ---------------------------------------------------------------------------
//
// Kapselt den kompletten Zustand und die Regeln des Spielfelds:
// - kennt belegte/leere Zellen
// - bietet Kollisionsprüfung an (isValidPosition)
// - "brennt" gelandete Tetrominos ein (lockPiece)
// - erkennt und löscht volle Reihen (clearFullLines)
//
// Board kennt die Klasse Tetromino NICHT - es arbeitet nur mit Position-Listen,
// die ihm von außen (i.d.R. von Game) übergeben werden.

class Board {
public:
    static constexpr int kDefaultWidth = 10;
    static constexpr int kDefaultHeight = 20;

    Board(int width = kDefaultWidth, int height = kDefaultHeight);

    int getWidth() const;
    int getHeight() const;

    // Gibt den Inhalt einer Zelle zurück:
    // - std::nullopt, wenn die Zelle leer ist
    // - sonst den Typ des Tetrominos, das dort liegt (nützlich fürs Rendering/Farbe)
    // Bei ungültigen Koordinaten wird ebenfalls std::nullopt zurückgegeben.
    std::optional<TetrominoType> getCell(int x, int y) const;

    // Prüft, ob die gegebenen Zellen gültig sind:
    // - alle Zellen innerhalb der Board-Grenzen
    // - keine der Zellen bereits belegt
    bool isValidPosition(const std::vector<Position>& cells) const;

    // Schreibt die gegebenen Zellen dauerhaft ins Board (nach erfolgreichem Landen).
    // Es wird NICHT geprüft, ob die Position gültig ist - das sollte vorher
    // via isValidPosition() sichergestellt werden.
    void lockPiece(const std::vector<Position>& cells, TetrominoType type);

    // Erkennt volle Reihen, entfernt sie und lässt darüberliegende Reihen nachrutschen.
    // Gibt die Anzahl der gelöschten Reihen zurück (nützlich fürs Scoring).
    int clearFullLines();

    // Prüft, ob das Spiel vorbei ist, d.h. ob die Spawn-Zone (oberste Reihe) blockiert ist.
    bool isGameOver() const;

private:
    bool isInsideBounds(const Position& pos) const;
    bool isRowFull(int row) const;

    int width_;
    int height_;

    // grid_[y][x]: äußerer Index = Zeile (y), innerer Index = Spalte (x)
    std::vector<std::vector<std::optional<TetrominoType>>> grid_;
};

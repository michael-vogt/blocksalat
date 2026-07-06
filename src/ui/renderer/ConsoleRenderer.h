#pragma once

#include "IRenderer.h"

// Einfache Textausgabe des Spielzustands auf der Konsole.
// Dient als erste, minimale Renderer-Implementierung zum Testen des Spielkerns.
class ConsoleRenderer : public IRenderer {
public:
    void render(const Game& game) override;
};

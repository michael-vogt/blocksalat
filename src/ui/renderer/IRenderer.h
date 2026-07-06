#pragma once

class Game; // Vorwaertsdeklaration reicht, da wir nur eine Referenz brauchen

// ---------------------------------------------------------------------------
// IRenderer
// ---------------------------------------------------------------------------
//
// Abstrakte Schnittstelle fuer alles, was den Spielzustand darstellen kann.
// Ein Renderer LIEST nur den Game-Zustand, er veraendert ihn nie.
//
// Konkrete Implementierungen: ConsoleRenderer (Textausgabe), spaeter z.B.
// SfmlRenderer oder SdlRenderer fuer grafische Darstellung - Game und main.cpp
// muessten dafuer nicht angepasst werden.

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void render(const Game& game) = 0;
};

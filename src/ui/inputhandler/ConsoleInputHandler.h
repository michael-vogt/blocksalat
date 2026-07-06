#pragma once

#include "IInputHandler.h"

// Blockierende, Enter-basierte Konsoleneingabe (Platzhalter, siehe main.cpp-
// Kommentare zur Einschraenkung von Standard-C++ bei Tastatureingaben).
class ConsoleInputHandler : public IInputHandler {
public:
    std::optional<InputAction> pollAction() override;
    bool isQuitRequested() const override;

private:
    bool quitRequested_ = false;
};

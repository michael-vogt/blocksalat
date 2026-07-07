#include "ConsoleInputHandler.h"

#include <iostream>
#include <string>

std::optional<InputAction> ConsoleInputHandler::pollAction() {
    std::cout << "> ";
    std::string input;

    if (!std::getline(std::cin, input)) {
        // Eingabestrom beendet (z.B. Ctrl+D) - als Beenden-Wunsch werten
        quitRequested_ = true;
        return std::nullopt;
    }

    if (!input.empty() && input[0] == 'e') {
        quitRequested_ = true;
        return std::nullopt;
    }

    if (input.empty()) {
        return std::nullopt; // leere Eingabe = einfach nur Zeit vergehen lassen
    }

    switch (input[0]) {
        case 'a': return InputAction::MoveLeft;
        case 'd': return InputAction::MoveRight;
        case 's': return InputAction::SoftDrop;
        case 'w': return InputAction::RotateClockwise;
        case 'q': return InputAction::RotateCounterClockwise;
        case 'x': return InputAction::HardDrop;
        default:  return std::nullopt;
    }
}

bool ConsoleInputHandler::isQuitRequested() const {
    return quitRequested_;
}

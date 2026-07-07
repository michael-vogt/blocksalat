#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "ui/console/ConsoleInputHandler.h"
#include "ui/console/ConsoleRenderer.h"
#include "core/Game.h"
#include "ui/IInputHandler.h"
#include "ui/IRenderer.h"

int main() {
    Game game;

    // Beide als Zeiger auf die jeweilige Basisklasse - so koennte man spaeter
    // z.B. std::make_unique<SfmlRenderer>() bzw. std::make_unique<SfmlInputHandler>()
    // einsetzen, ohne den Rest von main.cpp oder Game anzufassen.
    std::unique_ptr<IRenderer> renderer = std::make_unique<ConsoleRenderer>();
    std::unique_ptr<IInputHandler> inputHandler = std::make_unique<ConsoleInputHandler>();

    std::cout << "Steuerung: a=links  d=rechts  w=drehen(rechts)  q=drehen(links)\n";
    std::cout << "           s=soft drop  x=hard drop  e=beenden\n";
    std::cout << "Nach jeder Eingabe einfach Enter druecken. Leere Eingabe = nur Zeit vergeht.\n";

    auto lastTime = std::chrono::steady_clock::now();

    while (!game.isGameOver() && !inputHandler->isQuitRequested()) {
        renderer->render(game);

        if (auto action = inputHandler->pollAction()) {
            game.handleInput(*action);
        }

        if (inputHandler->isQuitRequested()) {
            std::cout << "Spiel beendet.\n";
            break;
        }

        // Vergangene reale Zeit seit dem letzten Durchlauf ermitteln,
        // damit die Gravity-Logik in Game.update() sinnvoll arbeiten kann.
        auto now = std::chrono::steady_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        game.update(elapsedSeconds);
    }

    if (game.isGameOver()) {
        renderer->render(game);
        std::cout << "\nGame Over! Endstand: " << game.getScore() << " Punkte.\n";
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#include "core/Game.h"
#include "../SfmlInputHandler.h"
#include "../SfmlRenderer.h"

int main() {
    Game game;

    const int cellSize = 30;
    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned int>(game.getBoard().getWidth() * cellSize),
                      static_cast<unsigned int>(game.getBoard().getHeight() * cellSize)),
        "Tetris"
    );
    window.setFramerateLimit(60);

    // Beide Klassen teilen sich dasselbe Fenster (siehe Kommentare in den Headern),
    // sind aber ansonsten komplett unabhaengig voneinander - genau wie bei der
    // Konsolen-Variante mit ConsoleRenderer/ConsoleInputHandler.
    SfmlRenderer renderer(window);
    SfmlInputHandler inputHandler(window);

    sf::Clock clock;

    while (window.isOpen() && !game.isGameOver() && !inputHandler.isQuitRequested()) {
        if (auto action = inputHandler.pollAction()) {
            game.handleInput(*action);
        }

        double elapsedSeconds = clock.restart().asSeconds();
        game.update(elapsedSeconds);

        renderer.render(game);
    }

    // Falls das Spiel durch Game Over endete (nicht durch Fenster schliessen),
    // Endstand kurz im Titel anzeigen, bevor das Fenster (falls noch offen) zu bleibt.
    if (game.isGameOver() && window.isOpen()) {
        window.setTitle("Game Over! Endstand: " + std::to_string(game.getScore()));
    }

    return 0;
}

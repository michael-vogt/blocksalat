#pragma once

#include <optional>

#include "../../core/Game.h"

// ---------------------------------------------------------------------------
// IInputHandler
// ---------------------------------------------------------------------------
//
// Abstrakte Schnittstelle, die rohe Eingaben (Tastatur, o.ae.) in abstrakte
// InputAction-Werte uebersetzt. Game kennt keine konkreten Tasten, nur
// InputAction - dadurch ist die Eingabequelle austauschbar.
//
// Konkrete Implementierungen: ConsoleInputHandler (blockierend, Enter-basiert),
// spaeter z.B. ein SfmlInputHandler, der Tastendruecke nicht-blockierend
// abfragt (pollAction() wuerde dann sofort std::nullopt zurueckgeben,
// wenn gerade keine neue Taste gedrueckt wurde, statt zu warten).

class IInputHandler {
public:
    virtual ~IInputHandler() = default;

    // Liefert die naechste vom Spieler ausgeloeste Aktion, falls eine vorliegt.
    // Ob dieser Aufruf blockiert (auf Eingabe wartet) oder sofort zurueckkehrt,
    // haengt von der konkreten Implementierung ab - siehe deren Dokumentation.
    virtual std::optional<InputAction> pollAction() = 0;

    // Gibt an, ob der Spieler das Programm beenden moechte (z.B. "e" gedrueckt,
    // oder Eingabestrom wurde geschlossen).
    virtual bool isQuitRequested() const = 0;
};

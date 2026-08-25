#pragma once

#include <SFML/System/Vector2.hpp>
#include "../chess/chess.hpp"

class BoardCoords {
public:
    static chess::Square screenToSquare(sf::Vector2f boardLocalPos, float pixelScale);

    static sf::Vector2f squareToScreen(chess::Square sq, float pixelScale);

    static const sf::Vector2f BOARD_ORIGIN;
};

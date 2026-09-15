#pragma once

#include <SFML/System/Vector2.hpp>
#include "../chess/chess.hpp"
#include "../render/BoardLayout.hpp"

class BoardCoords {
public:
    static chess::Square screenToSquare(sf::Vector2f boardLocalPos, const BoardLayout &layout);

    static sf::Vector2f squareToScreen(chess::Square sq, const BoardLayout &layout);
};
